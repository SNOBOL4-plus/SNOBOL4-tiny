/*
 * monitor_ipc_sync.c — SNOBOL4 LOAD()able sync-step IPC module.
 *
 * Replaces monitor_ipc.c with a barrier protocol:
 *   MON_SEND blocks after writing each event until the controller
 *   sends a 1-byte acknowledgment ('G' = go, 'S' = stop).
 *
 * Two FIFOs per participant:
 *   MONITOR_FIFO     — event FIFO  (participant writes, controller reads)
 *   MONITOR_ACK_FIFO — ack FIFO    (controller writes, participant reads)
 *
 * Protocol per trace event:
 *   1. participant writes "KIND body\n" to event FIFO
 *   2. participant blocks read() on ack FIFO
 *   3. controller reads event from all 5 event FIFOs
 *   4. controller compares — consensus rule
 *   5. controller writes 'G' (go) or 'S' (stop) to each ack FIFO
 *   6. participant receives ack:
 *      'G' → MON_SEND returns success, participant continues
 *      'S' → MON_SEND returns FAIL, participant exits via :F(END)
 *
 * Build:
 *   gcc -shared -fPIC -O2 -Wall -o monitor_ipc_sync.so monitor_ipc_sync.c
 */

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

typedef long      int_t;
typedef double    real_t;

struct descr {
    union { int_t i; real_t f; } a;
    char         f;
    unsigned int v;
};

#define DESCR_SZ   ((int)sizeof(struct descr))
#define BCDFLD     (4 * DESCR_SZ)

#define LOAD_PROTO  struct descr *retval, unsigned nargs, struct descr *args
#define LA_ALIST    LOAD_PROTO
typedef int lret_t;

#define TRUE  1
#define FALSE 0

static inline void *_blk(int n, struct descr *args) {
    return (void *)(args[n].a.i);
}
static inline int _len(int n, struct descr *args) {
    void *blk = _blk(n, args);
    if (!blk) return 0;
    return (int)((struct descr *)blk)->v;
}
static inline const char *_ptr(int n, struct descr *args) {
    void *blk = _blk(n, args);
    if (!blk) return NULL;
    return (const char *)blk + BCDFLD;
}

extern void retstring(struct descr *retval, const char *cp, int len);

#define RETSTR(CP, LEN) \
    do { retstring(retval, (CP), (LEN)); return TRUE; } while (0)
#define RETNULL \
    do { retval->a.i = 0; retval->f = 0; retval->v = 0; return TRUE; } while (0)
#define RETFAIL return FALSE

static int copy_str_arg(int n, char *buf, int bufsz, struct descr *args) {
    int len = _len(n, args);
    const char *ptr = _ptr(n, args);
    if (len < 0 || len >= bufsz) return -1;
    if (ptr && len > 0) memcpy(buf, ptr, len);
    buf[len] = '\0';
    return 0;
}

/* Module state */
static int mon_event_fd = -1;   /* write end of event FIFO */
static int mon_ack_fd   = -1;   /* read end of ack FIFO   */

/*
 * MON_OPEN(event_fifo_path, ack_fifo_path) → event_fifo_path or FAIL
 */
lret_t MON_OPEN(LA_ALIST) {
    char event_path[4096];
    char ack_path[4096];

    if (copy_str_arg(0, event_path, sizeof(event_path), args) < 0) RETFAIL;
    if (copy_str_arg(1, ack_path,   sizeof(ack_path),   args) < 0) RETFAIL;
    if (!event_path[0] || !ack_path[0]) RETFAIL;

    if (mon_event_fd >= 0) { close(mon_event_fd); mon_event_fd = -1; }
    if (mon_ack_fd   >= 0) { close(mon_ack_fd);   mon_ack_fd   = -1; }

    /* Event FIFO: write end — non-blocking open (reader already waiting) */
    mon_event_fd = open(event_path, O_WRONLY | O_NONBLOCK);
    if (mon_event_fd < 0) mon_event_fd = open(event_path, O_WRONLY);
    if (mon_event_fd < 0) RETFAIL;

    /* Ack FIFO: read end — blocking open (controller opens write end after) */
    mon_ack_fd = open(ack_path, O_RDONLY);
    if (mon_ack_fd < 0) { close(mon_event_fd); mon_event_fd = -1; RETFAIL; }

    RETSTR(event_path, (int)strlen(event_path));
}

/*
 * MON_SEND(kind, body) → kind on GO, FAIL on STOP or error
 *
 * Writes event then BLOCKS waiting for 1-byte ack from controller.
 * 'G' → return success (participant continues)
 * 'S' → return FAIL   (participant should branch to END)
 */
lret_t MON_SEND(LA_ALIST) {
    char kind[64];
    char body[3900];
    char line[4096];
    char ack[1];

    if (mon_event_fd < 0) RETNULL;  /* not open — silent no-op */

    if (copy_str_arg(0, kind, sizeof(kind), args) < 0) RETFAIL;
    if (copy_str_arg(1, body, sizeof(body), args) < 0) RETFAIL;

    int n = snprintf(line, sizeof(line), "%s %s\n", kind, body);
    if (n <= 0 || n >= (int)sizeof(line)) RETFAIL;

    /* Step 1: write event */
    ssize_t written = write(mon_event_fd, line, (size_t)n);
    if (written != (ssize_t)n) RETFAIL;

    /* Step 2: block waiting for ack */
    if (mon_ack_fd < 0) RETFAIL;
    ssize_t r = read(mon_ack_fd, ack, 1);
    if (r != 1) RETFAIL;

    if (ack[0] == 'S') RETFAIL;   /* controller says STOP */

    int klen = (int)strlen(kind);
    RETSTR(kind, klen);
}

/*
 * MON_CLOSE() → ""
 */
lret_t MON_CLOSE(LA_ALIST) {
    if (mon_event_fd >= 0) { close(mon_event_fd); mon_event_fd = -1; }
    if (mon_ack_fd   >= 0) { close(mon_ack_fd);   mon_ack_fd   = -1; }
    RETNULL;
}
