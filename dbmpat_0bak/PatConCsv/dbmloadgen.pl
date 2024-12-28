#!/usr/bin/perl

@PatList=();
open(CSV,"makefile.source" );
	while( my $data = <CSV> )
	{
		if($data=~m/^\s*(\w*)\.csv/){push(@PatList,$1);}
	}
close(CSV);

open(DBM,"> ../dbmlod.c" );
	print DBM "\#include \"common\.h\"\n";
	foreach(@PatList){print DBM "\#include \"dbmpat\/".$_."\.h\"\n";}
	print DBM "\nint DbmLoad\(\)\n\{\nint dbmap\=0\;\nif\(FirstRun\)\{dbm\_reset\(\)\;\}\n";
	foreach(@PatList)
	{
		print DBM "dbmap\=Setup_Dbm\(";
		my $nametemp="\"$_\.mpa\"";
		print DBM sprintf("%38s\,",$nametemp);
		print DBM sprintf("%32s\_dbm_bitmode\,",$_);
		print DBM sprintf("%32s\_dbm\_data\,",$_);
		print DBM sprintf("%32s\_dbm\_size\/sizeof\(unsigned int\)\,dbmap\)\;\n",$_);
	}
	print DBM "return 0\;\n\}\n";
close(DBM);
