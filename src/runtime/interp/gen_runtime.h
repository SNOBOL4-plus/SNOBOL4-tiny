/*
 * coro_runtime.h — Icon interpreter runtime API
 *
 * FI-4: declarations for all symbols moved from scrip.c to coro_runtime.c.
 * Include this in scrip.c (and anywhere else that needs Icon runtime access).
 *
 * AUTHORS: Lon Jones Cherryholmes · Claude Sonnet 4.6 (FI-4, 2026-04-14)
 */
#ifndef CORO_RUNTIME_H
#define CORO_RUNTIME_H

#include "../../ir/ir.h"
#include "../../frontend/snobol4/scrip_cc.h"
#include "../../runtime/x86/bb_broker.h"
#include "../../frontend/icon/icon_gen.h"

/*------------------------------------------------------------------------
 * Constants
 *------------------------------------------------------------------------*/
#define FRAME_SLOT_MAX        64
#define PROC_TABLE_MAX       256
#define FRAME_DEPTH_MAX         16
#define FRAME_STACK_MAX      256
#define SCAN_STACK_MAX  16
#define GLOBAL_MAX      64

/*------------------------------------------------------------------------
 * Types
 *------------------------------------------------------------------------*/
typedef struct { const char *name; EXPR_t *proc; } IcnProcEntry;

typedef struct { EXPR_t *node; long cur; const char *sval; } IcnGenEntry_d;

/* IM-10: moved above IcnFrame so IcnFrame.sc can embed IcnScope by value */
typedef struct { const char *name; int slot; } IcnScopeEnt;
typedef struct { IcnScopeEnt e[FRAME_SLOT_MAX]; int n; } IcnScope;

typedef struct {
    DESCR_t       env[FRAME_SLOT_MAX];
    int           env_n;
    int           returning;
    DESCR_t       return_val;
    IcnGenEntry_d gen[FRAME_DEPTH_MAX];
    int           gen_depth;
    int           loop_break;
    int           loop_next;    /* 1 = `next` requested → skip body remainder, advance loop */
    EXPR_t       *body_root;
    /* IM-10: slot→name map, copied from the scope built in coro_call.
     * Allows sync_monitor to name local variables in snapshots. */
    IcnScope      sc;
    /* suspend coroutine state: set by E_SUSPEND, cleared by coro_drive */
    int           suspending;   /* 1 = procedure is suspending a value     */
    DESCR_t       suspend_val;  /* the value being suspended               */
    EXPR_t       *suspend_do;   /* do-clause to run on resumption, or NULL */
} IcnFrame;

/*------------------------------------------------------------------------
 * Globals (defined in coro_runtime.c)
 *------------------------------------------------------------------------*/
extern IcnProcEntry proc_table[PROC_TABLE_MAX];
extern int          proc_count;
extern int          g_lang;        /* 0=SNOBOL4 1=Icon */
extern EXPR_t      *g_icn_root;

extern IcnFrame     frame_stack[FRAME_STACK_MAX];
extern int          frame_depth;
#define FRAME (frame_stack[frame_depth - 1])

extern const char  *scan_subj;
extern coro_t *active_coro;
extern int          scan_pos;
typedef struct { const char *subj; int pos; } ScanEntry;
extern ScanEntry scan_stack[SCAN_STACK_MAX];
extern int          scan_depth;

extern const char  *global_names[GLOBAL_MAX];
extern int          global_count;

/*------------------------------------------------------------------------
 * Functions (defined in coro_runtime.c)
 *------------------------------------------------------------------------*/
void    frame_push(EXPR_t *n, long v, const char *sv);
void    frame_pop(void);
int     icn_frame_lookup(EXPR_t *n, long *out);
int     icn_frame_lookup_sv(EXPR_t *n, long *out, const char **sv);
int     frame_active(EXPR_t *n);

int     is_global(const char *name);
void    global_register(const char *name);

int     coro_drive(EXPR_t *e);
int     coro_drive_fnc(EXPR_t *e);   /* suspend-aware driver for user proc generators */

/* Set by coro_drive_fnc while running the every-body with a suspended value.
 * interp_eval(E_FNC) checks this: if the E_FNC node matches coro_drive_node,
 * return coro_drive_val directly instead of calling the procedure again. */
extern EXPR_t  *coro_drive_node;   /* the E_FNC node currently being driven */
extern DESCR_t  coro_drive_val;    /* the suspended value to return          */

int     scope_add(IcnScope *sc, const char *name);
int     scope_get(IcnScope *sc, const char *name);
void    icn_scope_patch(IcnScope *sc, EXPR_t *e);

DESCR_t coro_call(EXPR_t *proc, DESCR_t *args, int nargs);
bb_node_t coro_eval(EXPR_t *e);
int       is_suspendable(EXPR_t *e);
void      icn_init_save_frame(void);  /* IC-5: save initial-block statics before frame pop */

/* IC-8: real-to-string formatter (defined in driver/interp.c). Used by !N real iteration. */
const char *real_str(double r, char *buf, int bufsz);

/* IC-8: deep-identity test for Icon `===` (defined in coro_runtime.c). */
int icn_descr_identical(DESCR_t a, DESCR_t b);

/* IC-9 (session #26): Icon-keyword assign / probe (defined in driver/interp.c).
 * Used by coro_bb_revswap to perform atomic swap-with-revert on &pos / &subject.
 * kw_assign returns 1 on success, 0 on OOB-fail (and writes nothing on fail).
 * icn_kw_can_assign answers the same question without writing. */
int kw_assign(const char *kw, DESCR_t val);
int icn_kw_can_assign(const char *kw, DESCR_t val);

#endif /* CORO_RUNTIME_H */
