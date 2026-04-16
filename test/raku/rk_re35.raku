# rk_re35.raku — RK-35: named captures <n> and $<n>

sub main() {
    my $s = 'John Smith, age 42';

    # single named group
    if ($s ~~ /<word>([A-Za-z]+)/) { say($<word>); } else { say('FAIL word'); }

    # two named groups
    if ($s ~~ /<first>([A-Za-z]+) <last>([A-Za-z]+)/) {
        say($<first>);
        say($<last>);
    } else { say('FAIL two named'); }

    # named digit group
    my $t = 'score: 99 points';
    if ($t ~~ /<num>([0-9]+)/) { say($<num>); } else { say('FAIL num'); }

    # mix: named + positional in same pattern
    if ($s ~~ /<fn>([A-Za-z]+) ([A-Za-z]+)/) {
        say($<fn>);
        say($1);
    } else { say('FAIL mix'); }

    say('rk_re35 ok');
}
