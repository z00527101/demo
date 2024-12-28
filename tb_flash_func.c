#include "common.h"
#include "common_flash.h"
#include <math.h>

unsigned int DATA3[0xff][DDUTCNT][FLASH_NUM],DATA2[0xff][DDUTCNT][FLASH_NUM],DATA1[0xff][DDUTCNT][FLASH_NUM];
int FCM_READ_DATA_BITSIZE=DATA_BITSIZE;
int NVR_CFG_BYFLASH_CP1[DDUTCNT][NC_Addr_Num][FLASH_NUM]={0};
int temp10[DDUTCNT][FLASH_NUM]={0};
int temp11[DDUTCNT][FLASH_NUM]={0};
int temp12[DDUTCNT][FLASH_NUM]={0};
//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1:set up testblock
void tb_mbist_mux_setup(){
		FlashPatExec("patFlash.mpa", "MBIST_MUX_SETUP");
}
void tb_write_soc_reg(){
		//setRegDataIn(0,0,0xC);
		setRegDataIn(0,0,0x2C);
		//setRegDataIn(0x0,0x2C,0x0);
		FlashPatExec("patFlash.mpa", "WRITE_SOC_REG");
}
void tb_nvr_cfg_read_all_data(){
	FCM_READ_DATA_BITSIZE=72;
	UT_DUT sysdut;
	Read_NVRCFG(AREA_NC,NVR_CFG_ADDR_For_Customer_CP1,NVR_CFG_ADDR_For_Customer_CP1+NC_Addr_Num-1,0x1,0x0,DATA3,DATA2,DATA1); 
	START_FLASH_LOOP
		START_DUT_LOOP(UT_CDUT)
			UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			printf("\nSITE:%d\tSDUT:%d\tDUT:%d\tflash:%d\n", UTL_ReadSiteNumber(),sysdut,dut,g_flash_no);//for overflow pin
			for(int i=0;i<NC_Addr_Num;i++){
			printf("i= %02d,SITE[%2d]\tSDUT[%2d]\tDUT[%02d]\tFLASH[%02d], data = 0x%02X_0x%08X_%08X\n",i, UTL_ReadSiteNumber(),sysdut,dut,g_flash_no,DATA3[i][dut-1][g_flash_no-1],DATA2[i][dut-1][g_flash_no-1],DATA1[i][dut-1][g_flash_no-1]);	
		        NVR_CFG_BYFLASH_CP1[dut-1][i][g_flash_no-1]= DATA1[i][dut-1][g_flash_no-1] ;
	                printf("NVR_CFG_BYFLASH_CP1[dut%02d][addr0x%x][flash %02d]=0x%8x\n", dut,NVR_CFG_ADDR_For_Customer_CP1+i,g_flash_no,NVR_CFG_BYFLASH_CP1[dut-1][i][g_flash_no-1]);	
   			}
		        printf("\n");	
		END_DUT_LOOP
		        printf("\n");	
	END_FLASH_LOOP
	UTL_WaitTime(5 MS);
        //tb_nvr_cfg_program_cptrim_crc_reverify();	
}
#if 0
void Read_NVRCFG(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname)
{
	int ybits = 0, xbits=0;
	int transfer_bits = 13;
	int tempyaddr=FCM_READ_DATA_BITSIZE-1;
	
	int start_addr_pre=0;
	int end_addr_pre=endaddr - startaddr;
	int tempxaddr = start_addr_pre ^ end_addr_pre;
	while(tempyaddr!=0)
	{
		ybits++;
		tempyaddr = tempyaddr>>1;
	}

	while(tempxaddr!=0)
	{
		xbits++;
		tempxaddr=tempxaddr>>1;
	}
	ulConfigFcm("SO", xbits, ybits, 1<<transfer_bits);

	UT_DUT sysdut;	
	FILE* fp;
	char fname[100];
	time_t t;
	struct tm * lt;
	time (&t);
	lt = localtime (&t);

	unsigned int i,j,d,k,fn;
	unsigned int *fcm_data=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits) * (1<<ybits));
	unsigned int *d1=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d2=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d3=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//	unsigned int *d1[FLASH_NUM];
//	unsigned int *d2[FLASH_NUM];
//	unsigned int *d3[FLASH_NUM];
 START_FLASH_LOOP
	setRegValue(UT_REG_D2C, area<<8);//set for Y15-8
	setRegValue(UT_REG_D1C, startaddr|addr_mask);//set for X23-0
	setRegValue(UT_REG_D1B, stepaddr);
	setRegDataIn(0x0,0x0,0x0);

	RegHandle h = UTL_GetRegHandle();
	if((xbits+ybits) > transfer_bits){                                                                                                                                                                                                                  
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<(transfer_bits-ybits))/stepaddr - 2);  // bytes for each fcm transfer//error
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 0);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, (1<<(xbits+ybits-transfer_bits))-2);  // transfer count - 1
	}
	else{
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<xbits)/stepaddr - 2 );  // bytes for each fcm transfer
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 2);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, 0              );  // transfer count - 1
	}
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);

	ulPresetFcm();
//	FlashPatExec("patFlash.mpa", "FN_NORL_READ_FCM");
//	FlashPatExec("patFlash.mpa", "FN_NORL_READ_FCM");
	FlashPatStart("patFlash.mpa", patname);
	
	START_DUT_LOOP(UT_CDUT)
//	for(fn=0;fn<FLASH_NUM;fn++)
//	{
//		d1[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//		d2[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//		d3[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//	}
//	for(fn=0;fn<FLASH_NUM;fn++){
//	}

	
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, (1<<xbits)-1, 0, (1<<ybits)-1, dut);	
			memset(d1, 0, sizeof(unsigned int)*(1<<xbits));//bit31:0
			memset(d2, 0, sizeof(unsigned int)*(1<<xbits));//bit63:32
			memset(d3, 0, sizeof(unsigned int)*(1<<xbits));//bit71:64
			for(i=start_addr_pre,k=0;i<=end_addr_pre;i++,k++)
			{

				for(j=0;j<(FCM_READ_DATA_BITSIZE<32?FCM_READ_DATA_BITSIZE:32);j++)
				{
					//d1[g_flash_no-1][i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZ-1-j]))&1)<<j;
					//d1[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<j;
					d1[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<j;
				}
				for(j=32;j<(FCM_READ_DATA_BITSIZE<64?FCM_READ_DATA_BITSIZE:64);j++)
				{
					//d2[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-32);
					d2[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<(j-32);
				}
				for(j=64;j<(FCM_READ_DATA_BITSIZE<72?FCM_READ_DATA_BITSIZE:72);j++)
				{
					//d3[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-(1<<ybits));
					d3[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<(j-64);
				}
				DATA3[k][dut-1][g_flash_no-1]=d3[i];
				DATA2[k][dut-1][g_flash_no-1]=d2[i];
				DATA1[k][dut-1][g_flash_no-1]=d1[i];
		//		printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, 0, d3[i], d2[i], d1[i]);fflush(stdout);
		//		printf("DUT[%02d]\tFLASH[%02d] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut,g_flash_no, 0, d3[i], d2[i], d1[i]);
				fflush(stdout);
			}
	END_DUT_LOOP
END_FLASH_LOOP
	free(fcm_data);
	free(d1);
	free(d2);
	free(d3);
//	for(fn=0;fn<FLASH_NUM;fn++)
//	{
//	free(d1[fn]);
//	free(d2[fn]);
//	free(d3[fn]);
//	}
}
#endif

void ReadReg_Print()
{

	printf("\n");
/*	int platshift=8;//for 72bit -> 64bit right 8bit shift
//	int platshift=0;//for 72bit -> 64bit right 8bit shift
//	unsigned  long long  regdata[DDUTCNT][g_flash_no]={0};
	unsigned  long long  regdata[DDUTCNT][g_flash_no];
	 ReadReg_STB_BFAIL_REG(0);
	START_DUT_LOOP(UT_CDUT)
		START_FLASH_LOOP
		printf("DUT[%i] FLASH%d read address[0x%05X], data = 0x%02X_%08X_%08X", dut,g_flash_no, 0, temp3[dut-1][g_flash_no-1], temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);fflush(stdout);

		if((temp3[dut-1][g_flash_no-1]==0x0)&&((temp2[dut-1][g_flash_no-1]==0xC18))&&(temp1[dut-1][g_flash_no-1]|0xf00000==0x20f10000))SetFinalResultOnly(dut,PASS);
		else SetFinalResultOnly(dut,FAIL);


		regdata[dut-1][g_flash_no-1]+=temp2[dut-1][g_flash_no-1];
		regdata[dut-1][g_flash_no-1]<<=(32-platshift);
		regdata[dut-1][g_flash_no-1]+=(temp1[dut-1][g_flash_no-1]>>platshift);

	//	printf("0x%llx ",regdata[dut-1][g_flash_no-1]);//to print long long data
	if(regdata[dut-1][g_flash_no-1] & 0x8000)printf(" TMEN");
	if(!(regdata[dut-1][g_flash_no-1]&0x4000))printf(" PORb");
	if(!(regdata[dut-1][g_flash_no-1] & 0x2000))printf(" CEb");
	if(regdata[dut-1][g_flash_no-1] & 0x1000)printf(" DPD");
	if(regdata[dut-1][g_flash_no-1] & 0x800)printf(" RDEN");
	if(regdata[dut-1][g_flash_no-1] & 0x400)printf(" RECALL");
	if(regdata[dut-1][g_flash_no-1] & 0x200)printf(" VREAD");
	if(!(regdata[dut-1][g_flash_no-1] & 0x100))printf(" WEb");
	if(regdata[dut-1][g_flash_no-1] & 0x80)printf(" PROG");
	if(regdata[dut-1][g_flash_no-1] & 0x40)printf(" PROG2");
	if(regdata[dut-1][g_flash_no-1] & 0x20)printf(" PREPG");
	if(regdata[dut-1][g_flash_no-1] & 0x10)printf(" CHIP");
	if(regdata[dut-1][g_flash_no-1] & 0x8)printf(" BLK");
	if(regdata[dut-1][g_flash_no-1] & 0x4)printf(" ERASE");
	if(regdata[dut-1][g_flash_no-1] & 0x3)printf(" RETRY%x%x",regdata[dut-1][g_flash_no-1]&0x2,regdata[dut-1][g_flash_no-1]&0x1);
	if(regdata[dut-1][g_flash_no-1] & 0x20000000)printf(" BUSY");
	if(regdata[dut-1][g_flash_no-1] & 0x10000000)printf(" COUNT");
	if(regdata[dut-1][g_flash_no-1] & 0x8000000)printf(" BIST_SEL");
	if(regdata[dut-1][g_flash_no-1] & 0x4000000)printf(" CONFEN");
	if(regdata[dut-1][g_flash_no-1] & 0x2000000)printf(" LVCTL");
	if(regdata[dut-1][g_flash_no-1] & 0x1000000)printf(" Addr_Ctl");
	if(regdata[dut-1][g_flash_no-1] & 0x400000)printf(" NVR");
	if(regdata[dut-1][g_flash_no-1] & 0x200000)printf(" LCK_CFG");
	if(regdata[dut-1][g_flash_no-1] & 0x100000)printf(" NVR_CFG");
	if(regdata[dut-1][g_flash_no-1] & 0x30000)printf(" ADDRN%x%x",regdata[dut-1][g_flash_no-1]&0x20000,regdata[dut-1][g_flash_no-1]&0x10000);
	printf("\n");
		END_FLASH_LOOP

	END_DUT_LOOP

	START_DUT_LOOP(UT_CDUT)
		START_FLASH_LOOP
//		printf("DUT[%i] FAIL REG: 0x%02X_%08X_%08X\n", dut, temp3[dut-1][g_flash_no-1], temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);fflush(stdout);
		SetFinalResultOnly(dut,PASS);
		END_FLASH_LOOP
	END_DUT_LOOP
*/
//	printf("\n");
	printf("\n");
	//int platshift=8;//for 72bit -> 64bit right 8bit shift
	int platshift=0;//for 72bit -> 64bit right 8bit shift
//	unsigned  long long  regdata[DDUTCNT][g_flash_no];
	unsigned  long long  regdata;
	unsigned long long   testdata=0;
	 ReadReg_STB_BFAIL_REG(0);
	START_DUT_LOOP(UT_CDUT)
		START_FLASH_LOOP
//		printf("DUT[%i] FLASH%d read address[0x%05X], data = 0x%02X_%08X_%08X", dut,g_flash_no, 0, temp3[dut-1][g_flash_no-1], temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);
		regdata=0;//init
		fflush(stdout);//??
//		regdata=0;//init

		if((temp3[dut-1][g_flash_no-1]==0x0)&&((temp2[dut-1][g_flash_no-1]==0xC18))&&(temp1[dut-1][g_flash_no-1]|0xf00000==0x20f10000))
             SetFinalResultOnly(dut,PASS);
		else SetFinalResultOnly(dut,FAIL);


//		regdata[dut-1][g_flash_no-1]=temp2[dut-1][g_flash_no-1];
		regdata=temp2[dut-1][g_flash_no-1];
	//	printf("\nregdata 0x%llx",regdata[dut-1][g_flash_no]);
//		regdata[dut-1][g_flash_no-1]<<=(32-platshift);
		regdata<<=(32-platshift);
	//	printf(" regdata 0x%llx",regdata[dut-1][g_flash_no]);
//		regdata[dut-1][g_flash_no-1]+=(temp1[dut-1][g_flash_no-1]>>platshift);
		regdata+=(temp1[dut-1][g_flash_no-1]>>platshift);
//		printf(" regdata 0x%llx,testdata 0x%llx",regdata[dut-1][g_flash_no],testdata);

//		printf("\nDUT[%i] FLASH%d read address[0x%05X], data = 0x%02X_%08X_%08X", dut,g_flash_no, 0, temp3[dut-1][g_flash_no-1], temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);
	printf("[DUT%2d]%t", dut);
		printf("reg data :0x%llx ",regdata);//to print long long data
	if(regdata & 0x8000)printf(" TMEN");
	if(!(regdata & 0x4000))printf(" PORb");
	if(!(regdata & 0x2000))printf(" CEb");
	if(regdata & 0x1000)printf(" DPD");
	if(regdata & 0x800)printf(" RDEN");
	if(regdata & 0x400)printf(" RECALL");
	if(regdata & 0x200)printf(" VREAD");
	if(!(regdata & 0x100))printf(" WEb");
	if(regdata & 0x80)printf(" PROG");
	if(regdata & 0x40)printf(" PROG2");
	if(regdata & 0x20)printf(" PREPG");
	if(regdata & 0x10)printf(" CHIP");
	if(regdata & 0x8)printf(" BLK");
	if(regdata & 0x4)printf(" ERASE");
	if(regdata & 0x3)printf(" RETRY%x%x",regdata&0x2,regdata&0x1);
	if(regdata & 0x20000000)printf(" BUSY");
	if(regdata & 0x10000000)printf(" COUNT");
	if(regdata & 0x8000000)printf(" BIST_SEL");
	if(regdata & 0x4000000)printf(" CONFEN");
	if(regdata & 0x2000000)printf(" LVCTL");
	if(regdata & 0x1000000)printf(" Addr_Ctl");
	if(regdata & 0x400000)printf(" NVR");
	if(regdata & 0x200000)printf(" LCK_CFG");
	if(regdata & 0x100000)printf(" NVR_CFG");
	if(regdata & 0x30000)printf(" ADDRN%x%x",regdata&0x20000,regdata&0x10000);
	printf("\n");
		END_FLASH_LOOP

	END_DUT_LOOP

	START_DUT_LOOP(UT_CDUT)
		START_FLASH_LOOP
//		printf("DUT[%i] FAIL REG: 0x%02X_%08X_%08X\n", dut, temp3[dut-1][g_flash_no-1], temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);fflush(stdout);
		SetFinalResultOnly(dut,PASS);
		END_FLASH_LOOP
	END_DUT_LOOP





}


void tb_read_stb_reg(){
	START_FLASH_LOOP
		//setRegDataOut(0,0xC18,0x20210000);
		//setRegDataOut(0,0x3E70,0x10202100);
		setRegDataOut(0,0x3E70,0x18202100);
		//setRegDataOut(0,0x2C,0x18202100);
		FlashPatExec("patFlash.mpa", "READ_STB_REG");
	END_FLASH_LOOP
		ReadReg_STB_BFAIL_REG(0);
}
void tb_flash_ip_reset(){
		FlashPatExec("patFlash.mpa", "FLASH_IP_RESET");
}
void tb_chip_enable(){
		FlashPatExec("patFlash.mpa", "CHIP_ENABLE");
}
void tb_write_timing_reg(){
	if(OP_RATE==rate_20mhz){ 
		FlashPatExec("patFlash.mpa", "WRITE_TIMING_FRE20M"); 
	}else if(OP_RATE==rate_15mhz){ 
		FlashPatExec("patFlash.mpa", "WRITE_TIMING_FRE15M");
	}else if(OP_RATE==rate_10mhz){ 
		FlashPatExec("patFlash.mpa", "WRITE_TIMING_FRE10M");
	}else if(OP_RATE==rate_5mhz){ 
		FlashPatExec("patFlash.mpa", "WRITE_TIMING_FRE05M");
	}else{
		printf("wrong timing op rate\n");UTL_Stop();
	}
}
void ReadFlash_SaveFile(char* identifier, int area, int startaddr, int endaddr, int stepaddr)
{
	time_t t;
	struct tm * lt;
	time (&t);
	lt = localtime (&t);
	int xbits = 0, ybits = 0;
	int transfer_bits = 17;
	int tempxaddr=endaddr, tempyaddr=DATA_BITSIZE-1;
	printf("databitsize%d\n",DATA_BITSIZE);
	while(tempxaddr!=0)
	{
		xbits++;
		tempxaddr = tempxaddr>>1;
	}
	while(tempyaddr!=0)
	{
		ybits++;
		tempyaddr = tempyaddr>>1;
	}
//	ulConfigFcm("SO", xbits, ybits,262144);
	ulConfigFcm("SO", xbits, ybits, pow(2,transfer_bits));
	START_FLASH_LOOP	
		ulPresetFcm();

		setRegValue(UT_REG_D2C, area<<8);//set for Y15-8
		setRegValue(UT_REG_D1C, startaddr);//set for X23-0
		setRegValue(UT_REG_D1B, stepaddr);
		setRegDataIn(0x0,0x0,0x0);

		RegHandle h = UTL_GetRegHandle();
		if((xbits+ybits) > transfer_bits){                                                                                                                                                                                                                  
			printf("bit>%d, xbits=%i, ybits=%i \n",transfer_bits,xbits, ybits);
		    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<(transfer_bits-ybits)) - 2);  // bytes for each fcm transfer//error
		    UTL_SetRegUs (h, UT_REG_CFLG, 1, 0);
		    UTL_SetRegUs (h, UT_REG_IDX2, 1, (1<<(xbits+ybits-transfer_bits))-2);  // transfer count - 1
		}
		else{
			printf("bit<=%d, xbits=%i, ybits=%i \n",transfer_bits,xbits, ybits);
		    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<xbits) - 2 );  // bytes for each fcm transfer
		    UTL_SetRegUs (h, UT_REG_CFLG, 1, 2);
		    UTL_SetRegUs (h, UT_REG_IDX2, 1, 0              );  // transfer count - 1
		}
		UTL_SendReg  (h);
		UTL_DeleteHandle(h);
