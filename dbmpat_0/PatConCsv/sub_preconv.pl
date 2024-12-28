#! /usr/bin/perl
# NOTICE: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL ADVANTEST BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

# 
# sub_preconv.pl 
#    pattern conversion program for pre
#
# Usage: ./sub_preconv.pl main [sub]
#
use strict;
my @bitmode_list = (0, 1, 3, 6, 12, 24, 48);
use constant N_TS_SIZE=>5;
use constant XN=>0;
use constant LABELLINE=>1;
use constant LABEL=>0;
use constant OP=>1;
use constant MAXBUFSIZE=>10000000;

my $main;
my $sub;
my $header;
my $temp;
my $pre;

my $bitsize;
my $line_number = 0;
my %idx_loop = ();
my %jni_loop = ();
my %jniline_xn_labelline = ();
my @jnilines = ();
my %label_line = ();

sub str2num{
	my $str = shift;
	my $num;

	if($str=~/^\d+$/){
		$num = $str;
	}
	else{
		$num = hex($str);
	}

	return $num;
}

sub get_loop_count{
	my $main_pattern_file = shift;
	my $target = shift;

	my $frh;
	open($frh, "< $main_pattern_file") or die("main_pattern file open error: $!");

	my %index_loop = ();
	while(my $line=<$frh>){
		$line=~s/^\s*([^#]*).*$/$1/;
		if($line=~/^[^,]*$target(\d+)\s*=\s*(0x[\da-fA-F]+|\d+)/){
			my $index = $1;
			my $value = $2;
			$index_loop{$index} = &str2num($value);
			#print "$target$index=$index_loop{$index}\n";
		}
		if($line=~/PatternSection/){
			last;
		}
	}
	close($frh);

	return %index_loop;
}

sub write_temp{
	my $fwh = shift;
	my $data = shift;

	foreach my $line(@$data){
		my ($lb,$op,$pat,$ts) = @$line;
		if(!defined($lb)){
			next;
		}
		#print "($lb,$op,$pat,$ts)\n";

		my $n = 1;
		#IDXI
		if($op=~/^IDXI\d* (0x[\da-fA-F]+|\d+)/){
			my $n_str = $1;
			$n = &str2num($n_str)+1;
		}
		#IDXn
		if($op=~/^IDX(\d+)/){
			my $index = $1;
			if(!exists($idx_loop{$index})){
				print "IDX${index} did not found.\n";
				exit(1);
			}
			$n = $idx_loop{$index}+1;
		}

		#write .tmp
		print $fwh pack('L',$n).pack('C',$ts).$pat;
		#print $fwh "$n,$ts,$pat\n";

		#make hash: label to line
		if($lb ne ''){
			if(exists($label_line{$lb})){
				print "${lb} is duplicated.\n";
				exit(1);
			}
			else{
				$label_line{$lb} = $line_number;
			}
		}
		#make hash: JNI line to xN,label line
		if($op=~/^JNI(\d+) (\w+)/){
			my $jni = $1;
			my $lb = $2;

			if(!exists($label_line{$lb})){
				print "${lb} did not found.\n";
				exit(1);
			}
			if(!exists($jni_loop{$jni})){
				print "IDXM${jni} did not found.\n";
				exit(1);
			}

			$jniline_xn_labelline{$line_number} = [$jni_loop{$jni}+1, $label_line{$lb}];
		}

		$line_number++;
	}

	return;
}

sub parse_line{
	my $line = shift;

	$$line=~s/^([^#]*)/$1/;
	$$line=~s/\x0d\x0a|\x0d|\x0a/\n/;
	$$line=~s/STPS/NOP/;
	$$line=~tr/\t / /s;
	$$line=~s/ ?, ?/,/g;

	my $lb;
	my $op;
	my $pat;
	my $ts;
	if($$line=~/^([^,]*),([^,]*),([01LHNPX ]*),(\d+)/){
		$lb = $1;
		$op = $2;
		$pat = $3;
		$ts = $4;
		$pat=~tr/ //d;
		$bitsize = length($pat);
	}
	my $record = [$lb,$op,$pat,$ts];

	return $record;
}

sub get_jsr_block{
	my $label = shift;
	#print "\tunfold JSR $label.\n";

	my $frh;
	open($frh, "< $sub") or die("sub_pattern file open error: $!");

	my @jsr_block = ();
	while(my $line = <$frh>){
		my $record = &parse_line(\$line);
		my $lb = $record->[LABEL];
		if($lb eq $label){
			$record->[LABEL] = ''; #delete label
			push(@jsr_block, $record);
			my $op;
			while(my $line2 = <$frh>){
				my $record2 = &parse_line(\$line2);
				push(@jsr_block, $record2);
				$op = $record2->[OP];
				if($op eq 'RTN'){
					last;
				}
			}
			if($op ne 'RTN'){
				print "JSR ${label} RTN did not found.\n";
				exit(1);
			}
			last;
		}
	}
	if($#jsr_block<0){
		print "JSR ${label} did not found.\n";
		exit(1);
	}
	#print @jsr_block;
	close($frh);

	return \@jsr_block;
}

sub change_format{
	%idx_loop = &get_loop_count($main, 'IDX');
	%jni_loop = &get_loop_count($main, 'IDXM');

	my $frh;
	my $fwh;
	open($frh, "< $main") or die("main file open error: $!");
	open($fwh, "> $temp") or die("temp file open error: $!");
	binmode $fwh;

	$line_number = 0;
	%jniline_xn_labelline = ();
	%label_line = ();
	my $record;
	while(my $line=<$frh>){
		$record = &parse_line(\$line);
		&write_temp($fwh, [$record]);

		#unfold JSR
		my $op = $record->[OP];
		if($op=~/^JSR (\w+)/){
			my $label = $1;
			my $jsr_block = &get_jsr_block($label);
			&write_temp($fwh, $jsr_block);
		}

	}
	close($frh);
	close($fwh);

	return;
}

sub get_max{
	my $keys = shift;
	my $ceil = shift;

	my $result;
	foreach my $val(@$keys){
		if($val<$ceil-1){
			$result = $val;
			last;
		}
	}

	return $result;
}

sub analy_jni{
	my $sta0 = shift;
	my $stp0 = shift;

	my @line_num = ($stp0);
	my @xn = (1);
	while((my $stp = &get_max(\@jnilines, $line_num[0]))>$sta0){
		my $xn_labelline = $jniline_xn_labelline{$stp};
		unshift(@line_num, $stp+1);
		unshift(@xn, $xn_labelline->[XN]);

		my $sta = $xn_labelline->[LABELLINE];
		if(exists($jniline_xn_labelline{$sta-1})){
			next;
		}
		else{
			unshift(@line_num, $sta);
			unshift(@xn, 1);
		}
	}

	my $ret = [\@line_num, \@xn];
	return $ret;
}

sub unfold_jni_sub{
	my $rdata = shift;
	my $sta0 = shift;
	my $stp0 = shift;

	my $retdata = '';
	if($stp0==$sta0){
		return \$retdata;
	}

	my $analy_result = &analy_jni($sta0, $stp0);
	my ($line_num, $xn) = @$analy_result;
	my $retref;
	if($#$line_num==0){
		$retref = $rdata;
	}
	else{
		my $sta = $sta0;
		foreach my $stp(@$line_num){
			my $n_loop = shift(@$xn);
			$retdata.=substr($$rdata,($sta-$sta0)*($bitsize+N_TS_SIZE),($stp-$sta0)*($bitsize+N_TS_SIZE)) x $n_loop;
			#$retdata.=${&unfold_jni_sub(\substr($$rdata,($sta-$sta0)*($bitsize+N_TS_SIZE),($stp-$sta0)*($bitsize+N_TS_SIZE)),$sta,$stp)} x $n_loop; # recursive call
			$sta = $stp;
		}
		$retref = \$retdata;
	}

	return $retref;
}

sub unfold_jni{
	my $frh;
	my $fwh;
	open($frh, "< $temp") or die("temp file open error: $!");
	open($fwh, "> $pre") or die("pre file open error: $!");
	binmode $frh;
	binmode $fwh;

	my $filesize = -s $temp;
	@jnilines = reverse(sort(keys(%jniline_xn_labelline)));

	my $analy_result = &analy_jni(0, $filesize/($bitsize+N_TS_SIZE));
	my ($line_num, $xn) = @$analy_result;
	my $sta = 0;
	foreach my $stp(@$line_num){
		my $n_loop = shift(@$xn);
		my $rdata;
		read($frh, $rdata, ($stp-$sta)*($bitsize+N_TS_SIZE));
		my $outdata = &unfold_jni_sub(\$rdata, $sta, $stp);
		if($n_loop>1){
			my $datasize = length($outdata);
			my $n_buf = int(sqrt($n_loop));
			if($datasize*$n_buf>MAXBUFSIZE){
				$n_buf = int(MAXBUFSIZE/$datasize);
			}

			my $outdata_block = $$outdata x $n_buf;
			while($n_loop>=$n_buf){
				print $fwh $outdata_block;
				$n_loop-=$n_buf;
			}
		}
		print $fwh $$outdata x $n_loop;
		$sta = $stp;
	}
	close($frh);
	close($fwh) or die("pre file write error: $!");

	return;
}

sub get_bitmode{
	my $bitsize = shift;

	# Bitmode scan
	my $bitmode = 0;
	foreach my $i(@bitmode_list){
		if($i>=$bitsize){
			$bitmode = $i;
			last;
		}
	}

	if($bitmode==0){
		print "Bitmode error.\n";
		exit(1);
	}

	return $bitmode;
}

sub make_header{
	my $frh;
	my $fwh;

	# bitmode
	my $bitmode = &get_bitmode($bitsize);

	my $filebase = $main;
	$filebase=~s/\.\w*$//;
	$filebase=~s/^.*\///;
	my $date = localtime;

	# header
	open($fwh, "> $header") or die("header file open error: $!");
	print $fwh "// This file has been automatically created by the script program patconv.sh on $date\n";
	print $fwh "const long ${filebase}_dbm_bitmode = $bitmode;\n";
	print $fwh "extern unsigned int _binary_${filebase}_dbm_start[];\n";
	print $fwh "unsigned int *${filebase}_dbm_data = _binary_${filebase}_dbm_start;\n";
	print $fwh "extern long _binary_${filebase}_dbm_size[];\n";
	print $fwh "const long ${filebase}_dbm_size = (unsigned int)_binary_${filebase}_dbm_size;\n";
	close($fwh);

	#my $filesize = -s "${filename}.dbm";
	#my $datasize = $filesize/32;
	#print "\tDBM data = $datasize blocks\n";

	return;
}

#------------------------------------------------------------------------------
#	main
#------------------------------------------------------------------------------
if($ARGV[1] eq ''){
	print "Usage: $0 workdir main.csv [sub]\n";
	exit(1);
}

my $work = $ARGV[0];
$main = $ARGV[1];
$sub = $ARGV[2];
print "main = $main\n";
print "sub  = $sub\n";

$header = $main;
$header=~s/(\w+)\.\w+/$1.h/;

my $fname = $main;
$fname=~s/^(.*\/|)(\w+)\.\w+$/$2/;
$temp = "$work/$fname.tmp";
$pre  = "$work/$fname.pre";

#make .tmp
print "change format\t...";
&change_format();
print "done\n";

#make .pre
print "unfold JNI\t...";
&unfold_jni();
print "done\n";

#make .h
print "make header\t...";
&make_header();
print "done\n";

unlink($temp);

1;
