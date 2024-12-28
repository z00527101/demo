# NOTICE: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  
# IN NO EVENT SHALL ADVANTEST BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.

#!/bin/sh

temp=${1/.csv/.asc}

ext=${1##*.}
if [ "$ext" != 'csv' ]; then
  echo "Usage: $0 main.csv [sub] [opt]" 1>&2
  exit 1
fi

main=$1
fpath=${main%.*}
fname=${fpath##*/}
path=$(cd $(dirname $0) && pwd)
disk=$(cd $(dirname $1) && pwd)
stapc=$fname

shift
if [ "${1:0:1}" != '-' ]; then
  sub=$1
  shift
fi

work='/dev/shm'
dbm_compress_threshold=0
while getopts dzc: opt
do
  case $opt in
    'd') work=${disk};;
    'z') stapc='#0';;
    'c') if [ ${OPTARG} -lt 2 ]; then
           echo "$0: illegal option -- c ${OPTARG}"
           exit 1
         else
           dbm_compress_threshold=${OPTARG}
         fi;;
    '?') exit 1;;
  esac
done

if [ ${dbm_compress_threshold} -lt 2 ]; then
  echo "dbm_compress = OFF"
else
  echo "dbm_compress = ON (threshold=${dbm_compress_threshold})"
fi;

if [ -f $temp ];then
${path}/sub_preconv.pl ${work} ${main} ${sub} &&
${path}/sub_dbmconv ${work}/${fname}.pre ${fpath}.dbm ${dbm_compress_threshold} &&
${path}/sub_patcomp.pl ${fpath}.asc
ret=$?
else
${path}/sub_preconv.pl ${work} ${main} ${sub} &&
${path}/sub_dbmconv ${work}/${fname}.pre ${fpath}.dbm ${dbm_compress_threshold} &&
${path}/sub_patconv ${work}/${fname}.pre ${fpath}.asc ${fname} ${stapc} &&
${path}/sub_patcomp.pl ${fpath}.asc
ret=$?
fi

rm -f ${work}/${fname}.pre

if [ ${ret} -eq 0 ]; then
  echo --- finished ---
else
  echo \*\*\* ERROR!!! \*\*\*
  exit 1
fi

exit 0