//		Pause2("before");
//
		FlashPatStart("patFlash.mpa", "FN_NORL_READ_FCM");
//		FlashPatStart("patFlash.mpa", "FN_SLOW_READ_FCM");
//FlashP	atExec("patFlash.mpa", "FN_NORL_READ_FCM");
//		Pause2("after");
		
		UT_DUT sysdut;	
		FILE* fp;
		char fname[100];
		
		unsigned int i,j,d,fn;
		unsigned int fcm_data[(1<<xbits) * (1<<ybits)];
		unsigned int d1[(1<<xbits)];
		unsigned int d2[(1<<xbits)];
		unsigned int d3[(1<<xbits)];
		int index=pow(2,ybits);
		START_DUT_LOOP(UT_CDUT)
			UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			memset(d1, 0, sizeof(d1));//bit31:0
			memset(d2, 0, sizeof(d2));//bit63:32
			memset(d3, 0, sizeof(d3));//bit71:64
			ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, (1<<xbits)-1, 0, (1<<ybits)-1, dut);	
			//	for(i=0;i<1<<xbits;i++)
			for(i=startaddr;i<=endaddr;i++)
			{
				for(j=0;j<(DATA_BITSIZE<32?DATA_BITSIZE:32);j++)
				{
						d1[i] |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<j;
				}
				for(j=32;j<(DATA_BITSIZE<64?DATA_BITSIZE:64);j++)
				{
						d2[i] |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<(j-32);
				}
				for(j=64;j<(DATA_BITSIZE<72?DATA_BITSIZE:72);j++)
				{
						d3[i] |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<(j-64);
				}
		//			printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, i, d3[i], d2[i], d1[i]);fflush(stdout);
			}
			// write readdata to file
			
			sprintf(fname,"/home/fsdiag/SMIC/data_detail/%s_%s_%sSYSDUT%i_DUT%i_%i_%i_%04d-%02d-%02d_%02d%02d%02d_%x_%x.csv", LotNumber, WaferId, identifier, sysdut, dut, g_die_X[dut-1], g_die_Y[dut-1], lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, startaddr, endaddr);
			//sprintf(fname,"./datalog/%s_%s_%sSYSDUT%i_DUT%i_%i_%i_%04d-%02d-%02d_%02d%02d%02d_%x_%x.csv", LotNumber, WaferId, identifier, sysdut, dut, g_die_X[dut-1], g_die_Y[dut-1], lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, startaddr, endaddr);
			//sprintf(fname,"/home/fsdiag/datalog/%s_%s_%sSYSDUT%i_DUT%i_%i_%i_%04d-%02d-%02d_%02d%02d%02d_%x_%x.csv", LotNumber, WaferId, identifier, sysdut, dut, g_die_X[dut-1], g_die_Y[dut-1], lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, startaddr, endaddr);
			if((fp=fopen(fname,"aw")) == NULL)
			{
				printf("An error happens while opening a file named \" %s \".\n",fname);
			}
			else
			{
				fprintf(fp,"g_flash_no, address,data[71:64],data[63:32],data[31:0]\n");
				int addr = 0;
			//	for(i=0;i<(1<<xbits);i++)
				for(i=startaddr;i<=endaddr;i++)
				{
					fprintf(fp,"%d,0x%05X,0x%02X,0x%08X,0x%08X\n",g_flash_no,(area<<24)|i, d3[i], d2[i], d1[i]);
				}
				fclose(fp);
				printf("DUT%d save file:%s\n", dut, fname);
			}
			
		END_DUT_LOOP
	END_FLASH_LOOP
}

