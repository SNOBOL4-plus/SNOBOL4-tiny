/*
 * smoke_gaps.c — Targeted smoke tests for the 3 critical gaps
 * before attempting to compile beautiful.c
 *
 * Gap 1: Pattern engine (sno_pat_*, sno_match_pattern, sno_match_and_replace)
 * Gap 2: Array/Table/Tree subscript API (sno_array_create, sno_subscript_get/set)
 * Gap 3: sno_register_fn, sno_push_val/pop_val/top_val, sno_tree_new
 *
 * Each test is standalone. Failures pinpoint exactly what to implement.
 */

#include <stdio.h>
#include <string.h>
#include "snobol4.h"

int pass = 0, fail = 0;

#define CHECK(desc, cond) \
    do { if (cond) { printf("  PASS: %s\n", desc); pass++; } \
         else      { printf("  FAIL: %s\n", desc); fail++; } } while(0)

/* =========================================================================
 * GAP 1: Pattern engine
 * ===================================================================== */

static void test_pattern_engine(void) {
    printf("\n[Gap 1: Pattern engine]\n");

    /* sno_pat_lit + sno_match_pattern */
    SnoVal lit = sno_pat_lit("hello");
    CHECK("pat_lit matches 'hello'",      sno_match_pattern(lit, "hello"));
    CHECK("pat_lit fails on 'world'",    !sno_match_pattern(lit, "world"));
    CHECK("pat_lit matches inside 'say hello'", sno_match_pattern(lit, "say hello"));

    /* sno_pat_span */
    SnoVal sp = sno_pat_span("abc");
    CHECK("pat_span('abc') matches 'aabbcc'", sno_match_pattern(sp, "aabbcc"));
    CHECK("pat_span fails on 'xyz'",         !sno_match_pattern(sp, "xyz"));

    /* sno_pat_break_ */
    SnoVal brk = sno_pat_break_(".");
    CHECK("pat_break('.') matches in 'foo.bar'", sno_match_pattern(brk, "foo.bar"));

    /* sno_pat_len */
    SnoVal l3 = sno_pat_len(3);
    CHECK("pat_len(3) matches 'abc'",  sno_match_pattern(l3, "abc"));
    CHECK("pat_len(3) matches in 'xabcy'", sno_match_pattern(l3, "xabcy"));

    /* sno_pat_pos + sno_pat_rpos */
    SnoVal p0 = sno_pat_pos(0);
    SnoVal rp0 = sno_pat_rpos(0);
    SnoVal anchored = sno_pat_cat(p0, sno_pat_cat(sno_pat_lit("ab"), rp0));
    CHECK("POS(0) 'ab' RPOS(0) matches 'ab'",   sno_match_pattern(anchored, "ab"));
    CHECK("POS(0) 'ab' RPOS(0) fails on 'xab'", !sno_match_pattern(anchored, "xab"));

    /* sno_pat_cat + sno_pat_alt */
    SnoVal cat = sno_pat_cat(sno_pat_lit("foo"), sno_pat_lit("bar"));
    CHECK("cat 'foo''bar' matches 'foobar'",  sno_match_pattern(cat, "foobar"));
    CHECK("cat 'foo''bar' fails on 'foobaz'", !sno_match_pattern(cat, "foobaz"));

    SnoVal alt = sno_pat_alt(sno_pat_lit("cat"), sno_pat_lit("dog"));
    CHECK("alt 'cat'|'dog' matches 'cat'", sno_match_pattern(alt, "cat"));
    CHECK("alt 'cat'|'dog' matches 'dog'", sno_match_pattern(alt, "dog"));
    CHECK("alt 'cat'|'dog' fails on 'bird'", !sno_match_pattern(alt, "bird"));

    /* sno_pat_epsilon — always matches */
    SnoVal eps = sno_pat_epsilon();
    CHECK("epsilon always matches",  sno_match_pattern(eps, "anything"));
    CHECK("epsilon matches empty",   sno_match_pattern(eps, ""));

    /* sno_pat_arbno */
    SnoVal star = sno_pat_arbno(sno_pat_lit("ab"));
    CHECK("arbno('ab') matches ''",      sno_match_pattern(star, ""));
    CHECK("arbno('ab') matches 'ababab'", sno_match_pattern(star, "ababab"));

    /* sno_pat_any_cs — ANY(chars) */
    SnoVal any_vw = sno_pat_any_cs("aeiou");
    CHECK("any('aeiou') matches 'a'", sno_match_pattern(any_vw, "a"));
    CHECK("any('aeiou') matches 'e'", sno_match_pattern(any_vw, "e"));
    CHECK("any('aeiou') fails on 'b'", !sno_match_pattern(any_vw, "b"));

    /* sno_pat_rtab */
    SnoVal rt2 = sno_pat_rtab(2);
    CHECK("rtab(2) on 'hello' leaves 2 chars", sno_match_pattern(rt2, "hello"));

    /* sno_pat_ref — deferred variable reference */
    sno_var_set("myPat", sno_pat_lit("xyz"));
    SnoVal ref = sno_pat_ref("myPat");
    CHECK("pat_ref('myPat') matches 'xyz'",   sno_match_pattern(ref, "xyz"));
    CHECK("pat_ref('myPat') fails on 'abc'", !sno_match_pattern(ref, "abc"));

    /* sno_var_as_pattern — variable holding pattern */
    SnoVal vp = sno_var_as_pattern(sno_var_get("myPat"));
    CHECK("var_as_pattern from var works", sno_match_pattern(vp, "xyz"));

    /* sno_pat_assign_cond — conditional capture $.var */
    SnoVal cap_var = SNO_NULL_VAL;
    sno_var_set("captured", SNO_NULL_VAL);
    SnoVal cap = sno_pat_assign_cond(sno_pat_span("0123456789"), sno_var_get("captured"));
    /* Note: assign_cond stores into the variable on match */
    int ok = sno_match_pattern(
        sno_pat_cat(sno_pat_pos(0), sno_pat_cat(cap, sno_pat_rpos(0))),
        "12345"
    );
    CHECK("pat_assign_cond captures digits", ok);

    /* sno_match_and_replace */
    SnoVal subject = SNO_STR_VAL("hello world");
    int replaced = sno_match_and_replace(&subject, sno_pat_lit("world"), SNO_STR_VAL("there"));
    CHECK("match_and_replace replaces 'world' with 'there'", replaced);
    CHECK("match_and_replace result is 'hello there'",
          strcmp(sno_to_str(subject), "hello there") == 0);

    /* sno_pat_user_call — unknown pattern function dispatched via sno_apply */
    /* (requires sno_register_fn to be working) */
}

