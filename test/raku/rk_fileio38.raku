# rk_fileio38.raku — RK-38/RK-56: open/close/slurp/lines + spurt

sub main() {
    my $path = '/tmp/rk38_test.txt';

    # RK-56: spurt writes file
    spurt($path, "line one\nline two\nline three\n");

    # slurp by path
    my $content = slurp($path);
    say($content);

    # open handle + slurp + close
    my $fh = open($path, ':r');
    my $c2 = slurp($fh);
    close($fh);
    say('slurp fh ok');

    # lines by path
    for lines($path) -> $line {
        say($line);
    }

    say('rk_fileio38 ok');
}