void tb_read_timing_reg(){
	START_FLASH_LOOP
		if(OP_RATE==rate_20mhz){ 
			FlashPatExec("patFlash.mpa", "READ_TIMING_FRE20M"); 
		}else if(OP_RATE==rate_15mhz){ 
			FlashPatExec("patFlash.mpa", "READ_TIMING_FRE15M");
		}else if(OP_RATE==rate_10mhz){ 
			FlashPatExec("patFlash.mpa", "READ_TIMING_FRE10M");
		}else if(OP_RATE==rate_5mhz){ 
			FlashPatExec("patFlash.mpa", "READ_TIMING_FRE05M");
		}else{
			printf("wrong timing op rate\n");UTL_Stop();
		}
	END_FLASH_LOOP
}
void tb_physical_address(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");
}
void tb_logical_address(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_00");
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2:trim &measure  testblock
void tb_chip_erase_veg_trimming(){
//--	HV_VSIM_Triming_Binary(GOX_TestMode);
//--	HV_VSIM_Triming_Binary(ENDU_TestMode);
//--	if use GOX/ENDU and VEG trim, please put VEG trim in the end.
	HV_VSIM_Triming_Binary(VEG_TestMode);
//	HV_MVM_Triming_Binary(VEG_TestMode);
}
void tb_program_vcg_trimming(){ 
//--	HV_VSIM_Triming_Binary(COX_TestMode);
	HV_VSIM_Triming_Binary(VCG_TestMode);
}
void tb_program_vsl_trimming(){
	HV_MVM_Triming_Binary(VSL_TestMode);
}
void tb_deep_program_sector_erase(){

//-- special for 067T, please check and rewrite

		setRegAddr(AREA_M,DPROG_ADDR_1_FLASH12);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");

		setRegAddr(AREA_M,DPROG_ADDR_1_FLASH3);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");

}
void tb_deep_program_sector_read(){

//-- special for 067T, and it can improve yield rate by rewrite the bist done error pattern,please check and rewrite

		setRegAddr(AREA_M,DPROG_ADDR_1_FLASH12);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");

		setRegAddr(AREA_M,DPROG_ADDR_1_FLASH3);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");

}
void tb_deep_program_for_current_trim(){

//-- special for 067T, please check and rewrite
#if 0
	setRegAddr_D1D(AREA_M, DPROG_ADDR_2_FLASH12);
	setRegDataIn_TPHB(0x0,DPROG_DATA_2_H,DPROG_DATA_2_L);
	setRegAddr(AREA_M, DPROG_ADDR_1_FLASH12);
	setRegDataIn(0x0,DPROG_DATA_1_H,DPROG_DATA_1_L);
	FlashPatExec("patFlash.mpa", "RFCUR_TRIM_DPROG");
#endif
	setRegAddr_D1D(AREA_M, DPROG_ADDR_2_FLASH3);
	setRegDataIn_TPHB(0x0,DPROG_DATA_2_H,DPROG_DATA_2_L);
	setRegAddr(AREA_M, DPROG_ADDR_1_FLASH3);
	setRegDataIn(0x0,DPROG_DATA_1_H,DPROG_DATA_1_L);
	FlashPatExec("patFlash.mpa", "RFCUR_TRIM_DPROG");

}
void tb_normal_read_triming()
{
	Read_triming(NORM_TRIM_ADDR, NORM_TRIM_SHIFT, REF_trim, "NORM_RD_TRIM");
}
void tb_m1_read_triming(){
	Read_triming(MARG1_TRIM_ADDR, MARG1_TRIM_SHIFT, MRG1_trim, "MRG1_RD_TRIM");
}
void tb_m0_read_triming(){
	Read_triming(MARG0_TRIM_ADDR, MARG0_TRIM_SHIFT, MRG0_trim, "MRG0_RD_TRIM");
}
void tb_vread_triming(){
	Read_triming(VREAD_TRIM_ADDR, VREAD_TRIM_SHIFT, VREAD1_trim, "VREAD1_TRIM");
}
void tb_measure_veg_chip_erase(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_VSIM(VEG_TestMode);
}
void tb_measure_vcg_program(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_VSIM(VCG_TestMode);
}
void tb_measure_vsl_program(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_MVM(VSL_TestMode);
}
void tb_measure_veg_chip_erase_ht(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_VSIM(VEG_TestMode);
//printf("\n************test VSIM & MVM*****************\n");
//	measure_HV_MVM(VEG_TestMode);
}
void tb_measure_vcg_program_ht(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_VSIM(VCG_TestMode);
//printf("\n************test VSIM & MVM*****************\n");
//	measure_HV_MVM(VCG_TestMode);
}
void tb_measure_vsl_program_ht(){
	ulSetSettlingTime(3 MS,5 MS,3 MS);
	measure_HV_MVM(VSL_TestMode);
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 3:nvr_cfg program and read and reload 
void tb_nvr_cfg_sector_erase(){
		setRegAddr(AREA_NC,0x0);
		FlashPatExec("patFlash.mpa", "NVR_CFG_UNLOCK_SECT_ERAS");
}
void tb_nvr_cfg_lock_en_sector_erase(){
		setRegAddr(AREA_NC,0x0);
		FlashPatExec("patFlash.mpa", "NVR_CFG_LOCK_EN_SECT_ERAS");
}
void tb_nvr_cfg_read_ff(){
		setRegAddr(AREA_NC,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_SSLRD_NORL");
}
void tb_nvr_cfg_cycling_x10(){
		for(int i=0; i<10; i++){
				setRegAddr(AREA_NC,0x0);
				FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
				setRegAddr(AREA_NC,0x0);
				setRegDataIn(0x0,0x0,0x0);
				FlashPatExec("patFlash.mpa", "BI_SPROG_NORL");
		}
}
void tb_nvr_cfg_read_00(){
		setRegAddr(AREA_NC,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_SSLRD_NORL");
}
void tb_nvr_cfg_read_trim_01(){
#if 0
	ulConfigUbm("SIO_PINS", C28, C27);

        START_FLASH_LOOP
		setRegAddr(AREA_NC);//--set  add/IDX1 loop num  for  NVR_CFG_READ_UBM_UBM

		START_DUT_LOOP(UT_CDUT)
			ADD_LOC_ECC[dut-1]=0xff;
		        WAFERID_ECC[dut-1]=0xff;
			Ubm_Read_Push("SIO_PINS",0,-1,0, 0,0, dut);//--init push func
			//Ubm_Read_Push("SIO_PINS",ADD_LOC_XY_L	   ,0,ADD_LOC_ECC[dut-1] 	   , XYh32[dut-1], XYl32[dut-1], dut);
			//Ubm_Read_Push("SIO_PINS",ADD_WAFERID_L     ,0,WAFERID_ECC[dut-1]	   , idh32[dut-1], idl32[dut-1], dut);
			//Ubm_Write_Push("SIO_PINS",ADD_LOC_XY_L  ,1, ADD_LOC_ECC[dut-1], WAFERID_h32[dut-1],WAFERID_l32[dut-1],  dut);//--2 group data
			//Ubm_Write_Push("SIO_PINS",ADD_LOC_XY_L  ,1, ADD_LOC_ECC[dut-1], WAFERID_h32[dut-1],WAFERID_l32[dut-1],  dut);//--2 group data
			Ubm_Write_Push("SIO_PINS",ADD_WAFERID_L ,0, WAFERID_ECC[dut-1], LOTID_h32[dut-1],LOTID_l32[dut-1],  dut);//--2 group data
			Ubm_Write_Push("SIO_PINS",ADD_WAFERID_L ,0, WAFERID_ECC[dut-1], LOTID_h32[dut-1],LOTID_l32[dut-1],  dut);//--2 group data

//-- printf("\nDUT%d,flash%x :0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",dut,g_flash_no,CBD0[dut-1][g_flash_no-1],CBD1[dut-1][g_flash_no-1],CBD2[dut-1][g_flash_no-1],CBD3[dut-1][g_flash_no-1],CBD4[dut-1][g_flash_no-1],CBD5[dut-1][g_flash_no-1],CBD6[dut-1][g_flash_no-1],CBD7[dut-1][g_flash_no-1],XYl32[dut-1],XYh32[dut-1],idl32[dut-1],idh32[dut-1]);
		END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_READ_TRIM_40");
		FlashPatExec("patFlash.mpa", "NVR_CFG_READ_UBM_UBM");
             END_FLASH_LOOP  
#endif
#if 1
		ulConfigUbm("SO_PINS", C28, C27);
		START_FLASH_LOOP
				START_DUT_LOOP(UT_CDUT)

		ulWriteUbm("SO_PINS",0*READ_DATA_BITSIZE,1*READ_DATA_BITSIZE-1, 0xff, WAFERID_h32[dut-1], WAFERID_l32[dut-1], dut);
		ulWriteUbm("SO_PINS",1*READ_DATA_BITSIZE,2*READ_DATA_BITSIZE-1, 0xff, LOTID_h32[dut-1], LOTID_l32[dut-1], dut);
		//ulWriteUbm("SO_PINS",0*READ_DATA_BITSIZE,1*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD0[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",1*READ_DATA_BITSIZE,2*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD1[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",2*READ_DATA_BITSIZE,3*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD2[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",3*READ_DATA_BITSIZE,4*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD3[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",4*READ_DATA_BITSIZE,5*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD4[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",5*READ_DATA_BITSIZE,6*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD5[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",6*READ_DATA_BITSIZE,7*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD6[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",7*READ_DATA_BITSIZE,8*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD7[dut-1][g_flash_no-1], dut);
		//ulWriteUbm("SO_PINS",8*READ_DATA_BITSIZE,9*READ_DATA_BITSIZE-1, 0xf0|(XYh32[dut-1]>>28), (XYh32[dut-1]<<4)|0xf, XYl32[dut-1], dut);
		//ulWriteUbm("SO_PINS",9*READ_DATA_BITSIZE,10*READ_DATA_BITSIZE-1, 0xf0|(idh32[dut-1]>>28),(idh32[dut-1]<<4)|0xf, idl32[dut-1], dut);
//		ulWriteUbm("SO_PINS",10*READ_DATA_BITSIZE,11*READ_DATA_BITSIZE-1, 0xff,0xffffffff, 0x7654, dut);//07gd
		END_DUT_LOOP
				FlashPatExec("patFlash.mpa", "NVR_CFG_READ_TRIM_40");
		END_FLASH_LOOP
#endif
}

void tb_nvr_cfg_program_trim_01(){
		ulConfigUbm("SI_PINS", C28, C27);
		InitialCpDieId(XYh32, XYl32,idh32,idl32);

		START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
		CBD0[dut-1][g_flash_no-1]=0xffff7fc9;
		CBD1[dut-1][g_flash_no-1]=0xffffffe0|(VSL_trim_value[VSL_trim[dut-1][g_flash_no-1]]<<11)|VSL_trim_value[VSL_trim[dut-1][g_flash_no-1]];
		CBD2[dut-1][g_flash_no-1]=0xffff8000|(VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]]<<10)|(VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]]<<5)|VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]];
		CBD3[dut-1][g_flash_no-1]=0xffffc1e0|(REF_trim_value[REF_trim[dut-1][g_flash_no-1]]<<9)|VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]];
		CBD4[dut-1][g_flash_no-1]=0xffff0001|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<1)|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<6)|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<11);
		CBD5[dut-1][g_flash_no-1]=0xffffffe0|VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]];
		CBD6[dut-1][g_flash_no-1]=0xffff8000|REF_trim_value[MRG1_trim[dut-1][g_flash_no-1]]|(REF_trim_value[MRG0_trim[dut-1][g_flash_no-1]]<<10)|(REF_trim_value[VREAD1_trim[dut-1][g_flash_no-1]]<<5);
		CBD7[dut-1][g_flash_no-1]=0xffffffff;
		printf("\n");


		ulWriteUbm("SI_PINS",0*PROG_DATA_BITSIZE,1*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD0[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",1*PROG_DATA_BITSIZE,2*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD1[dut-1][g_flash_no-1], dut);//bit[0,4]
		ulWriteUbm("SI_PINS",2*PROG_DATA_BITSIZE,3*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD2[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",3*PROG_DATA_BITSIZE,4*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD3[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",4*PROG_DATA_BITSIZE,5*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD4[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",5*PROG_DATA_BITSIZE,6*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD5[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",6*PROG_DATA_BITSIZE,7*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD6[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",7*PROG_DATA_BITSIZE,8*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, CBD7[dut-1][g_flash_no-1], dut);
		ulWriteUbm("SI_PINS",8*PROG_DATA_BITSIZE,9*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, XYl32[dut-1], dut);
		ulWriteUbm("SI_PINS",9*PROG_DATA_BITSIZE,10*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, XYh32[dut-1], dut);
		ulWriteUbm("SI_PINS",10*PROG_DATA_BITSIZE,11*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, idl32[dut-1], dut);
		ulWriteUbm("SI_PINS",11*PROG_DATA_BITSIZE,12*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, idh32[dut-1], dut);
//		ulWriteUbm("SI_PINS",12*PROG_DATA_BITSIZE,13*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x7654, dut);//07gd

		END_DUT_LOOP
				FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_TRIM_40");

		END_FLASH_LOOP
}

void tb_nvr_cfg_program_trim()
{

	ulConfigUbm("SI", C28, C27);

START_FLASH_LOOP
	setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)

	 CBD0[dut-1][g_flash_no-1]=0xffff7fc9;
	 CBD1[dut-1][g_flash_no-1]=0xffffffe0|VSL_trim_value[VSL_trim[dut-1][g_flash_no-1]];
	 CBD2[dut-1][g_flash_no-1]=0xffff8000|(VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]]<<0)|(VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]]<<5)|(VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]]<<10);
	 CBD3[dut-1][g_flash_no-1]=0xffffc1e0|(REF_trim_value[REF_trim[dut-1][g_flash_no-1]]<<9)|VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]];
	 CBD4[dut-1][g_flash_no-1]=0xffff0001|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<1)|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<6)|(VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]]<<11);
	 CBD5[dut-1][g_flash_no-1]=0xffffffe0|VEG_trim_value[VEG_trim[dut-1][g_flash_no-1]];
	 CBD6[dut-1][g_flash_no-1]=0xffff8000|REF_trim_value[MRG1_trim[dut-1][g_flash_no-1]]|(REF_trim_value[MRG0_trim[dut-1][g_flash_no-1]]<<10)|(REF_trim_value[VREAD1_trim[dut-1][g_flash_no-1]]<<5);
	 CBD7[dut-1][g_flash_no-1]=0xffffffff;

	if(HV_INTERNAL)
	{
	 COX_CBD2[dut-1][g_flash_no-1]=0xffff8000|(VCG_trim_value[COX_trim[dut-1][g_flash_no-1]])|(VCG_trim_value[COX_trim[dut-1][g_flash_no-1]]<<5)|(VCG_trim_value[COX_trim[dut-1][g_flash_no-1]]<<10);
	 COX_CBD3[dut-1][g_flash_no-1]=0xffffc1e0|(REF_trim_value[REF_trim[dut-1][g_flash_no-1]]<<9)|VCG_trim_value[COX_trim[dut-1][g_flash_no-1]];
	 GOX_CBD4[dut-1][g_flash_no-1]=0xffff0001|(VEG_trim_value[GOX_trim[dut-1][g_flash_no-1]]<<1)|(VEG_trim_value[GOX_trim[dut-1][g_flash_no-1]]<<6)|(VEG_trim_value[GOX_trim[dut-1][g_flash_no-1]]<<11);
	 GOX_CBD5[dut-1][g_flash_no-1]=0xffffffe0|VEG_trim_value[GOX_trim[dut-1][g_flash_no-1]];
	 ENDU_CBD4[dut-1][g_flash_no-1]=0xffff0001|(VEG_trim_value[ENDU_trim[dut-1][g_flash_no-1]]<<1)|(VEG_trim_value[ENDU_trim[dut-1][g_flash_no-1]]<<6)|(VEG_trim_value[ENDU_trim[dut-1][g_flash_no-1]]<<11);
	 ENDU_CBD5[dut-1][g_flash_no-1]=0xffffffe0|VEG_trim_value[ENDU_trim[dut-1][g_flash_no-1]];
	}
	
		if(NC_ECC_FLAG)		//ECC data function in 72bit 
	{
		CBD0_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD0[dut-1][g_flash_no-1]);
		CBD1_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD1[dut-1][g_flash_no-1]);
		CBD2_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD2[dut-1][g_flash_no-1]);
		CBD3_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD3[dut-1][g_flash_no-1]);
		CBD4_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD4[dut-1][g_flash_no-1]);
		CBD5_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD5[dut-1][g_flash_no-1]);
		CBD6_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD6[dut-1][g_flash_no-1]);
		CBD7_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,CBD7[dut-1][g_flash_no-1]);
	}
		else
	{
		CBD0_ECC[dut-1][g_flash_no-1]=0xff;
		CBD1_ECC[dut-1][g_flash_no-1]=0xff;
		CBD2_ECC[dut-1][g_flash_no-1]=0xff;
		CBD3_ECC[dut-1][g_flash_no-1]=0xff;
		CBD4_ECC[dut-1][g_flash_no-1]=0xff;
		CBD5_ECC[dut-1][g_flash_no-1]=0xff;
		CBD6_ECC[dut-1][g_flash_no-1]=0xff;
		CBD7_ECC[dut-1][g_flash_no-1]=0xff;
	}

 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",0x807ff8      ,NC_ECC_FLAG, CBD0_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD0[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ff9      ,NC_ECC_FLAG, CBD1_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD1[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ffa      ,NC_ECC_FLAG, CBD2_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD2[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ffb      ,NC_ECC_FLAG, CBD3_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD3[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ffc      ,NC_ECC_FLAG, CBD4_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD4[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ffd      ,NC_ECC_FLAG, CBD5_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD5[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807ffe      ,NC_ECC_FLAG, CBD6_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD6[dut-1][g_flash_no-1],  dut);
 		Ubm_Write_Push("SI",0x807fff      ,NC_ECC_FLAG, CBD7_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD7[dut-1][g_flash_no-1],  dut);

 printf("\nCBD:\tDUT%d,flash%x :0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",dut,g_flash_no,CBD0[dut-1][g_flash_no-1],CBD1[dut-1][g_flash_no-1],CBD2[dut-1][g_flash_no-1],CBD3[dut-1][g_flash_no-1],CBD4[dut-1][g_flash_no-1],CBD5[dut-1][g_flash_no-1],CBD6[dut-1][g_flash_no-1],CBD7[dut-1][g_flash_no-1]);
 printf("\nCBD_ECC:\tDUT%d,flash%x :0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",dut,g_flash_no,CBD0_ECC[dut-1][g_flash_no-1],CBD1_ECC[dut-1][g_flash_no-1],CBD2_ECC[dut-1][g_flash_no-1],CBD3_ECC[dut-1][g_flash_no-1],CBD4_ECC[dut-1][g_flash_no-1],CBD5_ECC[dut-1][g_flash_no-1],CBD6_ECC[dut-1][g_flash_no-1],CBD7_ECC[dut-1][g_flash_no-1]);

		END_DUT_LOOP
