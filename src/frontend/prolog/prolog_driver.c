#include "prolog_driver.h"
#include "prolog_parse.h"
#include "prolog_lower.h"
#include <stdio.h>
#include <stdlib.h>

Program *prolog_compile(const char *source, const char *filename)
{
    if (!filename) filename = "<stdin>";
    /* Case policy is a frontend concern (cf. commit 8aa5803b for DATATYPE).
     * Prolog preserves identifier spelling (atom names are case-significant);
     * tell the shared runtime to stop folding at name-ingest sites. No user
     * --case-sensitive flag required for .pl files. */
    sno_set_case_sensitive(1);
    PlProgram *pl = prolog_parse(source, filename);
    if (!pl) { fprintf(stderr, "prolog_compile: parse failed for %s\n", filename); return NULL; }
    return prolog_lower(pl);
}
