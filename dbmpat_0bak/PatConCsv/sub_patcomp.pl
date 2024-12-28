#! /usr/bin/perl
# NOTICE: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL ADVANTEST BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

# 
# sub_patcomp.pl 
#    pattern compress program
# Version 5.10.1
# Usage: ./sub_patcomp.pl asc
#
use strict;

sub addjni(){
  my $patunit=shift;
  my $patunit_linecount=shift;

  my $patunit_jni='';
  my $addline1='';
  my $addline2='';
  if($patunit=~/\nIDXI1\s+(\d+)\s+[^\n]+\n$/s){
    $patunit_jni=$`;
    $addline1=$&;
    $addline2=$&;
    if($1>0){
      my $idxi_count=$1-1;
      $addline1=~s/\nIDXI1\s+\d+/\nIDXI1 $idxi_count/s;
    }
    else{
      $addline1=~s/\nIDXI1\s+\d+/\nNOP\t/s;
    }
    $addline2=~s/\nIDXI1\s+\d+/JNIx .-$patunit_linecount/s;
  }
  else{
    if($patunit=~/\nNOP\s+[^\n]+\n$/s){
      $patunit_jni=$`;
      $addline1=$&;
      $patunit_linecount--;
      $addline1=~s/\nNOP/\nJNIx .-$patunit_linecount/s;
    }
  }
  $patunit_jni.=$addline1.$addline2;
  $patunit_linecount++;

  return ($patunit_jni, $patunit_linecount);
}

#------------------------------------------------------------------------------
#	main
#------------------------------------------------------------------------------
if($ARGV[0] eq ''){
	print "Usage: $0 asc\n";
	exit(1);
}
$|=1;
print "compress pat\t...";
$|=0;

# file read
my $fh;
open $fh, $ARGV[0];
my $pat_text=do{local $/; <$fh>};
close($fh);

# pattern step count
$pat_text=~/START.*STPS/s;
my $patstep=$&;
my $patstep_count=0;
$patstep_count = $patstep=~tr/\n/\n/;
if($patstep_count<=2048){
  print "skip\n";
  exit(0);
}

# pattern analysis
my @pat_struct=();
my %loopcount_lines=();
while($pat_text=~/((?:[^\n]+\n)+)\1+/s){
  push(@pat_struct, [1, $`]);
  $pat_text=$&.$';
  my $patunit=$1;

  if($patunit=~/^((?:[^\n]+\n)+?)\1+$/s){
    $patunit=$1;
  }

  $pat_text=~/^(?:$patunit)+/s;
  $pat_text=$';
  my $pat_block=$&;

  my $patblock_linecount=0;
  $patblock_linecount = $pat_block=~tr/\n/\n/;
  my $patunit_linecount=0;
  $patunit_linecount++ while($patunit=~m/\n/g);
  my $loopcount=$patblock_linecount/$patunit_linecount;

  (my $patunit_jni, my $patunit_jni_linecount)=&addjni($patunit, $patunit_linecount);

  push(@pat_struct, [$loopcount, $patunit, $patunit_jni]);

  $loopcount_lines{$loopcount}+=$patblock_linecount-$patunit_jni_linecount;
}
push(@pat_struct, [1, $pat_text]);

# make register
my $index=2;
my %loopcount_regidx=();
my $reg_text="REGISTER\n";
foreach my $loopcount(sort {$loopcount_lines{$b}<=>$loopcount_lines{$a}} keys %loopcount_lines){
  $loopcount_regidx{$loopcount}=$index;
  my $jnicount=$loopcount-2;
  $reg_text.="IDX$index=$jnicount\n";

  $index++;
  if($index>8){
    last;
  }
}
$pat_struct[0][1]=~s/(\nMODE DBMAUTOXFER\n)/$1$reg_text/s;

unlink($ARGV[0]);
#rename $ARGV[0], $ARGV[0].'.org';
open $fh, "> $ARGV[0]";

# file output
foreach my $pat_block(@pat_struct){
  if(exists($loopcount_regidx{$$pat_block[0]})){
    $$pat_block[2]=~s/JNIx/JNI$loopcount_regidx{$$pat_block[0]}/;
    print $fh $$pat_block[2];
  }
  else{
    print $fh $$pat_block[1] x $$pat_block[0];
  }
}
close($fh);

print "done\n";

1;