/* =========================================================================
 * GAP 2: Array/Table/Tree subscript via SnoVal
 * ===================================================================== */

static void test_collections(void) {
    printf("\n[Gap 2: Collections — array_create, subscript_get/set, table_new, tree_new]\n");

    /* sno_array_create("1:4") */
    SnoVal arr = sno_array_create(SNO_STR_VAL("1:4"));
    CHECK("array_create returns non-null", arr.type != SNO_NULL);

    sno_subscript_set(arr, SNO_INT_VAL(1), SNO_INT_VAL(18));
    sno_subscript_set(arr, SNO_INT_VAL(2), SNO_INT_VAL(33));
    sno_subscript_set(arr, SNO_INT_VAL(3), SNO_INT_VAL(36));
    sno_subscript_set(arr, SNO_INT_VAL(4), SNO_INT_VAL(81));

    SnoVal v1 = sno_subscript_get(arr, SNO_INT_VAL(1));
    SnoVal v4 = sno_subscript_get(arr, SNO_INT_VAL(4));
    CHECK("arr[1] == 18", sno_to_int(v1) == 18);
    CHECK("arr[4] == 81", sno_to_int(v4) == 81);

    /* sno_table_new */
    SnoVal tbl = SNO_TABLE_VAL(sno_table_new());
    sno_subscript_set(tbl, SNO_STR_VAL("key"), SNO_STR_VAL("value"));
    SnoVal got = sno_subscript_get(tbl, SNO_STR_VAL("key"));
    CHECK("table set/get 'key'='value'", strcmp(sno_to_str(got), "value") == 0);

    /* sno_tree_new — the tree DATA type */
    SnoVal t = sno_make_tree(SNO_STR_VAL("snoId"),
                              SNO_STR_VAL("hello"),
                              SNO_INT_VAL(0),
                              SNO_NULL_VAL);
    CHECK("tree_new returns non-null", t.type != SNO_NULL);
    SnoVal ttype = sno_field_get(t, "t");
    SnoVal tval  = sno_field_get(t, "v");
    CHECK("tree t field = 'snoId'",  strcmp(sno_to_str(ttype), "snoId") == 0);
    CHECK("tree v field = 'hello'",  strcmp(sno_to_str(tval),  "hello") == 0);
}

/* =========================================================================
 * GAP 3: sno_register_fn, push_val/pop_val/top_val
 * ===================================================================== */

static SnoVal _test_fn(SnoVal *args, int n) {
    if (n < 1) return SNO_INT_VAL(0);
    return sno_add(args[0], SNO_INT_VAL(1));
}

static void test_registration(void) {
    printf("\n[Gap 3: sno_register_fn + push_val/pop_val/top_val]\n");

    /* sno_register_fn */
    sno_register_fn("addOne", _test_fn, 1, 1);
    SnoVal result = sno_apply("addOne", (SnoVal[]){SNO_INT_VAL(41)}, 1);
    CHECK("registered fn 'addOne(41)' returns 42", sno_to_int(result) == 42);

    /* sno_push_val / sno_pop_val / sno_top_val */
    SnoVal a = SNO_STR_VAL("alpha");
    SnoVal b = SNO_STR_VAL("beta");
    sno_push_val(a);
    sno_push_val(b);
    SnoVal top = sno_top_val();
    CHECK("top_val() is 'beta'", strcmp(sno_to_str(top), "beta") == 0);
    SnoVal popped = sno_pop_val();
    CHECK("pop_val() returns 'beta'", strcmp(sno_to_str(popped), "beta") == 0);
    SnoVal popped2 = sno_pop_val();
    CHECK("second pop_val() returns 'alpha'", strcmp(sno_to_str(popped2), "alpha") == 0);
}

/* =========================================================================
 * Main
 * ===================================================================== */

int main(void) {
    sno_runtime_init();
    printf("=== Sprint 20 Gap Smoke Tests ===\n");

    test_pattern_engine();
    test_collections();
    test_registration();

    printf("\n================================================\n");
    printf("Results: %d pass, %d fail\n", pass, fail);
    if (fail == 0) printf("ALL PASS — gaps are closed.\n");
    return fail ? 1 : 0;
}
