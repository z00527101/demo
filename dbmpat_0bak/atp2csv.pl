#!/usr/bin/perl
use strict;
##############################################################
## READ ME
###### Support command/function
#	repeat	-> IDXI repeat_count - 1
#
##############################################################


opendir(DIR, '.');
my @atplist = grep(/\.atp$/, sort(readdir(DIR)));
close(DIR);
	
##############################################################

foreach my $atpfile (@atplist) {
	print "$atpfile:\n";
	
	# read atp pattern
	my $atp_text = &read_file($atpfile);
	my $pat_name = uc($atpfile);
	my @pins;
	my @cpins;
	my %waveform;
	my %pinindex;
	
	# remove comment
	#$atp_text =~ s/\{.*?\}//g;
	
	$pat_name =~ s/\.ATP$/.csv/;
	open(PAT, ">$pat_name")|| die "Can't open target file $pat_name!\n";
	
	my $vector_i=0;
	my $cycle_i=0;
	my $repeat_n;
	my $repeat_text;
	my $vector_text;
	
	#while($atp_text =~ s/\{\s*vector\s+(\d+)\s+cycle\s+([\d\-]+)\s*\}\s*(repeat\s+(\d+)\s+)?vector([^;]+);//)
	#while($atp_text =~ s/(repeat\s+(\d+)\s+)?vector([^;]+);//)
	while($atp_text =~ s/(\s*repeat\s+(\d+)\s+)?\s*>\s*[^01HLX]([^;]+);//)
	{
		$repeat_text = $1;
		$repeat_n = $2;
		$vector_text = $3;
		
		# repeat
		if($repeat_text)
		{
			if($repeat_n>1)
			{
				printf PAT ",\tIDXI %-8i\t\t", $repeat_n - 1;
			}
			elsif($repeat_n == 1)
			{
				printf PAT ",\tNOP           \t\t";
			}
			else
			{
				printf PAT "********* repeat error: %s", $repeat_text;
			}
				
		}
		else
		{
			$repeat_n=1;
			printf PAT ",\tNOP           \t\t";
		}
		
		$vector_text =~ s/\s+//g;
		if($vector_text =~ /([01HLX]+)/ )
		{
			$vector_text = $1;
			printf PAT ",\t%s\t\t,\t2\t\t,\t%s,%s\n", $vector_text, $vector_i,$vector_i+$repeat_n-1;
		}
		else
		{
			printf PAT "********* vector error: %s\n", $vector_text;
		}
		$vector_i = $vector_i+$repeat_n;
	}
	
#	printf PAT ",\tSTPS\n";
	printf PAT ",\tSTPS, 111XXXX1 , 1 ,   44214,44214\n";
	
	close(PAT);
	#system "pause";
}


sub read_file()
{
	my $file = shift;
	open(FILE, $file)|| die "Can't open target file $file!\n";
	my @lines = <FILE>;
	close(FILE);
	return join("", @lines);
}
