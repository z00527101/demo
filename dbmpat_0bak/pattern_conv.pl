sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };


$input_file = $ARGV[0];

open INPUT,"<$input_file"  or die "can not open $input_file";
#print "Open $input_file";

my @array=();
while(<INPUT>){
	#print "$_";
push @array,$_;
}
close INPUT;

$input_file=~ s/\.txt//;
$input_file=~ s/\-/\_/;
$output_file = uc($input_file)."\.csv";

if($output_file) {
print "matched!!";
}
else
{
 die "Wrong file was openned!!"
}

open OUTPUT,">$output_file"  or die "can not open $output_file";
print "Open $output_file\n";

print  OUTPUT "PatternSection:\n";
print  OUTPUT "Pinlist: CS, TCK, TDIO\n";
$cycle=0;
for($i=0;$i<@array;$i++)
{
 	if ($array[$i]=~ m/SET/ ) 
 	{
 		$cycle++;
 		@line=split(/\s+/,$array[$i]);
 		@vector = split(/;/,$line[3]);
 		$vec=trim($vector[0]);

 		if (substr($vec,3,1) eq "L" ) {substr($vec,3,1,"X");}
 		printf  OUTPUT " ,NOP          , %s  , 1         , FL_CYCLE=%d_%d  #\n", $vec,$i+1, $cycle;
 	}

 

 }
	print  OUTPUT "# end of pattern\n";
	close  OUTPUT;




