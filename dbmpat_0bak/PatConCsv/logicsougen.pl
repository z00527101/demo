#!/usr/bin/perl

opendir(TEMPDIR, "./"); 
my @dbmlist = grep /\.dbm$/, readdir TEMPDIR; 
for(0..(@dbmlist-1)) {$dbmlist[$_]=~s/\.dbm$//;}
closedir(TEMPDIR);

opendir(TEMPDIR, "./"); 
my @csvlist = grep /\.csv$/, readdir TEMPDIR; 
for(0..(@csvlist-1)) {$csvlist[$_]=~s/\.csv$//;}
closedir(TEMPDIR);

my %soulist1 = (); ++$soulist1{$_} foreach ( @dbmlist, @csvlist );
my @soulist2=keys(%soulist1);

open(SOURCE,">makefile.source" );
print SOURCE "CSV \= \\\n";
for(0..(@soulist2-1))
{
	if($_ eq (@soulist2-1)) {print SOURCE @soulist2[$_]."\.csv \n";}
        else                    {print SOURCE @soulist2[$_]."\.csv \\\n";}
}
close(SOURCE);

