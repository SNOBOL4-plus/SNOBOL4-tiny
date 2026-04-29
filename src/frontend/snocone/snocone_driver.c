/*
 * snocone_driver.c — Snocone frontend pipeline driver
 *
 * snocone_compile(source, filename) → Program*
 *
 * Delegates to snocone_control_compile() which runs the full pipeline:
 *   snocone_lex() → snocone_control lowering (if/while/for/procedure) →
 *   per-expression: snocone_parse() + snocone_lower() → Program*
 */

#include "snocone_driver.h"
#include "snocone_control.h"
#include <stdio.h>

Program *snocone_compile(const char *source, const char *filename)
{
    if (!filename) filename = "<stdin>";
    return snocone_control_compile(source, filename);
}
