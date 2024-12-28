sub  trim { my $s = shift; $s =~ s/^\s+|\s+$//g; return $s };

$dir = "*.pat";
@files = glob( $dir );
#$input_file = $ARGV[0];

foreach $input_file (@files ){
open INPUT,"<$input_file"  or die "can not open $input_file";
#print "Open $input_file";

my @array=();
while(<INPUT>){
	#print "$_";
push @array,$_;
}
close INPUT;

$input_file=~ s/\.pat//;
#$input_file=~ s/\-/\_/;
   if (length($input_file)> 32)
   {$input_file=substr($input_file,0,31);}

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

for($i=0;$i<@array;$i++)
{
 	if ($array[$i]=~ m/NOP/ ) 
 	{
 		
 		@line=split(/\s+/,$array[$i]);
 		@vector = split(/;/,$line[3]);
 		$vec=trim($vector[0]);
 		# for($m=0;$m<@line;$m++)
 		# {print "line[$m]= $line[$m], \n";}
 		# if (substr($vec,3,1) eq "L" ) {substr($vec,3,1,"X");}
 		printf  OUTPUT " ,NOP          , %s  , 1         , %s  #\n", $vec, trim($array[$i-1]);
 	}

 	if ($array[$i]=~ m/EXIT/ ) 
 	{
 		
 		@line=split(/\s+/,$array[$i]);
 		@vector = split(/;/,$line[3]);
 		$vec=trim($vector[0]);
 		# for($m=0;$m<@line;$m++)
 		# {print "line[$m]= $line[$m], \n";}
 		# if (substr($vec,3,1) eq "L" ) {substr($vec,3,1,"X");}
 		printf  OUTPUT " ,NOP          , %s  , 1         , %s  #\n", $vec, trim($array[$i-1]);
 	}
  	if ($array[$i]=~ m/IDXI/ ) 
 	{
 		
 		@line=split(/\s+/,$array[$i]);
 		@vector = split(/;/,$line[4]);
 		$vec=trim($vector[0]);
 		printf  OUTPUT " ,IDXI %-6d  , %s  , 1         , %s  #\n", $line[2],$vec, trim($array[$i-1]);

 	}


 }
	print  OUTPUT "# end of pattern\n";
	close  OUTPUT;

}