//		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_TRIM_40_UBM");
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");
END_FLASH_LOOP

}
void tb_nvr_cfg_read_trim(){

		ulConfigUbm("SIO_PINS", C28, C27);

        START_FLASH_LOOP
		setRegAddr(AREA_NC);//--set  add/IDX1 loop num  for  NVR_CFG_READ_UBM_UBM

		START_DUT_LOOP(UT_CDUT)
			Ubm_Read_Push("SIO_PINS",0,-1,0, 0,0, dut);//--init push func
			Ubm_Read_Push("SIO_PINS",0x81fff8	   ,0,CBD0_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD0[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fff9	   ,0,CBD1_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD1[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fffa	   ,0,CBD2_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD2[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fffb	   ,0,CBD3_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD3[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fffc	   ,0,CBD4_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD4[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fffd	   ,0,CBD5_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD5[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81fffe	   ,0,CBD6_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD6[dut-1][g_flash_no-1], dut);
			Ubm_Read_Push("SIO_PINS",0x81ffff	   ,0,CBD7_ECC[dut-1][g_flash_no-1], 0xffffffff,CBD7[dut-1][g_flash_no-1], dut);
			//Ubm_Read_Push("SIO_PINS",ADD_LOC_XY_L	   ,0,ADD_LOC_ECC[dut-1] 	   , XYh32[dut-1], XYl32[dut-1], dut);
			//Ubm_Read_Push("SIO_PINS",ADD_WAFERID_L     ,0,WAFERID_ECC[dut-1]	   , idh32[dut-1], idl32[dut-1], dut);
			//Ubm_Write_Push("SIO_PINS",ADD_LOC_XY_L  ,0, ADD_LOC_ECC[dut-1], WAFERID_h32[dut-1],WAFERID_l32[dut-1],  dut);//--2 group data
			//Ubm_Write_Push("SIO_PINS",ADD_WAFERID_L ,0, WAFERID_ECC[dut-1], LOTID_h32[dut-1],LOTID_l32[dut-1],  dut);//--2 group data

//-- printf("\nDUT%d,flash%x :0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",dut,g_flash_no,CBD0[dut-1][g_flash_no-1],CBD1[dut-1][g_flash_no-1],CBD2[dut-1][g_flash_no-1],CBD3[dut-1][g_flash_no-1],CBD4[dut-1][g_flash_no-1],CBD5[dut-1][g_flash_no-1],CBD6[dut-1][g_flash_no-1],CBD7[dut-1][g_flash_no-1],XYl32[dut-1],XYh32[dut-1],idl32[dut-1],idh32[dut-1]);
		END_DUT_LOOP
//		FlashPatExec("patFlash.mpa", "NVR_CFG_READ_TRIM_40");
		FlashPatExec("patFlash.mpa", "NVR_CFG_READ_UBM_UBM");
             END_FLASH_LOOP 

      tb_nvr_cfg_read_trim_01();
      tb_nvr_cfg_read_Vcode(); 
}

void  ReadNCFlash(unsigned int addr)
{
	ReadFlash(AREA_NC,addr);
}
//void  ReadNCFlash_repair(unsigned int addr)
//{
//	ReadFlash_repair(AREA_NC,addr);
//}
#if 0
void  ReadFlash_repair(unsigned int AREA,unsigned int addr)
{
	int xbits=1;
	int ybits=7;
	ulConfigFcm("SO", xbits,ybits, 8192);
	START_FLASH_LOOP
	ulPresetFcm();
	setRegAddr(AREA, addr);
	setRegDataOut(0x0,0x0,0x0);
	FlashPatExec("patFlash.mpa", "FN_SLOW_READ_FCM_1");// SLOW READ FLASH
	UT_DUT sysdut;	
	
	unsigned int i,j,d,fn;
	unsigned int fcm_data[(1<<xbits) * (1<<ybits)];
	unsigned int d1,d2,d3;
	int index=pow(2,ybits);
	i=addr&0x1;

	START_DUT_LOOP(UT_CDUT)
		memset(&d1, 0, sizeof(d1));//bit31:0
		memset(&d2, 0, sizeof(d2));//bit63:32
		memset(&d3, 0, sizeof(d3));//bit71:64
		ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, (1<<xbits)-1, 0, (1<<ybits)-1, dut);	
			for(j=0;j<(DATA_BITSIZE<32?DATA_BITSIZE:32);j++)
			{
					d1 |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<j;
			}
			for(j=32;j<(DATA_BITSIZE<64?DATA_BITSIZE:64);j++)
			{
					d2 |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<(j-32);
			}	
			for(j=64;j<(DATA_BITSIZE<72?DATA_BITSIZE:72);j++)
			{
					d3 |= ((fcm_data[i*index+DATA_BITSIZE-1-j])&1)<<(j-64);
			}
		printf("DUT[%i] FLASH%d read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, g_flash_no, addr, d3, d2, d1);fflush(stdout);
		temp1[dut-1]=	d1;temp2[dut-1]=d2; temp3[dut-1]=d3;	
	temp1_flash[FLASH_INDEX]=d1; temp2_flash[FLASH_INDEX]=d2; temp3_flash[FLASH_INDEX]= d3;		
	temp10[dut-1][g_flash_no]=d1;		
	temp11[dut-1][g_flash_no]=d2;		
	temp12[dut-1][g_flash_no]=d3;		
	END_DUT_LOOP
	END_FLASH_LOOP

}
#endif
void tb_reload_all_trim_code_CP1(){
		FlashPatExec("patFlash.mpa", "RELOAD_ALL_TRIM_CODE");
}

void tb_reload_all_trim_code(){
		FlashPatExec("patFlash.mpa", "RELOAD_ALL_TRIM_CODE");
#if 1
		int i;
		for(i = 0x807FC0; i <= 0x807FC1; i++){
			ReadNCFlash(i);
	}
//			START_DUT_LOOP(UT_CDUT)
//			//ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, (VREF10_trim_value[i]&0xf)|0xffffbe70,dut);
//			ulWriteUbm("SI", 0 , 31, 0x0, 0x0, (VcodeTmp[dut-1]&0xf)|0xffffbe70,dut);
//          		END_DUT_LOOP
//			FlashPatExec("patFlash.mpa", "WRITE_SOC_REG_UBM");
#endif



#if 1
	//	int i;
		for(i = 0x807FE0; i <= 0x807FE3; i++){
		//for(i = 0x807FC0; i <= 0x807FE3; i++){
			ReadNCFlash(i);
	}
#if 1
		ulConfigUbm("SI_PINS", C28, C27);
			START_DUT_LOOP(UT_CDUT)
			//ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, (VREF10_trim_value[i]&0xf)|0xffffbe70,dut);
			ulWriteUbm("SI", 0 , 31, 0x0, 0x0, (VcodeTmp[dut-1]&0xf)|0xffffbe70,dut);
          		END_DUT_LOOP
			FlashPatExec("patFlash.mpa", "WRITE_SOC_REG_UBM");
#endif
#endif
}
void tb_nvr_cfg_program_cp1_id(){
		setRegMultiAddr(AREA_NC, CP1_ID_ADDR, CP1_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xFF,0xFFFFFFFF,CP1_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
#if 0
//-- special for 067T, need to rewrite for more universal

		CP1_ID_DATA_ECC=CalculateECC(0xffffffff,CP1_ID_DATA);
/*
		setRegMultiAddr(AREA_NC, CP1_ID_ADDR, CP1_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xFFFFFFFF,CP1_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
		setRegMultiAddr(AREA_NC, CP1_ID_ADDR^0xC00000, (CP1_ID_ADDR^0xC00000)+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
	//	setRegDataIn(0xff,CP1_ID_DATA_ECC,0xffffffff);
		setRegDataIn(0xff,CP1_ID_DATA_ECC>>4,0xfffffff|(CP1_ID_DATA_ECC<<28));
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
*/
		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW
		START_DUT_LOOP(UT_CDUT)
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",CP1_ID_ADDR   ,NC_ECC_FLAG ,CP1_ID_DATA_ECC,  0xffffffff,CP1_ID_DATA,  dut);
		END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");
#endif
}
void tb_nvr_cfg_program_cp2_id(){
#if 0
		setRegMultiAddr(AREA_NC, CP2_ID_ADDR, CP2_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0xFFFFFFFF,CP2_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_ID_ADDR);
		setRegDataOut(0x0,0xffffffff, CP2_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_H32");
	END_FLASH_LOOP
#endif
#if 1
//-- special for 067T, need to rewrite for more universal

		//CP2_ID_DATA_ECC=CalculateECC(0xffffffff,CP2_ID_DATA);
		CP2_ID_DATA_ECC=0xff;

/*		setRegMultiAddr(AREA_NC, CP2_ID_ADDR, CP2_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xFFFFFFFF,CP2_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
		setRegMultiAddr(AREA_NC, CP2_ID_ADDR-0x040000, CP2_ID_ADDR-0x040000+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,CP2_ID_DATA_ECC,0xffffffff);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
*/
		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW
		START_DUT_LOOP(UT_CDUT)
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",CP2_ID_ADDR   ,NC_ECC_FLAG ,CP2_ID_DATA_ECC,  0xffffffff,CP2_ID_DATA,  dut);
		END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");
#endif
}
void tb_nvr_cfg_program_cp3_id(){

//-- special for 067T, need to rewrite for more universal

		//CP3_ID_DATA_ECC=CalculateECC(0xffffffff,CP3_ID_DATA);
		CP3_ID_DATA_ECC=0xff;

/*		setRegMultiAddr(AREA_NC, CP3_ID_ADDR, CP3_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xffffffff,CP3_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
		setRegMultiAddr(AREA_NC, CP3_ID_ADDR-0x040000, CP3_ID_ADDR-0x040000+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,CP3_ID_DATA_ECC,0xffffffff);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
*/
		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW
		START_DUT_LOOP(UT_CDUT)
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",CP3_ID_ADDR   ,NC_ECC_FLAG ,CP3_ID_DATA_ECC,  0xffffffff,CP3_ID_DATA,  dut);
		END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");


}
void tb_nvr_cfg_program_read_s2reteststamp(){
		setRegMultiAddr(AREA_NC, CP2_RETEST_ID_ADDR, CP2_RETEST_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0xFF,CP2_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");

	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_RETEST_ID_ADDR);
		setRegDataOut(0x0,0xFF, CP2_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L36");
	END_FLASH_LOOP

#if 0
//-- special for 067T, need to rewrite for more universal

		CP2_RETEST_ID_DATA_ECC=CalculateECC(0xffffffff,CP2_RETEST_ID_DATA);

/*		setRegMultiAddr(AREA_NC, CP2_RETEST_ID_ADDR, CP2_RETEST_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xffffffff,CP2_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
		setRegMultiAddr(AREA_NC, CP2_RETEST_ID_ADDR-0x040000, CP2_RETEST_ID_ADDR-0x040000+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,CP2_RETEST_ID_DATA_ECC,0xffffffff);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
*/
		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW
		START_DUT_LOOP(UT_CDUT)
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",CP2_RETEST_ID_ADDR   ,NC_ECC_FLAG ,CP2_RETEST_ID_DATA_ECC,  0xffffffff,CP2_RETEST_ID_DATA,  dut);
		END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");




//	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_RETEST_ID_ADDR);
		setRegDataOut(CP2_RETEST_ID_DATA_ECC,0xffffffff, CP2_RETEST_ID_DATA);
	FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//	END_FLASH_LOOP
#endif	
}
void tb_nvr_cfg_program_read_s3reteststamp(){
		setRegMultiAddr(AREA_NC, CP3_RETEST_ID_ADDR, CP3_RETEST_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xFF,0xFFFFFFFF,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP3_RETEST_ID_ADDR);
		setRegDataOut(0xff, 0xffffffff,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L36");
	END_FLASH_LOOP

#if 0
//-- special for 067T, need to rewrite for more universal

		CP3_RETEST_ID_DATA_ECC=CalculateECC(0xffffffff,CP3_RETEST_ID_DATA);
/*
		setRegMultiAddr(AREA_NC, CP3_RETEST_ID_ADDR, CP3_RETEST_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xffffffff,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
		setRegMultiAddr(AREA_NC, CP3_RETEST_ID_ADDR-0x040000, CP3_RETEST_ID_ADDR-0x040000+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0xff,CP3_RETEST_ID_DATA_ECC,0xffffffff);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
*/
		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW
		START_DUT_LOOP(UT_CDUT)
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",CP3_RETEST_ID_ADDR   ,NC_ECC_FLAG ,CP3_RETEST_ID_DATA_ECC,  0xffffffff,CP3_RETEST_ID_DATA,  dut);
		END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");

//	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP3_RETEST_ID_ADDR);
		setRegDataOut(CP3_RETEST_ID_DATA_ECC, 0xffffffff,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//	END_FLASH_LOOP
#endif
}
void tb_nvr_cfg_read_cp1_id(){
	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP1_ID_ADDR);
		setRegDataOut(0xff,0xFFFFFFFF, CP1_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L36");
	END_FLASH_LOOP
#if 0
//-- special for 067T, need to rewrite for more universal
//-- if CP ID ADDR different in flashes, add FLASH_LOOP and  g_flash_no judge  condition
		CP1_ID_DATA_ECC=CalculateECC(0xffffffff,CP1_ID_DATA);
//START_FLASH_LOOP
		setRegAddr(AREA_NC, CP1_ID_ADDR);
		setRegDataOut(CP1_ID_DATA_ECC, 0xffffffff,CP1_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//END_FLASH_LOOP
#endif
}
void tb_nvr_cfg_read_cp2_id(){
#if 0
		setRegMultiAddr(AREA_NC, CP2_ID_ADDR, CP2_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0xFFFFFFFF,CP2_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_ID_ADDR);
		setRegDataOut(0x0,0xffffffff, CP2_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_H32");
	END_FLASH_LOOP
#endif
#if 1
//-- special for 067T, need to rewrite for more universal

		//CP2_ID_DATA_ECC=CalculateECC(0xffffffff,CP2_ID_DATA);
		CP2_ID_DATA_ECC=0xff;

//		START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_ID_ADDR);
		setRegDataOut(CP2_ID_DATA_ECC,0xffffffff, CP2_ID_DATA);
			FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//		END_FLASH_LOOP
#endif
}
void tb_nvr_cfg_read_cp3_id(){

//-- special for 067T, need to rewrite for more universal

		//CP3_ID_DATA_ECC=CalculateECC(0xffffffff,CP3_ID_DATA);
		CP3_ID_DATA_ECC=0xff;

//	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP3_ID_ADDR);
		setRegDataOut(CP3_ID_DATA_ECC,0xffffffff, CP3_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//	END_FLASH_LOOP
}
void tb_nvr_cfg_read_cp2_retest_stamp(){

	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_RETEST_ID_ADDR);
		setRegDataOut(0xff,0xffffffff, CP2_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L36");
	END_FLASH_LOOP
#if 0
//-- special for 067T, need to rewrite for more universal

		CP2_RETEST_ID_DATA_ECC=CalculateECC(0xffffffff,CP2_RETEST_ID_DATA);

//START_FLASH_LOOP
		setRegAddr(AREA_NC, CP2_RETEST_ID_ADDR);
		setRegDataOut(CP2_RETEST_ID_DATA_ECC,0xffffffff, CP2_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//END_FLASH_LOOP
#endif
}
void tb_nvr_cfg_read_cp3_retest_stamp(){
	START_FLASH_LOOP
		setRegAddr(AREA_NC, CP3_RETEST_ID_ADDR);
		setRegDataOut(0xff, 0xffffffff,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L36");
	END_FLASH_LOOP
#if 0
//-- special for 067T, need to rewrite for more universal

		CP3_RETEST_ID_DATA_ECC=CalculateECC(0xffffffff,CP3_RETEST_ID_DATA);

//		START_FLASH_LOOP
		setRegAddr(AREA_NC, CP3_RETEST_ID_ADDR);
		setRegDataOut(CP3_RETEST_ID_DATA_ECC,0xffffffff,CP3_RETEST_ID_DATA);
		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID");
//		END_FLASH_LOOP
#endif
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 4:repair related


//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 5:stress &special
void tb_mrn_me_program_00(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);

		setRegMultiAddr(AREA_M_R_N,0x400000,0x400000+ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");
		setRegMultiAddr(AREA_M_R_N,0x800000,0x800000+ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
	
	
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");

		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_mrn_mo_program_00(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegMultiAddr(AREA_M_R_N,0x400080,0x400080+ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
	
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");
		setRegMultiAddr(AREA_M_R_N,0x800080,0x800080+ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
	
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");

}
void tb_mrn_gox_stress_hhi(){
	if(HV_INTERNAL==0)
	{
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",13 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N, 0x0);
		FlashPatExec("patFlash.mpa", "GOX_STRESS");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
		
	}
	else
	{
		HV_internal_set(GOX_TestMode);

		setRegAddr(AREA_M_R_N, 0x00);
		FlashPatExec("patFlash.mpa", "GOX_STRESS_INTERNAL");

		HV_internal_set(VEG_TestMode);
	}


}
void tb_main_2x8rows_program_00(){
		setRegMultiAddr(AREA_M,0x0,7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_RPROG_NORL");
		//setRegMultiAddr(AREA_M,0x1fc00,0x1fc00+7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegMultiAddr(AREA_M,0x3e00,0x3e00+7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_RPROG_NORL");
}
void tb_main_2x8rows_read_m0_00(){
		setRegMultiAddr(AREA_M,0x0,7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "READ_MARGIN0_2_8ROWS00");
		//setRegMultiAddr(AREA_M,0x1fc00,0x1fc00+7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegMultiAddr(AREA_M,0x3e00,0x3e00+7*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "READ_MARGIN0_2_8ROWS00");
}
void tb_mrn_me_1word_ff(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",4.6 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MEO_FN_NORL_PROG");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_mrn_mo_1word_ff(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",4.6 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N,ROW_SHIFT_ADDR);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MEO_FN_NORL_PROG");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_main_read_m0_ckbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		FlashPatExec("patFlash.mpa", "MARGIN0_CG3P5_MODE");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CSLRD_CKBD_ALL");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_rdn_read_m0_ckbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_SSLRD_CKBD");
		}
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_nvr_read_m0_ckbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CSLRD_CKBD_NVR");
		FlashPatExec("patFlash.mpa", "TMODE_EXIT");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_main_read_m0_ickbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		FlashPatExec("patFlash.mpa", "MARGIN0_CG3P5_MODE");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CSLRD_CKBD_ALL");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_rdn_read_m0_ickbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
		area=area<<i;
		setRegAddr(area,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_SSLRD_CKBD");
		}
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_nvr_read_m0_ickbd_cg3p5v(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CSLRD_CKBD_NVR");
		FlashPatExec("patFlash.mpa", "TMODE_EXIT");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_mrn_cox_stress(){
	if(HV_INTERNAL==0)
	{
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",12.3 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N, 0x00);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "COX_STRESS");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
	}
	else
	{
		HV_internal_set(COX_TestMode);

		setRegAddr(AREA_M_R_N, 0x00);
		FlashPatExec("patFlash.mpa", "COX_STRESS_INTERNAL");

		HV_internal_set(VCG_TestMode);
	}
}
void tb_cox2_stress(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",12.0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N, 0x00);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "COX2_STRESS");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void tb_mrn_sol_ox_stress(){
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "SOL_OX_STRESS");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
		
}
void tb_mrn_rdist_stress(){
		setVi(3.6 V, 0 V, 2);
		P("TM0", 	P_IN(2), 		P_FIXH);
//		P("EF0_TM0", 	P_IN(2), 		P_FIXH);
//		P("EF1_TM0", 	P_IN(2), 		P_FIXH);
		setRegAddr(AREA_M_R_N,0x0);
		FlashPatExec("patFlash.mpa", "RDIST_STRESS");
//		setVi(3.8 V, 0 V, 1);
		P("TM0", 	P_IN(1), 		P_FIXL);
	//	P("EF0_TM0", 	P_IN(1), 		P_FIXL);
	//	P("EF1_TM0", 	P_IN(1), 		P_FIXL);
}
void tb_mrn_endurance_screen(){
	if(HV_INTERNAL==0)
	{
		setHvUnMask("VPP1_PINS");
		setHvVSIM("VPP1_PINS",10.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M_R_N,0x0);
		FlashPatExec("patFlash.mpa", "FN_ENDURANCE_SCR"); 
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
	}
	else
	{
		HV_internal_set(ENDU_TestMode);

		setRegAddr(AREA_M_R_N, 0x00);
		FlashPatExec("patFlash.mpa", "FN_ENDURANCE_SCR_INTERNAL");

		HV_internal_set(VEG_TestMode);
	}

}
void tb_mrn_chip_erase_x50(){
		setRegAddr(AREA_M_R_N,0x0);
		setRegValue(UT_REG_IDX5, 50-2);
		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS_X50");
}
void tb_main_1st_row_column_program_00(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_RPROG_NORL");
		setRegMultiAddr(AREA_M,ROW_SHIFT_ADDR,(M_ROW_NUM-1)*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "FLASH40_FN_NORL_PROG_1ST_COL");
}
void tb_main_1st_row_column_read_00(){
		setRegAddr(AREA_M,0x0);
		setRegDataOut(0x0,0x0,0x0);
	//--	FlashPatExec("patFlash.mpa", "BI_RSLRD_NORL");
		FlashPatExec("patFlash.mpa", "BI_RREAD_NORL");
	START_FLASH_LOOP
        ConfigCFM_Sector();
		setRegMultiAddr(AREA_M,0+ROW_SHIFT_ADDR,(M_ROW_NUM-1)*ROW_SHIFT_ADDR,ROW_SHIFT_ADDR);
		setRegDataOut(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "FN_NORL_READ");
        ReadCFM_Sector();
	END_FLASH_LOOP
}
void tb_main_1sec_pre_program_00(){
		///--- setRegAddr(AREA_M_R_N,0x0);
		///--- FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_SPPRO_NORL");
}
void tb_main_1sec_read_00(){


		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");
}
void tb_vread1_sector_erase_retry(){
		bool Pflag[DDUTCNT]={0};
		FlashPatExec("patFlash.mpa","FLASH_SH_VREAD_01");
	
				setRegAddr(AREA_M,0x0);
				setRegDataIn(0xff,0xffffffff,0xffffffff);
				FlashPatExec("patFlash.mpa", "VREAD_SECT_ERAS_RETRY");
					
			
		FlashPatExec("patFlash.mpa","FLASH_SH_VREAD_00");
		//ReadReg_STB_BFAIL_REG(1);
			

}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 6:normal function 

void tb_mrn_chip_erase(){
		setRegAddr(AREA_M_R_N,0x0);
		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
}
void tb_main_chip_erase(){
		setRegAddr(AREA_M,0x0);
		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
}
void tb_main_sector_erase(){
		setRegMultiAddr(AREA_M,0x0,(M_SECT_NUM-1)*SECT_SHIFT_ADDR,SECT_SHIFT_ADDR);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
}
void tb_main_program_ff(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CPROG_NORL_ALL");
}
void tb_main_program_ckbd(){
		
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_ALL");
}
void tb_main_program_ickbd(){
		
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_ALL");
}
void tb_main_program_diagonal(){
		
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CPROG_DIAG_ALL");

}
void tb_main_read_ff(){
     setRegAddr(AREA_M,0x0);
     setRegDataIn(0xff,0xffffffff,0xffffffff);
     FlashPatExec("patFlash.mpa", "BI_CREAD_NORL_ALL");
}
void tb_main_read_ckbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CREAD_CKBD_ALL");
}
void tb_main_read_ickbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CREAD_CKBD_ALL");
}
void tb_main_read_diagonal(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CREAD_DIAG_ALL");
		
}
void tb_main_read_m0_ckbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN0_BI_CSLRD_CKBD");
}
void tb_main_read_m0_ickbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN0_BI_CSLRD_CKBD");
}

void tb_main_read_m1_ff(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_NORL");
}
void tb_main_read_m1_ckbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD");
}
void tb_main_read_m1_ickbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD");
}
void tb_main_read_m1_ht_ff(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_NORL");
}
void tb_main_read_m1_ht_ckbd(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_CKBD");
}
void tb_main_read_m1_ht_ickbd(){
		
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_CKBD");
}


void tb_nvr_sector_erase(){
		setRegMultiAddr(AREA_N,0x0,(N_SECT_NUM/2-1)*SECT_SHIFT_ADDR,SECT_SHIFT_ADDR);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
		setRegMultiAddr(AREA_N,N_SECT_HIGH_ADDR_ALL,N_SECT_HIGH_ADDR_ALL+(N_SECT_NUM/2-1)*SECT_SHIFT_ADDR,SECT_SHIFT_ADDR);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
}
void tb_nvr_program_ff(){ 
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CPROG_NORL_NVR");
}
void tb_nvr_program_ckbd(){ 
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_NVR");
}
void tb_nvr_program_ickbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_NVR");
}
void tb_nvr_read_ff(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
	
		FlashPatExec("patFlash.mpa", "BI_CREAD_NORL_NVR");
}
void tb_nvr_read_ckbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CREAD_CKBD_NVR");
}
void tb_nvr_read_ickbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CREAD_CKBD_NVR");
}

void tb_nvr_read_m0_ckbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN0_BI_CSLRD_CKBD_NVR");
}
void tb_nvr_read_m0_ickbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN0_BI_CSLRD_CKBD_NVR");
}
void tb_nvr_read_m1_ff(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_NORL_NVR");
}
void tb_nvr_read_m1_ckbd(){
		
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD_NVR");
		FlashPatExec("patFlash.mpa", "TMODE_EXIT");
}
void tb_nvr_read_m1_ickbd(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD_NVR");
}
void tb_nvr_read_m1_ht_ff(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_NORL_NVR");
}
void tb_nvr_read_m1_ht_ckbd(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_CKBD_NVR");
}

void tb_nvr_read_m1_ht_ickbd(){
		setRegAddr(AREA_N,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_CKBD_NVR");
}
void tb_rdn_sector_erase(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
		}
}
void tb_rdn_program_ff(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0xff,0xffffffff,0xffffffff);
				FlashPatExec("patFlash.mpa", "BI_SPROG_NORL");
		}
}
void tb_rdn_program_ckbd(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0x0,0x0,0x0);
				FlashPatExec("patFlash.mpa", "BI_SPROG_CKBD");
		}
}
void tb_rdn_program_ickbd(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0xff,0xffffffff,0xffffffff);
				FlashPatExec("patFlash.mpa", "BI_SPROG_CKBD");
		}
}
void tb_rdn_read_ff(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
			
				
				setRegDataIn(0xff,0xffffffff,0xffffffff);
				FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");
				}
}
void tb_rdn_read_ckbd(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
				setRegDataIn(0x0,0x0,0x0);
				FlashPatExec("patFlash.mpa", "BI_SREAD_CKBD");
}
void tb_rdn_read_ickbd(){
		int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0xff,0xffffffff,0xffffffff);
				FlashPatExec("patFlash.mpa", "BI_SREAD_CKBD");
		}
}
void tb_rdn_read_m0_ckbd(){
	int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN0_BI_SSLRD_CKBD");
		}
}
void tb_rdn_read_m0_ickbd(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN0_BI_SSLRD_CKBD");
}
void tb_rdn_read_m1_ff(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_BI_SSLRD_NORL");
}


