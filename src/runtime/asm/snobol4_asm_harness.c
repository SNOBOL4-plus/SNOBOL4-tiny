/*
 * snobol4_asm_harness.c — Sprint A9: thin C harness for ASM pattern bodies
 *
 * The body-only ASM output (sno2c -asm-body) emits:
 *   global root_alpha, root_beta
 *   extern cursor, subject_data, subject_len_val
 *   extern match_success, match_fail
 *
 * This harness:
 *   - Reads subject string from argv[1]
 *   - Exposes cursor, subject_data, subject_len_val as globals (the extern targets)
 *   - Provides match_success and match_fail as C labels reachable via jmp
 *   - Calls root_alpha via inline asm jmp
 *   - On match_success: prints subject[0..cursor] then newline, exits 0
 *   - On match_fail: exits 1
 *
 * Link: gcc -o prog body.o snobol4_asm_harness.o
 * Run:  ./prog "subject string"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

/* -----------------------------------------------------------------------
 * Globals shared with the ASM body
 * These must be plain globals — the ASM body references them as externs.
 *
 * subject_data is a flat array (not a pointer!) so the ASM body can use
 *   lea rsi, [rel subject_data]
 * identically whether in standalone or body-only mode.
 * ----------------------------------------------------------------------- */

uint64_t  cursor          = 0;   /* current match cursor */
uint64_t  subject_len_val = 0;   /* subject length */
char      subject_data[65536];   /* subject bytes — flat array, not a pointer */

/* Additional globals that some patterns may reference */
uint64_t  outer_cursor    = 0;
uint64_t  cap_len         = 0;

/* Capture buffer — enough for any reasonable subject */
char      cap_buf[65536];

/* -----------------------------------------------------------------------
 * Forward declarations for ASM-defined symbols
 * ----------------------------------------------------------------------- */
extern void root_alpha(void); /* ASM entry point — alpha port */

/* -----------------------------------------------------------------------
 * match_success / match_fail
 *
 * The ASM body ends with:   jmp match_success   or   jmp match_fail
 * These are C functions that never return (they call exit()).
 *
 * They must have C linkage and be reachable by the ASM linker.
 * ----------------------------------------------------------------------- */

void match_success(void) __attribute__((noreturn));
void match_fail(void)    __attribute__((noreturn));

void match_success(void) {
    /*
     * Two output modes:
     *  1. If cap_len > 0: a $ capture wrote into cap_buf — print cap_buf.
     *  2. Otherwise: print subject[0..cursor] (the matched span).
     */
    if (cap_len > 0) {
        write(STDOUT_FILENO, cap_buf, (size_t)cap_len);
    } else {
        /* Print matched portion: subject[0..cursor] */
        size_t n = (size_t)cursor;
        if (n > subject_len_val) n = (size_t)subject_len_val;
        write(STDOUT_FILENO, subject_data, n);
    }
    /* Newline */
    write(STDOUT_FILENO, "\n", 1);
    exit(0);
}

void match_fail(void) {
    exit(1);
}

/* -----------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <subject>\n", argv[0]);
        return 1;
    }

    /* Initialise runtime state */
    size_t slen = strlen(argv[1]);
    if (slen >= sizeof(subject_data) - 1) slen = sizeof(subject_data) - 1;
    memcpy(subject_data, argv[1], slen);
    subject_data[slen] = '\0';
    subject_len_val = (uint64_t)slen;
    cursor          = 0;
    outer_cursor    = 0;
    cap_len         = 0;
    memset(cap_buf, 0, sizeof cap_buf);

    /* Jump into root_alpha — the ASM body pattern entry point.
     * We use a computed goto via inline asm to avoid the C ABI overhead
     * and so the ASM body can jmp directly to match_success/match_fail.
     * The ASM body is compiled to not expect any call/ret convention —
     * it is pure jmp-based Byrd box code.
     */
    __asm__ volatile (
        "jmp root_alpha\n\t"
        :
        :
        : "memory"
    );

    /* Unreachable — the asm jmp above either reaches match_success or
     * match_fail, both of which call exit(). */
    __builtin_unreachable();
}
