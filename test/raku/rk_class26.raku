# RK-26: basic OO — class / method / new / field access

class Point {
    has $.x;
    has $.y;
    method sum() { return $!x + $!y; }
    method scale($factor) { return ($!x + $!y) * $factor; }
}

my $p = Point.new(x => 3, y => 4);
say $p.x;
say $p.y;
say $p.sum();
say $p.scale(2);

class Dog {
    has $.name;
    has $.age;
    method greet() { return "Woof from " ~ $!name; }
}

my $d = Dog.new(name => "Rex", age => 5);
say $d.name;
say $d.greet();