void tb_rdn_read_m1_ckbd(){


	int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_BI_SSLRD_CKBD");
		}
}
void tb_rdn_read_m1_ickbd(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_BI_SSLRD_CKBD");
}
void tb_rdn_read_m1_ht_ff(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_NORL");
}
void tb_rdn_read_m1_ht_ckbd(){
	int area=AREA_R_MIN;
		for(int i=0; i<AREA_R_NUM; i++){
				area=area<<i;
				setRegAddr(area,0x0);
				setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_CKBD");
		}
}
void tb_rdn_read_m1_ht_ickbd(){
		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_CKBD");
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 7:about idd
void tb_idd_auto_low_power(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_CHIEN_00");
		DcExec("VS_PINS");
}
void tb_idd_auto_power(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_CHIEN_00");
		DcExec("VS_PINS");
}
void tb_idd_deep_power_down(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_DEPDP_01");
		DcExec("VS_PINS");
}
void tb_idd_erase(){
		setDcVSIM(0.0 V, R20V, R200uA, 200 UA, -200 UA, 40 UA, -40 UA, 5);
		setRegAddr(AREA_M,0x0);
		FlashPatExec_NoWait("patFlash.mpa", "FN_CHIP_ERAS_ICC");
		DcExec("VS_PINS");
		UTL_StopFct();
		FlashPatExec("patFlash.mpa", "FLASH_SH_DEPDP_00");
}
void tb_idd_erase_low(){
		setRegAddr(AREA_M,0x0);
		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
		DcExec("VS_PINS");
}
void tb_idd_program(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_CHIEN_00");
		FlashPatExec("patFlash.mpa", "FLASH_SH_DEPDP_00");
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec_NoWait("patFlash.mpa", "BI_CPROG_NORL_ALL_ICC");
		DcExec("VS_PINS");
		UTL_StopFct();
}
void tb_idd_program_low(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_CHIEN_00");
		FlashPatExec("patFlash.mpa", "FLASH_SH_DEPDP_00");
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec_NoWait("patFlash.mpa", "BI_CPROG_NORL_ALL_ICC");
		DcExec("VS_PINS");
		UTL_StopFct();
}
void tb_idd_read_high(){
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec_NoWait("patFlash.mpa","BI_CREAD_CKBD_ALL");
		DcExec("VS_PINS");
		UTL_StopFct();
}
void tb_idd_standby(){
		FlashPatExec("patFlash.mpa", "FLASH_SH_CHIEN_01");
		DcExec("VS_PINS");
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 8:others testblock 
#if 0
void tb_nvrx_read_Vcode_ECID_CP3(){
		ulConfigUbm("SO_PINS", C28, C27);
		START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
			printf("dut=%d,%x,%x,%x,%x\n",dut,vref10_value_code[3][g_flash_no-1][dut-1] , 0x0|vref10_value_code[2][g_flash_no-1][dut-1] , vref10_value_code[1][g_flash_no-1][dut-1], vref10_value_code[0][g_flash_no-1][dut-1]);//36bit: V2+T_Value  V1+T_Value  V0+T_Value
			ulWriteUbm("SO_PINS",0*READ_DATA_BITSIZE,1*READ_DATA_BITSIZE-1,   0xff, 0xffffffff,  0xbfff <<16| 0x8<<12 | VcodeTmp[dut-1][g_flash_no-1]<<8|0x76, dut);//Vcode'
//			ulWriteUbm("SO_PINS",1*READ_DATA_BITSIZE,2*READ_DATA_BITSIZE-1,   0xf0, vref10_value_code[3][g_flash_no-1][dut-1] <<20    |( 0x0000 | vref10_value_code[2][g_flash_no-1][dut-1])<<4    | 0xf,(0x0000 | vref10_value_code[1][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref10_value_code[0][g_flash_no-1][dut-1]), dut);//72bit: V5 ~ V0  Tcode_
			ulWriteUbm("SO_PINS",1*READ_DATA_BITSIZE,2*READ_DATA_BITSIZE-1,   0xf0, vref10_value_code[3][g_flash_no-1][dut-1] <<20    |( 0x0000 | vref10_value_code[2][g_flash_no-1][dut-1])<<4    | 0xf,(0x0000 | vref10_value_code[1][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref10_value_code[0][g_flash_no-1][dut-1]), dut);//72bit: V5 ~ V0  Tcode_
			ulWriteUbm("SO_PINS",2*READ_DATA_BITSIZE,3*READ_DATA_BITSIZE-1,   0xf0, vref10_value_code[7][g_flash_no-1][dut-1] <<20    |( 0x0000 | vref10_value_code[6][g_flash_no-1][dut-1])<<4    | 0xf,(0x0000 | vref10_value_code[5][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref10_value_code[4][g_flash_no-1][dut-1]), dut);//72bit: V11 ~ V6 + Tdode
			ulWriteUbm("SO_PINS",3*READ_DATA_BITSIZE,4*READ_DATA_BITSIZE-1,   0xf0, vref10_value_code[11][g_flash_no-1][dut-1] <<20   |( 0x0000 | vref10_value_code[10][g_flash_no-1][dut-1])<<4   | 0xf,(0x0000 | vref10_value_code[9][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref10_value_code[8][g_flash_no-1][dut-1]), dut);//72bit: T1~T0 Vcode';  V15 ~ V12 Tcode
			ulWriteUbm("SO_PINS",4*READ_DATA_BITSIZE,5*READ_DATA_BITSIZE-1,   0xf0, vref10_value_code[15][g_flash_no-1][dut-1] <<20   |( 0x0000 | vref10_value_code[14][g_flash_no-1][dut-1])<<4   | 0xf,(0x0000 | vref10_value_code[13][g_flash_no-1][dut-1])   <<16 |(0x0000 | vref10_value_code[12][g_flash_no-1][dut-1]), dut);//72bit: T7 ~ T2 Vcode'
			ulWriteUbm("SO_PINS",5*READ_DATA_BITSIZE,6*READ_DATA_BITSIZE-1,   0xf0, vref12_value_code[3][g_flash_no-1][dut-1] <<20    |( 0x0000 | vref12_value_code[2][g_flash_no-1][dut-1])<<4    | 0xf,(0x0000 | vref12_value_code[1][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref12_value_code[0][g_flash_no-1][dut-1]), dut);//72bit: V5 ~ V0  Tcode_
			ulWriteUbm("SO_PINS",6*READ_DATA_BITSIZE,7*READ_DATA_BITSIZE-1,   0xf0, vref12_value_code[7][g_flash_no-1][dut-1] <<20    |( 0x0000 | vref12_value_code[6][g_flash_no-1][dut-1])<<4    | 0xf,(0x0000 | vref12_value_code[5][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref12_value_code[4][g_flash_no-1][dut-1]), dut);//72bit: V11 ~ V6 + Tdode
			ulWriteUbm("SO_PINS",7*READ_DATA_BITSIZE,8*READ_DATA_BITSIZE-1,   0xf0, vref12_value_code[11][g_flash_no-1][dut-1] <<20   |( 0x0000 | vref12_value_code[10][g_flash_no-1][dut-1])<<4   | 0xf,(0x0000 | vref12_value_code[9][g_flash_no-1][dut-1])    <<16 |(0x0000 | vref12_value_code[8][g_flash_no-1][dut-1]), dut);//72bit: T1~T0 Vcode';  V15 ~ V12 Tcode
			ulWriteUbm("SO_PINS",8*READ_DATA_BITSIZE,9*READ_DATA_BITSIZE-1,   0xf0, vref12_value_code[15][g_flash_no-1][dut-1] <<20   |( 0x0000 | vref12_value_code[14][g_flash_no-1][dut-1])<<4   | 0xf,(0x0000 | vref12_value_code[13][g_flash_no-1][dut-1])   <<16 |(0x0000 | vref12_value_code[12][g_flash_no-1][dut-1]), dut);//72bit: T7 ~ T2 Vcode'
			ulWriteUbm("SO_PINS",9*READ_DATA_BITSIZE,10*READ_DATA_BITSIZE-1,  0xf0, vref12_value_codeHT[3][g_flash_no-1][dut-1] <<20  |( 0x0000 | vref12_value_codeHT[2][g_flash_no-1][dut-1])<<4  | 0xf,(0x0000 | vref12_value_codeHT[1][g_flash_no-1][dut-1])  <<16 |(0x0000 | vref12_value_codeHT[0][g_flash_no-1][dut-1]), dut);//72bit: V5 ~ V0  Tcode_
			ulWriteUbm("SO_PINS",10*READ_DATA_BITSIZE,11*READ_DATA_BITSIZE-1, 0xf0, vref12_value_codeHT[7][g_flash_no-1][dut-1] <<20  |( 0x0000 | vref12_value_codeHT[6][g_flash_no-1][dut-1])<<4  | 0xf,(0x0000 | vref12_value_codeHT[5][g_flash_no-1][dut-1])  <<16 |(0x0000 | vref12_value_codeHT[4][g_flash_no-1][dut-1]), dut);//72bit: V11 ~ V6 + Tdode
			ulWriteUbm("SO_PINS",11*READ_DATA_BITSIZE,12*READ_DATA_BITSIZE-1, 0xf0, vref12_value_codeHT[11][g_flash_no-1][dut-1] <<20 |( 0x0000 | vref12_value_codeHT[10][g_flash_no-1][dut-1])<<4 | 0xf,(0x0000 | vref12_value_codeHT[9][g_flash_no-1][dut-1])  <<16 |(0x0000 | vref12_value_codeHT[8][g_flash_no-1][dut-1]), dut);//72bit: T1~T0 Vcode';  V15 ~ V12 Tcode
			ulWriteUbm("SO_PINS",12*READ_DATA_BITSIZE,13*READ_DATA_BITSIZE-1, 0xf0, vref12_value_codeHT[15][g_flash_no-1][dut-1] <<20 |( 0x0000 | vref12_value_codeHT[14][g_flash_no-1][dut-1])<<4 | 0xf,(0x0000 | vref12_value_codeHT[13][g_flash_no-1][dut-1]) <<16 |(0x0000 | vref12_value_codeHT[12][g_flash_no-1][dut-1]), dut);//72bit: T7 ~ T2 Vcode'
			ulWriteUbm("SO_PINS",13*READ_DATA_BITSIZE,14*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfffffff0 | TcodeTmp[dut-1][g_flash_no-1], dut);//72bit: T7 ~ T2 Vcode'
///* info for customer
			ulWriteUbm("SO_PINS",14*READ_DATA_BITSIZE,15*READ_DATA_BITSIZE-1, 0xf0, 0x0401a10f, 0x01c0761f, dut); //Addr120:  0 0401a10f 01c0761f
			ulWriteUbm("SO_PINS",15*READ_DATA_BITSIZE,16*READ_DATA_BITSIZE-1, 0xfd, 0xfff1812f, 0x40001a12, dut); //Addr121:  d fff1812f 40001a12
			ulWriteUbm("SO_PINS",16*READ_DATA_BITSIZE,17*READ_DATA_BITSIZE-1, 0xf0, 0x0011a12f, 0xfffd1812, dut); //Addr122:  0 0011a12f fffd1812
			ulWriteUbm("SO_PINS",17*READ_DATA_BITSIZE,18*READ_DATA_BITSIZE-1, 0xf0, 0x4001a12f, 0xfffb1812, dut); //Addr123:  0 4001a12f fffb1812
			ulWriteUbm("SO_PINS",18*READ_DATA_BITSIZE,19*READ_DATA_BITSIZE-1, 0xff, 0xfff0006f, 0xfeff1812, dut); //Addr124:  f fff0006f feff1812
//			ulWriteUbm("SO_PINS",19*READ_DATA_BITSIZE,20*READ_DATA_BITSIZE-1, 0xff& XYh32[dut-1]>>28 , XYh32[dut-1] <<4|0xf,XYl32[dut-1], dut); //Addr1a2
//			ulWriteUbm("SO_PINS",20*READ_DATA_BITSIZE,21*READ_DATA_BITSIZE-1, 0xff& idh32[dut-1]>>28 , idh32[dut-1] <<4|0xf,idl32[dut-1], dut); //Addr1a3
//			ulWriteUbm("SO_PINS",22*READ_DATA_BITSIZE,23*READ_DATA_BITSIZE-1, 0xf0|fail_sect_addr[dut-1][g_flash_no-1][1]>>4, fail_sect_addr[dut-1][g_flash_no-1][1]<<20|fail_sect_addr[dut-1][g_flash_no-1][0]<<4|0xf, 0xff000000|fail_sect_count[dut-1][g_flash_no-1]<<16|0xaaaa, dut);//Addr 1ac: Pass info + FailSectorAddr 

//			ulWriteUbm("SO_PINS",19*READ_DATA_BITSIZE,20*READ_DATA_BITSIZE-1, 0xf0|LOTID3[dut-1]>>4,LOTID3[dut-1]<<28|LOTID2[dut-1]<<20|LOTID1[dut-1]<<12|LOTID0[dut-1]<<4|0xf,0xffffffff, dut);//Addr 1a2 :LOTID 
//			ulWriteUbm("SO_PINS",20*READ_DATA_BITSIZE,21*READ_DATA_BITSIZE-1, 0xf0|Time_Mon[dut-1]>>4,Time_Mon[dut-1]<<28|Time_Year[dut-1]<<12|g_die_Y[dut-1]<<4|0xf,g_die_X[dut-1]<<24|WaferID[dut-1]<<16|LOTID5[dut-1]<<8|LOTID4[dut-1], dut); //Addr1a3: lotid + X/Y coordinate + Time info  
//			ulWriteUbm("SO_PINS",21*READ_DATA_BITSIZE,22*READ_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xffffff00|Time_Day[dut-1], dut);//Addr1a4: Time info
//*/
			END_DUT_LOOP
			FlashPatExec("patFlash.mpa", "NVR_READ_VCODE_TCODE_CP3");
		END_FLASH_LOOP
}
#endif
#if 0
void tb_nvrx_program_Vcode_ECID_CP3(){
		ulConfigUbm("SI_PINS", C28, C27);
		InitialCpDieId(XYh32, XYl32,idh32,idl32);
		START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
			printf("dut=%d,fail_count=%x,RR1=%x,RR0=%x,\n",dut,fail_sect_count[dut-1][g_flash_no-1],fail_sect_addr[dut-1][g_flash_no-1][1],fail_sect_addr[dut-1][g_flash_no-1][0] );
			printf("dut=%d,Vcode=%x,Tcode=%x,\n",dut, VcodeTmp[dut-1][g_flash_no-1],TcodeTmp[dut-1][g_flash_no-1]);//Vcode',Tcode'
 			printf("Vref10_NT test data:");	
			printf("dut=%d,%x,%x,%x,%x,\n",dut, vref10_value_code[3][g_flash_no-1][dut-1],vref10_value_code[2][g_flash_no-1][dut-1],vref10_value_code[1][g_flash_no-1][dut-1],vref10_value_code[0][g_flash_no-1][dut-1]);//36bit: V2+T_Value  V1+T_Value  V0+T_Value
			printf("dut=%d,%x,%x,%x,%x,\n",dut, vref10_value_code[7][g_flash_no-1][dut-1],vref10_value_code[6][g_flash_no-1][dut-1],vref10_value_code[5][g_flash_no-1][dut-1],vref10_value_code[4][g_flash_no-1][dut-1]);//36bit: V2+T_Value  V1+T_Value  V0+T_Value
			printf("dut=%d,%x,%x,%x,%x,\n",dut, vref10_value_code[11][g_flash_no-1][dut-1],vref10_value_code[10][g_flash_no-1][dut-1],vref10_value_code[9][g_flash_no-1][dut-1],vref10_value_code[8][g_flash_no-1][dut-1]);//36bit: V2+T_Value  V1+T_Value  V0+T_Value
			printf("dut=%d,%x,%x,%x,%x,\n",dut, vref10_value_code[15][g_flash_no-1][dut-1],vref10_value_code[14][g_flash_no-1][dut-1],vref10_value_code[13][g_flash_no-1][dut-1],vref10_value_code[12][g_flash_no-1][dut-1]);//36bit: V2+T_Value  V1+T_Value  V0+T_Value
 			printf("Vref12_NT test data:");	
 			printf("Vref12_HT test data:");	
			ulWriteUbm("SI_PINS",0*PROG_DATA_BITSIZE,1*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff,  0xbfff<<16|0x8<<12|VcodeTmp[dut-1][g_flash_no-1]<<8|0x76,dut);//Vcode' 0x180[11:8]  Tcode=8 0x180[15:12] 
			ulWriteUbm("SI_PINS",1*PROG_DATA_BITSIZE,2*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[1][g_flash_no-1][dut-1]<<16|vref10_value_code[0][g_flash_no-1][dut-1], dut);	//Addr 1c0 l36bits 
			ulWriteUbm("SI_PINS",2*PROG_DATA_BITSIZE,3*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[3][g_flash_no-1][dut-1]<<16|vref10_value_code[2][g_flash_no-1][dut-1], dut);	//Addr 1c0 h36bits
			ulWriteUbm("SI_PINS",3*PROG_DATA_BITSIZE,4*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[5][g_flash_no-1][dut-1]<<16|vref10_value_code[4][g_flash_no-1][dut-1], dut);	//Addr 1c1 l36bits 
			ulWriteUbm("SI_PINS",4*PROG_DATA_BITSIZE,5*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[7][g_flash_no-1][dut-1]<<16|vref10_value_code[6][g_flash_no-1][dut-1], dut); 	//Addr 1c1 h36bits
			ulWriteUbm("SI_PINS",5*PROG_DATA_BITSIZE,6*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[9][g_flash_no-1][dut-1]<<16|vref10_value_code[8][g_flash_no-1][dut-1], dut);	//Addr 1c2 l36bits
			ulWriteUbm("SI_PINS",6*PROG_DATA_BITSIZE,7*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[11][g_flash_no-1][dut-1]<<16|vref10_value_code[10][g_flash_no-1][dut-1], dut);	//Addr 1c2 h36bits
			ulWriteUbm("SI_PINS",7*PROG_DATA_BITSIZE,8*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[13][g_flash_no-1][dut-1]<<16|vref10_value_code[12][g_flash_no-1][dut-1], dut);	//Addr 1c3 l36bits
			ulWriteUbm("SI_PINS",8*PROG_DATA_BITSIZE,9*PROG_DATA_BITSIZE-1,   0xff, 0xffffffff, vref10_value_code[15][g_flash_no-1][dut-1]<<16|vref10_value_code[14][g_flash_no-1][dut-1], dut);	//Addr 1c3 h36bits
			ulWriteUbm("SI_PINS",25*PROG_DATA_BITSIZE,26*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfffffff0|TcodeTmp[dut-1][g_flash_no-1], dut);//Tcode' 0x1CC[3:0]
///* info for customer
			ulWriteUbm("SI_PINS",26*PROG_DATA_BITSIZE,27*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x01c0 <<16|0x761f, dut);    	//Addr120 :  01c0761f
			ulWriteUbm("SI_PINS",27*PROG_DATA_BITSIZE,28*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x0040 <<16|0x1a10, dut);		//Addr120 :0 0401a10f
			ulWriteUbm("SI_PINS",28*PROG_DATA_BITSIZE,29*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x4000 <<16|0x1a12, dut);    	//Addr121 :  40001a12
			ulWriteUbm("SI_PINS",29*PROG_DATA_BITSIZE,30*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xdfff <<16|0x1812, dut);		//Addr121 :d fff1812f
			ulWriteUbm("SI_PINS",30*PROG_DATA_BITSIZE,31*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfffd <<16|0x1812, dut);     	//Addr122 :  fffd1812
			ulWriteUbm("SI_PINS",31*PROG_DATA_BITSIZE,32*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x0001 <<16|0x1a12, dut);		//Addr122 :0 0011a12f
			ulWriteUbm("SI_PINS",32*PROG_DATA_BITSIZE,33*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfffb <<16|0x1812, dut);     	//Addr123 :  fffb1812
			ulWriteUbm("SI_PINS",33*PROG_DATA_BITSIZE,34*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0x0400 <<16|0x1a12, dut);		//Addr123 :0 4001a12f
			ulWriteUbm("SI_PINS",34*PROG_DATA_BITSIZE,35*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfeff <<16|0x1812, dut);     	//Addr124 :  feff1812
			ulWriteUbm("SI_PINS",35*PROG_DATA_BITSIZE,36*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xffff <<16|0x0006, dut);		//Addr124 :f fff0006f
			ulWriteUbm("SI_PINS",36*PROG_DATA_BITSIZE,37*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, XYl32[dut-1], dut);                 //Addr1a2 l32
			ulWriteUbm("SI_PINS",37*PROG_DATA_BITSIZE,38*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, XYh32[dut-1], dut);                 //Addr1a2 h32
			ulWriteUbm("SI_PINS",38*PROG_DATA_BITSIZE,39*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, idl32[dut-1], dut);                 //Addr1a3 l32 
			ulWriteUbm("SI_PINS",39*PROG_DATA_BITSIZE,40*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, idh32[dut-1], dut);                 //Addr1a3 h32
//			ulWriteUbm("SI_PINS",36*PROG_DATA_BITSIZE,37*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, LOTID3[dut-1]<<24|LOTID2[dut-1]<<16|LOTID1[dut-1]<<8|LOTID0[dut-1], dut);//Addr1a2 h36bits
//			ulWriteUbm("SI_PINS",37*PROG_DATA_BITSIZE,38*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, g_die_X[dut-1]<<24| WaferID[dut-1]<<16|LOTID5[dut-1]<<8|LOTID4[dut-1], dut);//Addr1a3 l36bits
//			ulWriteUbm("SI_PINS",38*PROG_DATA_BITSIZE,39*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, Time_Mon[dut-1]<<24|Time_Year[dut-1]<<8|g_die_Y[dut-1], dut);//Addr1a3 h36bits
//			ulWriteUbm("SI_PINS",39*PROG_DATA_BITSIZE,40*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xffffff00|Time_Day[dut-1], dut);	//Addr1a4 l36bits
			ulWriteUbm("SI_PINS",40*PROG_DATA_BITSIZE,41*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xff000000|fail_sect_count[dut-1][g_flash_no-1]<<16|0xaaaa, dut); //Addr1ac :aaaa  pass die logo
			ulWriteUbm("SI_PINS",41*PROG_DATA_BITSIZE,42*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xffff0000|fail_sect_addr[dut-1][g_flash_no-1][1]<<8|fail_sect_addr[dut-1][g_flash_no-1][0], dut);//Addr1ac :RR0 RR1
//*/
			END_DUT_LOOP  
			FlashPatExec("patFlash.mpa", "NVR_PROG_VCODE_TCODE_CP3");
		END_FLASH_LOOP
}
#endif
void Read_NVRCFG(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname)
{
	int ybits = 0, xbits=0;
	int transfer_bits = 13;
	int tempyaddr=FCM_READ_DATA_BITSIZE-1;
	
	int start_addr_pre=0;
	int end_addr_pre=endaddr - startaddr;
	int tempxaddr = start_addr_pre ^ end_addr_pre;
	while(tempyaddr!=0)
	{
		ybits++;
		tempyaddr = tempyaddr>>1;
	}

	while(tempxaddr!=0)
	{
		xbits++;
		tempxaddr=tempxaddr>>1;
	}
	ulConfigFcm("SO", xbits, ybits, 1<<transfer_bits);

	UT_DUT sysdut;	
	FILE* fp;
	char fname[100];
	time_t t;
	struct tm * lt;
	time (&t);
	lt = localtime (&t);

	unsigned int i,j,d,k,fn;
	unsigned int *fcm_data=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits) * (1<<ybits));
	unsigned int *d1=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d2=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d3=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//	unsigned int *d1[FLASH_NUM];
//	unsigned int *d2[FLASH_NUM];
//	unsigned int *d3[FLASH_NUM];
 START_FLASH_LOOP
	setRegValue(UT_REG_D2C, area<<8);//set for Y15-8
	setRegValue(UT_REG_D1C, startaddr|addr_mask);//set for X23-0
	setRegValue(UT_REG_D1B, stepaddr);
	setRegDataIn(0x0,0x0,0x0);

	RegHandle h = UTL_GetRegHandle();
	if((xbits+ybits) > transfer_bits){                                                                                                                                                                                                                  
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<(transfer_bits-ybits))/stepaddr - 2);  // bytes for each fcm transfer//error
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 0);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, (1<<(xbits+ybits-transfer_bits))-2);  // transfer count - 1
	}
	else{
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<xbits)/stepaddr - 2 );  // bytes for each fcm transfer
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 2);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, 0              );  // transfer count - 1
	}
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);

	ulPresetFcm();
