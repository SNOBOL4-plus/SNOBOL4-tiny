# rk_stdio39.raku — RK-39: $*STDIN $*STDOUT $*STDERR handles

sub main() {
    # $*STDOUT is handle index 1 — print/say to it
    print($*STDOUT, "stdout ok\n");
    say($*STDOUT, 'say stdout ok');

    # $*STDERR is handle index 2
    print($*STDERR, "stderr ok\n");

    say('rk_stdio39 ok');
}
