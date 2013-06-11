#!/usr/bin/perl

use strict;


my $out = "# a square die in [-0.5 -0.5 0] [0.5 0.5 0] region\n";
$out .= "AttributeBegin\n";
$out .= "Material \"ic\" \"color Kd\" [.0 .0 .0] \"color R\" [.3 .3 .3] \"float d\" [1000] \"float h\" [5000] \"float deg\" [90]\n";
$out .= "Include \"geometry/unitsquare.pbrt\"\n";
$out .= "AttributeEnd\n\n";

my $d = 1000;
my $dMean = 1000;
my $dRange = 100;
my $r = 0;
my $z = 0;
my $xMax = 980;
my $yMax = 980;

srand(1234);

open (IN, $ARGV[0]) or error("Could not open $ARGV[0]");
while (my $line = <IN>) {
    if($line =~ /(\d+),(\d+)\s+(\d+),(\d+)/) {
	my $xScale = ($3 - $1) / $xMax;
	my $yScale = ($4 - $2) / $yMax;
	my $xOffset = (($3 + $1) / 2) / $xMax -0.5;
	my $yOffset = -((($4 + $2) / 2) / $yMax -0.5);
	if($xScale==0 || $yScale==0){
	    print "$1,$2 $3,$4 -> zero area\n"
	}

	$out .= "AttributeBegin\n";
	$out .= "Translate $xOffset $yOffset $z  \n";
	$out .= "Scale $xScale $yScale 1\n";
	$out .= "Include \"geometry/unitsquare.pbrt\"\n";
	$out .= "AttributeEnd\n\n";
    } elsif($line =~ /Mean:(\d+)\s+Range:(\d+)/){
	$dMean = $1;
	$dRange = $2;
    } else  {
	$z -= 0.00001;
	$d = $dMean - $dRange/2 + rand($dRange);
	$r = 0.3 + rand(0.4);
	$out .= "\n# New set of pattern, using new material\n";
	$out .= "Material \"ic\" \"color Kd\" [.0 .0 .0] \"color Ks\" [.3 .3 .3] \"color R\" [$r $r $r] \"float d\" [$d] \"float h\" [530] \"float deg\" [90]\n\n";
    }

}
close IN;

open (OUT, ">die.pbrt");
print OUT  $out;
close OUT;