//	FlashPatExec("patFlash.mpa", "FN_NORL_READ_FCM");
//	FlashPatExec("patFlash.mpa", "FN_NORL_READ_FCM");
	FlashPatStart("patFlash.mpa", patname);
	
	START_DUT_LOOP(UT_CDUT)
//	for(fn=0;fn<FLASH_NUM;fn++)
//	{
//		d1[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//		d2[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//		d3[fn]=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
//	}
//	for(fn=0;fn<FLASH_NUM;fn++){
//	}

	
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, (1<<xbits)-1, 0, (1<<ybits)-1, dut);	
			memset(d1, 0, sizeof(unsigned int)*(1<<xbits));//bit31:0
			memset(d2, 0, sizeof(unsigned int)*(1<<xbits));//bit63:32
			memset(d3, 0, sizeof(unsigned int)*(1<<xbits));//bit71:64
			for(i=start_addr_pre,k=0;i<=end_addr_pre;i++,k++)
			{

				for(j=0;j<(FCM_READ_DATA_BITSIZE<32?FCM_READ_DATA_BITSIZE:32);j++)
				{
					//d1[g_flash_no-1][i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZ-1-j]))&1)<<j;
					d1[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<j;
					//d1[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<j;
				}
				for(j=32;j<(FCM_READ_DATA_BITSIZE<64?FCM_READ_DATA_BITSIZE:64);j++)
				{
					d2[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-32);
					//d2[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<(j-32);
				}
				for(j=64;j<(FCM_READ_DATA_BITSIZE<72?FCM_READ_DATA_BITSIZE:72);j++)
				{
					//d3[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-(1<<ybits));
					d3[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-64);
					//d3[i] |= (((fcm_data[i*(1<<ybits)+j]))&1)<<(j-64);
				}
				DATA3[k][dut-1][g_flash_no-1]=d3[i];
				DATA2[k][dut-1][g_flash_no-1]=d2[i];
				DATA1[k][dut-1][g_flash_no-1]=d1[i];
		//		printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, 0, d3[i], d2[i], d1[i]);fflush(stdout);
		//		printf("DUT[%02d]\tFLASH[%02d] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut,g_flash_no, 0, d3[i], d2[i], d1[i]);
				fflush(stdout);
			}
	END_DUT_LOOP
END_FLASH_LOOP
	free(fcm_data);
	free(d1);
	free(d2);
	free(d3);
//	for(fn=0;fn<FLASH_NUM;fn++)
//	{
//	free(d1[fn]);
//	free(d2[fn]);
//	free(d3[fn]);
//	}
}

