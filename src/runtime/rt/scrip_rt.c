/*
 * scrip_rt.c — libscrip_rt.so skeleton (M-JITEM-X64 / EM-1)
 *
 * Authors: Lon Jones Cherryholmes · Claude Sonnet
 * Date: 2026-05-06
 *
 * EM-1: no-op skeleton. Both entries are present so that the literal-
 * zero asm emitted by sm_codegen_x64_emit links cleanly. Subsequent
 * rungs flesh out the runtime — first numeric/stack ops (EM-2/3),
 * then control flow (EM-4), then chunks (EM-5), then the pattern
 * matcher (EM-6).
 *
 * This file is compiled with -fPIC and linked into libscrip_rt.so.
 * Only the symbols declared in scrip_rt.h are part of the public ABI;
 * everything else inside the .so is implementation detail.
 */

#include "scrip_rt.h"

void scrip_rt_init(int argc, char **argv)
{
    /* EM-1: no-op. Receiver of argc/argv to keep the ABI stable
     * across rungs that need them (later: &INPUT, &OUTPUT, args). */
    (void)argc;
    (void)argv;
}

int scrip_rt_finalize(void)
{
    /* EM-1: program rc is always 0. Once SM_HALT codegen lands
     * (EM-2), this returns the halt code passed via scrip_rt_halt(). */
    return 0;
}
