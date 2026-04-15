# rk_map_grep_sort24.raku — RK-24: map/grep/sort list ops
sub main() {
    my @nums = '';
    push(@nums, 1); push(@nums, 2); push(@nums, 3);
    push(@nums, 4); push(@nums, 5);

    # map: double each element
    my @doubled = map { $_ * 2 } @nums;
    for @doubled -> $x { say($x); }

    # grep: keep evens
    my @evens = grep { $_ % 2 == 0 } @nums;
    for @evens -> $x { say($x); }

    # sort: lexicographic strings
    my @words = '';
    push(@words, 'banana'); push(@words, 'apple'); push(@words, 'cherry');
    my @sorted = sort @words;
    for @sorted -> $w { say($w); }

    # sort: numeric
    my @unsorted = '';
    push(@unsorted, 3); push(@unsorted, 1); push(@unsorted, 4);
    push(@unsorted, 1); push(@unsorted, 5); push(@unsorted, 9);
    push(@unsorted, 2); push(@unsorted, 6);
    my @nsorted = sort @unsorted;
    for @nsorted -> $n { say($n); }
}
