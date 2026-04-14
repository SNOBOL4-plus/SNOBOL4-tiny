# rk_arrays.raku — RK-14: array operations
# Arrays stored as \x01-separated strings in normal DESCR_t slots.
# push/pop/elems/arr_get as builtins; @arr[$i] = val via arr_set builtin.

sub main() {
    my @nums = '';
    push(@nums, 1);
    push(@nums, 2);
    push(@nums, 3);
    say(elems(@nums));
    say(arr_get(@nums, 0));
    say(arr_get(@nums, 2));
    @nums[1] = 99;
    say(arr_get(@nums, 1));
    my $p = pop(@nums);
    say($p);
    say(elems(@nums));

    my @words = '';
    push(@words, 'hello');
    push(@words, 'world');
    say(arr_get(@words, 0));
    say(arr_get(@words, 1));
    say(elems(@words));
}