void tb_nvr_cfg_dump_NcData(){
	unsigned int DATA3[0xff][DDUTCNT][FLASH_NUM],DATA2[0xff][DDUTCNT][FLASH_NUM],DATA1[0xff][DDUTCNT][FLASH_NUM];
	FCM_READ_DATA_BITSIZE=72;
	UT_DUT sysdut;
	Read_NVRCFG(AREA_NC,NVR_CFG_ADDR_For_Customer_CP1,NVR_CFG_ADDR_For_Customer_CP1+NC_Addr_Num-1,0x1,0x0,DATA3,DATA2,DATA1); 
	START_FLASH_LOOP
		START_DUT_LOOP(UT_CDUT)
                        UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			printf("\nSITE:%d\tSDUT:%d\tDUT:%d\tflash:%d\n",UTL_ReadSiteNumber(),sysdut,dut,g_flash_no);
			for(int i=0;i<NC_Addr_Num;i++){

			//printf("i= %02d,DUT[%02d]\tFLASH[%02d], data = 0x%02X_0x%08X_%08X\n",i, dut,g_flash_no,DATA3[i][dut-1][g_flash_no-1],DATA2[i][dut-1][g_flash_no-1],DATA1[i][dut-1][g_flash_no-1]);	
			printf("\ni= %02d,DUT[%02d]\t[addr0x%x]\t[FLASH[%02d], data = 0x%02X_0x%08X_%08X\n",i, dut,NVR_CFG_ADDR_For_Customer_CP1+i,g_flash_no,DATA3[i][dut-1][g_flash_no-1],DATA2[i][dut-1][g_flash_no-1],DATA1[i][dut-1][g_flash_no-1]);	
		        NVR_CFG_BYFLASH_CP1[dut-1][i][g_flash_no-1]= DATA1[i][dut-1][g_flash_no-1] ;
//	                printf("NVR_CFG_BYFLASH_CP1[dut%02d][addr0x%x][flash %02d]=0x%8x\n", dut,NVR_CFG_ADDR_For_Customer_CP1+i,g_flash_no,NVR_CFG_BYFLASH_CP1[dut-1][i][g_flash_no-1]);	
   			}
		        printf("\n");	
		END_DUT_LOOP
		        printf("\n");	
	END_FLASH_LOOP
}




///--- void Meas_read_trim_Savefile(char *trimlabel,int flag[][DDUTCNT],int trim[DDUTCNT])
///--- {
///--- 
///--- 	FILE* fp;
///--- 	char fname[100];
///--- 	time_t t;
///---     struct tm * lt;
///---     time (&t);
///--- 	lt = localtime (&t);
///--- 		sprintf(fname,"/home/fsdiag/SMIC/data_detail/%s trim_%04d-%02d-%02d_%02d%02d%02d.csv", trimlabel,lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 		if((fp=fopen(fname,"aw")) == NULL)
///--- 		{
///--- 			printf("An error happens while opening a file named \" %s \".\n",fname);
///--- 		}
///--- 		else
///--- 		{		
///--- 				fprintf(fp,"X\t%d\tY\t%d\tDUT\t%d\t",XYh32[dut],XYl32[dut],dut);
///--- 				for(int i=0;i<32;i++)
///--- 				{
///--- 						if(!flag[i][dut-1])
///--- 								
///--- 								fprintf(fp,"P\t");
///--- 						else
///--- 								fprintf(fp,"F\t");
///--- 						
///--- 						if(flag[i+1][dut-1]-flag[i][dut-1]==1){
///--- 								trim[dut-1]=i;
///--- 						}
///--- 				}
///--- 				fprintf(fp,"BTC:\t%d\n",REF_trim_value[trim[dut-1]]);
///--- 		}
///--- 		END_DUT_LOOP
///--- 			fclose(fp);
///--- 			printf("save file:%s\n",  fname);
///--- }


///--- void tb_Pre_program_2rows_00(){
///--- 		setRegAddr(AREA_M_R,0x0);
///--- 		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "BI_RPPRO_NORL");
///--- 		setRegAddr(AREA_M,ROW_SHIFT_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "BI_RPPRO_NORL");
///--- }

///--- void tb_chip_erase_high_M_R(){
///--- 		setRegAddr(AREA_M_R,0x0);
///--- 		FlashPatExec("patFlash.mpa", "FN_CHIP_ERAS");
///--- }


///--- void tb_measure_VEG_chip_erase(){
///--- 
///---                  FlashPatExec_NoWait("patFlash.mpa","MEAS_VEG_CHIP_ERASE");
///---                  UTL_WaitTime(15 MS);
///---                  DcExec("VPP1_PINS");
///---                  Read_Meas_Result("VPP1_PINS", "VEG HV",5.12 V, 4.88 V, 1 V, "V");
///---                  UTL_StopFct();
///---                  FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
///--- }
///--- void measure_VCG_VSIM(double hlimit, double lolimit)
///--- {
///--- 
///--- 		double VCG_target=4.72 V;
///--- 		double trim_data[DDUTCNT][FLASH_NUM]={0};
///--- 		bool Fail_flag[DDUTCNT]={0};
///--- 		UT_DUT sysdut;
///--- 		SettlingTimeHandle handle_HV = UTL_GetSettlingTimeHandle();
///--- 		UTL_SetSettlingTimeAfterRon(handle_HV, 5 MS);
///--- 		UTL_SetSettlingTimeAfterRof(handle_HV, 3 MS);
///--- 		UTL_SendSettlingTime(handle_HV);
///--- 		UTL_DeleteHandle(handle_HV);
///--- 
///--- 		setHvVSIM("VPP1_PINS",VCG_target,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
///--- 		setHvUnMask("VPP1_PINS");
///--- 
///--- 	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
///--- 		FlashPatExec("patFlash.mpa","MEAS_VCG_PROG");
///--- 		UTL_WaitTime(30 MS);
///--- 		HVVSIM_Measure_Voltage_ByFlash("VEG HV",g_pinlist[vspinno].vsname,vspinno,VCG_target , trim_data, g_flash_no);
///--- 	      	UTL_StopFct();
///---         	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
///--- 
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 
///--- 			UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
///--- 			HV_measure_VCG_temp[dut-1][g_flash_no-1]=trim_data[dut-1][g_flash_no-1];
///--- 			if(trim_data[dut-1][g_flash_no-1]>=lolimit&&trim_data[dut-1][g_flash_no-1]<=hlimit)
///--- 				printf("\nx:%d,y:%d\t,SDUT%d,DUT%d FLASH%d VCG measure\t%.3f pass",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, trim_data[dut-1][g_flash_no-1]);
///--- 			else    {
///--- 				printf("\nx:%d,y:%d\t,SDUT%d,DUT%d FLASH%d VCG measure\t%.3f fail",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, trim_data[dut-1][g_flash_no-1]);
///--- 				Fail_flag[dut-1]=1;
///--- 				}
///--- 
///--- 		END_DUT_LOOP
///--- 		printf("\n");
///--- 	END_FLASH_LOOP_BY_PINLIST
///--- 
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			if(Fail_flag[dut-1]==0)
///--- 			{
///--- 				SetFinalResultOnly(dut,PASS);
///--- 			}
///--- 			else
///--- 			{	
///--- 				SetFinalResultOnly(dut,FAIL);
///--- 			}
///--- 		END_DUT_LOOP
///--- 		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
///--- 		setHvMask("VPP1_PINS");
///--- 
///--- 
///--- 
///--- 
///--- }


