#include "prolog_driver.h"
#include "prolog_parse.h"
#include "prolog_lower.h"
#include <stdio.h>
#include <stdlib.h>

Program *prolog_compile(const char *source, const char *filename)
{
    if (!filename) filename = "<stdin>";
    PlProgram *pl = prolog_parse(source, filename);
    if (!pl) { fprintf(stderr, "prolog_compile: parse failed for %s\n", filename); return NULL; }
    return prolog_lower(pl);
}
