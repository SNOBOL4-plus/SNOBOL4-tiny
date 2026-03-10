/*
 * smoke_deferred.c — smoke test: deferred pattern refs (*name)
 *
 * In beautiful.sno: *snoParse, *snoExpr, *snoCommand etc.
 * These are stored in the variable table as patterns, then
 * referenced via sno_pat_ref("snoParse") at match time.
 *
 * This test:
 *   1. Stores a pattern in a variable
 *   2. Matches using a deferred ref to that variable
 *   3. Verifies the match succeeds
 */
#include <stdio.h>
#include "snobol4.h"

int main(void) {
    sno_runtime_init();
    int pass = 0, fail = 0;

#define CHECK(desc, cond) \
    do { if (cond) { printf("  PASS: %s\n", desc); pass++; } \
         else      { printf("  FAIL: %s\n", desc); fail++; } } while(0)

    /* Store SPAN('abc') as a pattern in variable "myPat" */
    SnoVal span_pat = sno_pat_span("abc");
    sno_var_set("myPat", span_pat);

    /* Match via deferred ref *myPat */
    SnoVal ref_pat = sno_pat_ref("myPat");
    CHECK("deferred ref *myPat matches 'aabbcc'",
          sno_match_pattern(ref_pat, "aabbcc"));
    CHECK("deferred ref *myPat fails on 'xyz'",
          !sno_match_pattern(ref_pat, "xyz"));

    /* Nested: *outer = *inner, inner = LEN(3) */
    SnoVal len3 = sno_pat_len(3);
    sno_var_set("inner", len3);
    SnoVal outer_ref = sno_pat_ref("inner");
    sno_var_set("outer", outer_ref);
    SnoVal deref2 = sno_pat_ref("outer");
    CHECK("double deferred ref *outer → *inner → LEN(3) matches 'abc'",
          sno_match_pattern(deref2, "abc"));
    CHECK("double deferred ref fails on 'ab' (len 2)",
          !sno_match_pattern(deref2, "ab"));

    /* Cat of two deferred refs: *a *b */
    sno_var_set("a", sno_pat_span("0123456789"));
    sno_var_set("b", sno_pat_lit("x"));
    SnoVal cat = sno_pat_cat(sno_pat_ref("a"), sno_pat_ref("b"));
    CHECK("*a *b matches '123x'",   sno_match_pattern(cat, "123x"));
    CHECK("*a *b fails on '123y'", !sno_match_pattern(cat, "123y"));

    printf("\nDeferred ref smoke: %d pass, %d fail\n", pass, fail);
    return fail ? 1 : 0;
}
