#!/bin/bash                            
PWD=`pwd`                              
WORKPATH=$PWD                          

######################################
# Check if the futureSuite is runing #
###################################### 
UTU=`ps -e | grep utu | gawk '{print $1}'`
if [ -n "$UTU" ]; then
  echo "FutureSuite is running"
  fsproreset
  fsclear
else
  starttesim
fi


#######################################
## Load socket and location file 	 #
#######################################
fscd $PWD              
fsproset $PWD/T2_4F2G1_HAW_067T_2C_MST_V0P5
fsconf --socket-file $PWD/socket_067T_lab.soc
fsprostart&