///--- void tb_program_VCG_trimming_Binary(){
///--- #if 0
///--- 	setHvUnMask("VPP1_PINS");
///--- 	setHvVSIM("VPP1_PINS",4.72 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 1);
///--- 	ulSetSettlingTime(5 MS,8 MS,3 MS);
///--- 	int CBD3_IN=0;
///--- 	int addr=0x02;
///--- 	int counter =0;
///--- 	double target=0, target_delta=0.01 ;
///--- 	double error=0.06;
///--- 	int i=0;
///--- 	int start[DDUTCNT][FLASH_NUM]={0},stop[DDUTCNT][FLASH_NUM]={0},mid[DDUTCNT][FLASH_NUM]={0};
///--- 	int trimcount=24;
///--- 	int testcount=3;
///--- 	double value[DDUTCNT][FLASH_NUM]={0};
///--- 	double trim_data[DDUTCNT][FLASH_NUM]={0};
///--- 	double lolimit=4.66;
///--- 	double hlimit=4.78;
///--- 	double temp[DDUTCNT][FLASH_NUM][10]={0},data[10][DDUTCNT];
///--- 	double delta[DDUTCNT][FLASH_NUM]={0},delta_temp[DDUTCNT][FLASH_NUM]={0};
///--- 	double temp_1,temp_2;
///--- 	long long int active_dut=0;
///--- 	GetActiveDut(&active_dut);
///--- 	UT_DUT sysdut;
///--- 	struct timespec t_start, t_end;
///--- 	double t;
///--- 	int equal_flag[DDUTCNT][FLASH_NUM]={0};
///--- 
///--- 	START_FLASH_LOOP
///--- 		START_DUT_LOOP(UT_CDUT) 
///--- 			for( i=0;i<trimcount+1;i++)
///--- 				VCG_trim_result[i][dut-1][g_flash_no-1]=-999;	
///--- 			delta_temp[dut-1][g_flash_no-1]=0;
///--- 			stop[dut-1][g_flash_no-1]=trimcount;	
///--- 			mid[dut-1][g_flash_no-1]=15;
///--- 		END_DUT_LOOP
///--- 		END_FLASH_LOOP
///--- 
///--- 		
///--- 	ulConfigUbm("SI", C28, C27);
///--- 	
///--- 	ExclusionHandle hex=UTL_GetExclusionHandle();
///--- 	UTL_SetExclusionIgnoreWet  (hex,UT_OFF);                            
///--- 	UTL_SetExclusionMask       (hex,UT_OFF);		
///--- 
///--- 
///--- 
///--- 	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
///--- 		while(UTL_GetDutCount(UT_CDUT))		
///--- 		{
///--- 		
///--- 			START_DUT_LOOP(UT_CDUT)
///--- 			
///--- 				ulWriteUbm("SI",0,PROG_DATA_BITSIZE-1, 0x0, 0x0, VCG_trim_value[mid[dut-1][g_flash_no-1]]<<0, dut);
///--- 			END_DUT_LOOP
///--- 	
///--- 		
///--- 			setRegAddr(AREA_M,addr);
///--- 			FlashPatExec_NoWait("patFlash.mpa","PROG_VCG_TRIM_UBM");
///--- 	
///--- 			
///--- 			UTL_WaitTime(35 MS);
///--- 			START_GROUP_TEST_BY_VPP(vspinno)
///--- 				for(i=0; i <testcount;i++)
///--- 				{
///--- 					DcExecQuick2(g_pinlist[vspinno].vsname);
///--- 					START_DUT_LOOP(UT_CDUT)
///--- 						ReadMeasData(g_pinlist[vspinno].vsname,  1 UA, data[i],dut);
///--- 						temp[dut-1][g_flash_no-1][i]= data[i][dut-1];			
///--- 					END_DUT_LOOP
///--- 				}		
///--- 				
///--- 				START_DUT_LOOP(UT_CDUT)
///--- 					if(VCG_trim_result[mid[dut-1][g_flash_no-1]][dut-1][g_flash_no-1]==-999)
///--- 					{
///--- 						getMidValue(temp[dut-1][g_flash_no-1],testcount,VCG_trim_result[mid[dut-1][g_flash_no-1]],g_flash_no);
///--- 					
///--- 					}
///--- 				END_DUT_LOOP
///--- 			END_GROUP_TEST_BY_VPP(vspinno)
///--- 			UTL_StopFct();
///--- 			FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
///--- 	
///--- 			UTL_ClearExclusionDut(hex);
///--- 	
///--- 			START_DUT_LOOP(UT_CDUT)
///--- 				int flag ;
///--- 				delta[dut-1][g_flash_no-1]=VCG_trim_result[mid[dut-1][g_flash_no-1]][dut-1][g_flash_no-1]- target;
///--- 	
///--- 				
///--- 				if(fabs(delta[dut-1][g_flash_no-1])>0.05 UA)
///--- 				{
///--- 					if(VCG_trim_result[mid[dut-1][g_flash_no-1]][dut-1][g_flash_no-1]<target)stop[dut-1][g_flash_no-1]=mid[dut-1][g_flash_no-1];
///--- 					else start[dut-1][g_flash_no-1]=mid[dut-1][g_flash_no-1];
///--- 					
///--- 					temp_1= VCG_trim_result[start[dut-1][g_flash_no-1]][dut-1][g_flash_no-1];
///--- 					temp_2= VCG_trim_result[stop[dut-1][g_flash_no-1]][dut-1][g_flash_no-1];
///--- 					if (temp_1!=-999 && temp_2 != -999 && temp_1* temp_2< 0)
///--- 						mid[dut-1][g_flash_no-1]= (int)((stop[dut-1][g_flash_no-1]*temp_1 - start[dut-1][g_flash_no-1]*temp_2)/(temp_1- temp_2)+0.5);
///--- 					else mid[dut-1][g_flash_no-1]=(int)((start[dut-1][g_flash_no-1]+stop[dut-1][g_flash_no-1])/2+0.5);
///--- 				}
///--- 				
///--- 				else
///--- 				{	if(delta[dut-1][g_flash_no-1]>target_delta )
///--- 					{ start[dut-1][g_flash_no-1]=mid[dut-1][g_flash_no-1];mid[dut-1][g_flash_no-1]+=1;}
///--- 					else if(delta[dut-1][g_flash_no-1] <-target_delta)
///--- 					{stop[dut-1][g_flash_no-1]=mid[dut-1][g_flash_no-1];mid[dut-1][g_flash_no-1]-=1;}
///--- 				}
///--- 	
///--- 			
///--- 				
///--- 				if(start[dut-1][g_flash_no-1]+1>=stop[dut-1][g_flash_no-1] ||fabs(delta[dut-1][g_flash_no-1])<=target_delta || equal_flag[dut-1][g_flash_no-1]==1)
///--- 
///--- 				UTL_AddExclusionDut(hex,dut);
///--- 				delta_temp[dut-1][g_flash_no-1]=delta[dut-1][g_flash_no-1];
///--- 					
///--- 			END_DUT_LOOP          
///--- 			UTL_SetExclusionSetOrReset (hex,UT_ON);                             
///--- 			UTL_SendExclusion(hex);       
///--- 	                                    
///--- 		}
///--- 		ReturnActiveDut(active_dut);
///--- 	END_FLASH_LOOP_BY_PINLIST
///--- 
///--- 	printf("\nGet best trim value:");
///--- 	START_FLASH_LOOP
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			CalMultitrimvalue("VCG",VCG_trim_result,VCG_trim,target,error,trimcount,dut) ;	
///--- 		END_DUT_LOOP
///--- 	END_FLASH_LOOP
///--- #if 1   
///--- 	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
///--- 		setRegAddr(AREA_M,addr);
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			ulWriteUbm("SI",0,PROG_DATA_BITSIZE-1, 0x0, 0x0, VCG_trim_value[VCG_trim[dut-1][g_flash_no-1]], dut);
///--- 		END_DUT_LOOP
///--- 
///--- 		FlashPatExec_NoWait("patFlash.mpa","PROG_VCG_TRIM_UBM");
///--- 		UTL_WaitTime(40 MS);
///--- 		
///--- 		HVVSIM_Measure_Voltage_ByFlash("VCG HV",g_pinlist[vspinno].vsname,vspinno,4.5 V , value, g_flash_no);
///--- 		UTL_StopFct();
///--- 		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
///--- 	END_FLASH_LOOP_BY_PINLIST
///--- 
///--- 	START_FLASH_LOOP
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
///--- 			VCG_trim_result[trimcount][dut-1][g_flash_no-1]=value[dut-1][g_flash_no-1];
///--- 			if(value[dut-1][g_flash_no-1]>=lolimit&&value[dut-1][g_flash_no-1]<=hlimit)
///--- 				printf("\nx:%3d,y:%3d\t,SDUT%d,DUT%d FLASH%d trim VCG\t%.3f pass",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no,value[dut-1][g_flash_no-1]);
///--- 			else
///--- 				printf("\nx:%3d,y:%3d\t,SDUT%d,DUT%d FLASH%d trim VCG\t%.3f fail",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no,value[dut-1][g_flash_no-1]);
///--- 		END_DUT_LOOP
///--- 	END_FLASH_LOOP
///--- 
///--- #endif
///--- 	START_DUT_LOOP(UT_CDUT)
///--- 		START_FLASH_LOOP
///--- 			if(value[dut-1][g_flash_no-1]>=lolimit&&value[dut-1][g_flash_no-1]<=hlimit)
///--- 				SetFinalResultOnly(dut,PASS);
///--- 			else{		
///--- 				SetFinalResultOnly(dut,FAIL);
///--- 				break;
///--- 			}
///--- 		END_FLASH_LOOP
///--- 	END_DUT_LOOP          
///--- 	UTL_DeleteHandle(hex);
///--- 	setHvVSIM("VPP1_PINS", 0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 1);
///--- 	setHvMask("VPP1_PINS");
///--- 
///--- 
///--- 
///--- 
///--- 
///--- 
///--- 
///--- #endif 
///--- }



///--- void tb_set_VSL_trim_Reg(){
///--- 		int	addr=0x01;
///--- 		ulConfigUbm("SI", C28, C27);
///--- 		int CBD1_IN[DDUTCNT]={0};
///--- 		long long int active_dut=0;
///--- 		UT_DUT sysdut;
///--- 		GetActiveDut(&active_dut);
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			ulWriteUbm("SI",0*PROG_DATA_BITSIZE,1*PROG_DATA_BITSIZE-1, 0x0, 0x0, 0x8, dut);
///--- 		END_DUT_LOOP
///--- 		printf("\n");		
///--- 		setRegAddr(AREA_M,addr);
///--- 		FlashPatExec_NoWait("patFlash.mpa","PROG_VSL_TRIM_UBM");
///--- 		UTL_StopFct();
///--- 		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
///--- }


///--- void tb_meo_program00_M_R_N(){
///--- 		setRegMultiAddr(AREA_M_R_N,0x0,ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");
///--- 		setRegMultiAddr(AREA_M_R_N,0x80,ROW_SHIFT_ADDR-1,PROG_WORD_SHIFT_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "MEO_PROG_00_M_R");
///--- }
///--- void tb_nvr_cfg_program_read_CP2_id_high(){
///--- 		
///--- 		setRegMultiAddr(AREA_NC, CP2_ID_ADDR, CP2_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
///--- 		setRegDataIn(0x0,0xFFFFFFFF,CP2_ID_DATA);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
///--- 		setRegAddr(AREA_NC, CP2_ID_ADDR);
///--- 		setRegDataOut(0x0,0xffffffff, CP2_ID_DATA);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L32");
///--- 	
///--- }
///--- void tb_nvr_cfg_program_read_CP3_id_high(){
///--- 
///--- 		
///--- 		setRegMultiAddr(AREA_NC, CP3_ID_ADDR, CP3_ID_ADDR+ROW_SHIFT_ADDR, ROW_SHIFT_ADDR);
///--- 		setRegDataIn(0x0,0x0,CP3_ID_DATA);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_ID");
///--- 		setRegAddr(AREA_NC, CP3_ID_ADDR);
///--- 		setRegDataOut(0xff,0xffffffff, CP3_ID_DATA);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L32");
///--- 		
///--- }
///--- void tb_nvr_cfg_read_CP3_id_high(){
///--- 		
///--- 		setRegAddr(AREA_NC, CP3_ID_ADDR);
///--- 		setRegDataOut(0xff,0xffffffff, CP3_ID_DATA);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_L32");
///--- 		
///--- }
///--- void tb_main_FN_read_ff(){
///--- 		
///--- 		
///--- 		
///--- 		
///--- 		setRegDataOut(0x0,0x0,0x0);
///--- 		setRegMultiAddr(AREA_M,0x0000,0x3,0x1);
///--- 
///--- 
///--- 		FlashPatExec("patFlash.mpa", "FN_NORL_READ");
///--- 
///--- 
///--- 
///--- 
///--- }
///--- void tb_nvr_cfg_FN_read_ff(){
///--- 		FlashPatExec("patFlash.mpa", "FLASH_SH_RECAL_01");
///--- 		
///--- 		
///--- 		
///--- 		
///--- 		setRegMultiAddr(AREA_NC,0,ROW_SHIFT_ADDR,1);
///--- 		setRegDataOut(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "FN_NORL_READ");
///--- }
///--- void tb_nvr_cfg_read_s1id(){
///--- 		
///--- START_FLASH_LOOP
///--- 		setRegAddr(AREA_NC, CP1_ID_ADDR);
///--- 
///--- 		setRegDataOut(0xff, CP1_ID_DATA,0xFFFFFFFF);
///--- 		FlashPatExec("patFlash.mpa", "NVR_CFG_RD_ID_H32");
///--- END_FLASH_LOOP	
///--- 	
///--- }
///--- void tb_nvrx_program_ickbd(){
///--- 		setRegAddr(AREA_N,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_NVR");
///--- }
///--- void tb_nvrx_read_Margin0_00(){
///--- 		setRegAddr(AREA_N,0x0);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_NORL_NVR");
///--- }
///--- void tb_nvrx_read_00(){
///--- 		setRegAddr(AREA_N,0x0);
///--- 		setRegDataIn(0x00,0x00000000,0x00000000);
///--- 		FlashPatExec("patFlash.mpa", "BI_SSLRD_NORL");
///--- }

///--- void tb_program_ickbd_nvrx(){
///--- 		
///--- 		setRegAddr(AREA_N,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "BI_CPROG_CKBD_NVR");
///--- }
///--- void tb_read_Margin0_array_00(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0x00,0x00000000,0x00000000);
///--- 		FlashPatExec("patFlash.mpa", "MARGIN0_BI_CSLRD_NORL");
///--- }
///--- void tb_read_Margin1_ckbd_LVCTL(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_01");
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD");
///--- 		FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_00");
///--- }
///--- void tb_read_Margin1_erows_ff(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_NORL");
///--- }
///--- void tb_read_Margin1_ht_ff(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_HT_BI_CSLRD_NORL");
///--- }
///--- void tb_read_Margin1_ickbd_LVCTL(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		
///--- 		FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_01");
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_CKBD");
///--- 		FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_00");
///--- }
///--- void tb_read_Margin1_orows_ff(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "MARGIN1_BI_CSLRD_NORL");
///--- }
///--- void tb_read_Margin1_rdn(){
///--- 		setRegRDNAreaLoop(AREA_R_MIN, AREA_R_NUM, 0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_BI_SSLRD_NORL");
///--- }
///--- void tb_read_array_00(){
///--- 		setRegAddr(AREA_M,0x0);
///--- 		setRegDataIn(0x00,0x00000000,0x00000000);
///--- 		FlashPatExec("patFlash.mpa", "BI_CREAD_NORL_ALL");
///--- }
///--- void tb_sect_read_Margin1_ht_array_ickbd(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_CKBD");
///--- }
///--- void tb_sect_read_Margin1_ht_array_ckbd(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_CKBD");
///--- }
///--- void tb_sect_read_Margin0_array_00(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "RDN_MARGIN0_BI_SSLRD_NORL");
///--- }
///--- void tb_sect_read_Margin1_ht_array_ff(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "RDN_MARGIN1_HT_BI_SSLRD_NORL");
///--- }
///--- void tb_sect_read_array_00(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");
///--- }
///--- void tb_sect_read_array_FF(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "BI_SREAD_NORL");
///--- }
///--- void tb_sect_read_array_ickbd(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "BI_SREAD_CKBD");
///--- }
///--- void tb_sect_read_array_ckbd(){
///--- 		
///--- 		setRegAddr(AREA_M,SECTOR_ADDR);
///--- 		setRegDataIn(0x0,0x0,0x0);
///--- 		FlashPatExec("patFlash.mpa", "BI_SREAD_CKBD");
///--- }
///--- void tb_redundancy_sector_erase_low(){
///--- 		int area=AREA_R_MIN;
///--- 		for(int i=0; i<AREA_R_NUM; i++){
///--- 				area=area<<i;
///--- 				setRegAddr(area,0x0);
///--- 				FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
///--- 		}
///--- }
///--- void tb_rtst_stress(){
///--- 		setRegAddr(AREA_M_R,0x0);
///--- 		setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 		FlashPatExec("patFlash.mpa", "RTST_STRESS");
///--- }
///--- void tb_main_2x8rows_check_error(){
///--- 
///--- 		FlashPatExec("patFlash.mpa", "CHECK_ERROR_MAIN_BFRE");
///--- }
///--- void tb_vread1_sector_erase_retry_p2(){
///--- 		FlashPatExec("patFlash.mpa", "VREAD_SECT_ERAS_RETRY");
///--- }
///--- void tb_vread1_sector_erase_retry_p3(){
///--- 		FlashPatExec("patFlash.mpa", "VREAD_SECT_ERAS_RETRY");
///--- }
///--- void tb_vread1_sector_erase_retry_p4(){
///--- 		bool Pflag[DDUTCNT]={0};
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 		FlashPatExec("patFlash.mpa","FLASH_SH_VREAD_01");
///--- 
///--- 		for(int i=0;i<10;i++){
///--- 				setRegAddr(AREA_M,0x0);
///--- 				setRegDataIn(0xff,0xffffffff,0xffffffff);
///--- 				FlashPatExec("patFlash.mpa", "VREAD_SECT_ERAS_RETRY");
///--- 				printf("count:%d",i);
///--- 				if(UTL_ReadMeasResult(dut)==UT_RES_PASSED)
///--- 				{
///--- 						Pflag[dut]=1;	
///--- 						i=11;
///--- 				}	
///--- 		}
///--- 		FlashPatExec("patFlash.mpa","FLASH_SH_VREAD_00");
///--- 		END_DUT_LOOP
///--- 
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			if(Pflag[dut]==1)
///--- 			
///--- 				SetFinalResultOnly(dut,PASS);
///--- 			else 
///--- 				SetFinalResultOnly(dut,FAIL);
///--- 		END_DUT_LOOP
///--- 
///--- }








