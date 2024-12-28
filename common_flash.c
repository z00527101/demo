#include "common.h"

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1: variable define
bool  isRepair[DDUTCNT]={0};
int g_mode =-1; //--  is used in FlashPatStart, please cover by lily with FlashPatStart
char Flash_TM0[][10]={"EF0_TM0","EF1_TM0","EF2_TM0"};

int VEG_trim[DDUTCNT][FLASH_NUM]={0};
int VCG_trim[DDUTCNT][FLASH_NUM]={0};
int VSL_trim[DDUTCNT][FLASH_NUM]={0};
int GOX_trim[DDUTCNT][FLASH_NUM]={0};
int ENDU_trim[DDUTCNT][FLASH_NUM]={0};
int COX_trim[DDUTCNT][FLASH_NUM]={0};

///--- int GOX_trim[DDUTCNT][FLASH_NUM]={0};
///--- int ENDURANCE_trim[DDUTCNT][FLASH_NUM]={0};
///--- int COX_trim[DDUTCNT][FLASH_NUM]={0};
int REF_trim[DDUTCNT][FLASH_NUM]={0};
int MRG0_trim[DDUTCNT][FLASH_NUM]={0};
int MRG1_trim[DDUTCNT][FLASH_NUM]={0};
int VREAD1_trim[DDUTCNT][FLASH_NUM]={0};

unsigned int XYh32[DDUTCNT]={0};
unsigned int XYl32[DDUTCNT]={0};
unsigned int idh32[DDUTCNT]={0};
unsigned int idl32[DDUTCNT]={0};
int ADD_LOC_ECC[DDUTCNT]={0};
int WAFERID_ECC[DDUTCNT]={0};
//for customer nvr write
int WAFER_LOT_ID_ECC[DDUTCNT]={0};
int COORD_WAFER_ID_ECC[DDUTCNT]={0};

unsigned int LOTID_h32[DDUTCNT]={0};
unsigned int LOTID_h32_nc_reload[DDUTCNT]={0};
unsigned int LOTID_l32[DDUTCNT]={0};
unsigned int LOTID_l32_nc_reload[DDUTCNT]={0};
unsigned int WAFERID_h32[DDUTCNT]={0};
unsigned int WAFERID_h32_nc_reload[DDUTCNT]={0};
unsigned int WAFERID_l32[DDUTCNT]={0};
unsigned int WAFERID_l32_nc_reload[DDUTCNT]={0};
unsigned short  LOTID_ECC[DDUTCNT]={0};
unsigned short  WAFERID_XY_ECC[DDUTCNT]={0};


int CBD0[DDUTCNT][FLASH_NUM]={0xffff7fc9};
int CBD1[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD2[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD3[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD4[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD5[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD6[DDUTCNT][FLASH_NUM]={0xffffffff};
int CBD7[DDUTCNT][FLASH_NUM]={0xffffffff};

int CBD0_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD1_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD2_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD3_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD4_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD5_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD6_ECC[DDUTCNT][FLASH_NUM]={0};
int CBD7_ECC[DDUTCNT][FLASH_NUM]={0};
int CP1_ID_DATA_ECC=0;
int CP2_ID_DATA_ECC=0;
int CP3_ID_DATA_ECC=0;
int CP2_RETEST_ID_DATA_ECC=0;
int CP3_RETEST_ID_DATA_ECC=0;

int GOX_CBD4[DDUTCNT][FLASH_NUM]={0xffffffff};
int GOX_CBD5[DDUTCNT][FLASH_NUM]={0xffffffff};
int ENDU_CBD4[DDUTCNT][FLASH_NUM]={0xffffffff};
int ENDU_CBD5[DDUTCNT][FLASH_NUM]={0xffffffff};
int COX_CBD2[DDUTCNT][FLASH_NUM]={0xffffffff};
int COX_CBD3[DDUTCNT][FLASH_NUM]={0xffffffff};
                    
int Column_fail_sec_reg[DDUTCNT][FLASH_NUM][3];
int Column_fail_sec_num[DDUTCNT][FLASH_NUM];
int fail_sect_num[DDUTCNT][FLASH_NUM]={0};
int fail_sect_addr[DDUTCNT][FLASH_NUM][4]={0};
int fail_sect_addr1[DDUTCNT][FLASH_NUM]={0};
int fail_sect_addr2[DDUTCNT][FLASH_NUM]={0};
int fail_sect_count[DDUTCNT][FLASH_NUM]={0};

//-- vpp leakage current offset
double VPP_VEG_leakage[DDUTCNT][FLASH_NUM]={0};
double VPP_VCG_leakage[DDUTCNT][FLASH_NUM]={0};


///--- int repair_flag_data[DDUTCNT]={0};
///--- int retry_num=0;

//--*****************************************************************************************************
//--*****************************************************************************************************
/*part1*********************** Register value*********************/ 

void setRegFlashNo(int flash_no){
		setRegValue(UT_REG_ZH, flash_no);
}
void setRegRDNAreaLoop(unsigned area_start, unsigned int area_num, unsigned int addr){
		int loop=area_num;
		setRegValue(UT_REG_D2C, area_start<<8);//--set for Y15-8
		setRegValue(UT_REG_D1C, addr);//--set for X23-0
		if(loop==1)setRegValue(UT_REG_CFLG, 0x1);
		else if(loop>=2){setRegValue(UT_REG_CFLG, 0x0); setRegValue(UT_REG_IDX1, loop>=2?loop-2:0);}
		else{printf("wrong setting when set multi addr reg!!!"); UTL_StopFct();}
}
void setRegMultiAddr(unsigned addr_area, unsigned int addr_start, unsigned int addr_stop, unsigned int addr_step){
		int loop=(addr_stop-addr_start)/addr_step+1;
		setRegValue(UT_REG_D2C, addr_area<<8);//--set for Y15-8
		setRegValue(UT_REG_D1C, addr_start);//--set for X23-0
		setRegValue(UT_REG_D1B, addr_step);
//--		printf("loop:%d\n",loop);
		if(loop==1)setRegValue(UT_REG_CFLG, 0x1);
		else if(loop>=2){setRegValue(UT_REG_CFLG, 0x0); setRegValue(UT_REG_IDX1, loop-2);}
		else{printf("wrong setting when set multi addr reg!!!"); UTL_StopFct();}
}
void setRegAddr(unsigned addr_area)
{
		setRegValue(UT_REG_D2C, addr_area<<8);//--set for Y15-8
	//--	X address bit 24bit in UBM_UBM pattern
		setRegValue(UT_REG_IDX5,24-(ROW_SHIFT_BIT+1)-2);//--bit23 ~ bit row shift  +1	,used in UBM_UBM pattern
		setRegValue(UT_REG_IDX4,ROW_SHIFT_BIT-2);//--bit row shift -1 ~ bit0	,used in UBM_UBM pattern
}
void setRegAddrNUM(unsigned addr_area,int addnum)
{
		setRegValue(UT_REG_D2C, addr_area<<8);//--set for Y15-8
	//--	X address bit 24bit in UBM_UBM pattern
		setRegValue(UT_REG_IDX5,24-(ROW_SHIFT_BIT+1)-2);//--bit23 ~ bit row shift  +1	,used in UBM_UBM pattern
		setRegValue(UT_REG_IDX4,ROW_SHIFT_BIT-2);//--bit row shift -1 ~ bit0	,used in UBM_UBM pattern

		if(addnum==1)setRegValue(UT_REG_CFLG, 0x1);
		else if(addnum>=2){setRegValue(UT_REG_CFLG, 0x0); setRegValue(UT_REG_IDX1, addnum-2);}
		else{printf("wrong setting when set multi addr reg!!!"); UTL_StopFct();}


}
void setRegAddr(unsigned addr_area, unsigned int addr){
		setRegValue(UT_REG_CFLG, 0x1);
		setRegValue(UT_REG_D2C, addr_area<<8);//--set for Y15-8
		setRegValue(UT_REG_D1C, addr);//--set for X23-0
}
void setRegAddr_D1D(unsigned addr_area, unsigned int addr){
		setRegValue(UT_REG_CFLG, 0x1);
		setRegValue(UT_REG_D2C, addr_area<<8);//--set for Y15-8
		setRegValue(UT_REG_D1D, addr);//--set for X23-0
}
void setRegDataIn(unsigned int data3, unsigned int data2, unsigned int data1){//--data3=bit71:bit64,data2=bit63-bit32,data1=bit31-bit0
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs (h, UT_REG_TPH2, 2, data3>>4 & 0xf );               	//--bit71-68--TP2(35:32)
	UTL_SetRegUs (h, UT_REG_TPH2, 1, (data3 & 0xf)<<28 | data2>>4 ); 	//--bit67-36--TP2(31:0)
	UTL_SetRegUs (h, UT_REG_TPH1, 2, data2 & 0xf );                   	//--bit35-32--TP1(35:32)
	UTL_SetRegUs (h, UT_REG_TPH1, 1, data1 );                         	//--bit31-0 --TP1(31:0)
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);
}
void setRegDataIn_TPHB(unsigned int data3, unsigned int data2, unsigned int data1){//--data3=bit71:bit64,data2=bit63-bit32,data1=bit31-bit0
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs (h, UT_REG_TPH2B, 2, data3>>4 & 0xf );               	//--bit71-68--TP2(35:32)
	UTL_SetRegUs (h, UT_REG_TPH2B, 1, (data3 & 0xf)<<28 | data2>>4 ); 	//--bit67-36--TP2(31:0)
	UTL_SetRegUs (h, UT_REG_TPH1B, 2, data2 & 0xf );                   	//--bit35-32--TP1(35:32)
	UTL_SetRegUs (h, UT_REG_TPH1B, 1, data1 );                         	//--bit31-0 --TP1(31:0)
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);
}
void setRegDataOut(unsigned int data3, unsigned int data2, unsigned int data1){//--data3=bit71:bit64,data2=bit63-bit32,data1=bit31-bit0
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs (h, UT_REG_TPH2, 2, data3>>4 & 0xf );               	//--bit71-68--TP2(35:32)
	UTL_SetRegUs (h, UT_REG_TPH2, 1, (data3 & 0xf)<<28 | data2>>4 ); 	//--bit67-36--TP2(31:0)
	UTL_SetRegUs (h, UT_REG_TPH1, 2, data2 & 0xf );                   	//--bit35-32--TP1(35:32)
	UTL_SetRegUs (h, UT_REG_TPH1, 1, data1 );                         	//--bit31-0 --TP1(31:0)
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);
}
void setRegDataOut_TPHB(unsigned int data3, unsigned int data2, unsigned int data1){//--data3=bit71:bit64,data2=bit63-bit32,data1=bit31-bit0
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs (h, UT_REG_TPH2B, 2, data3>>4 & 0xf );               	//--bit71-68--TP2(35:32)
	UTL_SetRegUs (h, UT_REG_TPH2B, 1, (data3 & 0xf)<<28 | data2>>4 ); 	//--bit67-36--TP2(31:0)
	UTL_SetRegUs (h, UT_REG_TPH1B, 2, data2 & 0xf );                   	//--bit35-32--TP1(35:32)
	UTL_SetRegUs (h, UT_REG_TPH1B, 1, data1 );                         	//--bit31-0 --TP1(31:0)
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);
}
/*part2*********************** Pattern Exec*********************/
void FlashPatExec(char* patfile, char* startlabel){
	if(in_flash_loop)setRegFlashNo(0x1<<(g_flash_no-1));  //--choose dut number 06S8
	if (g_mode==-1 ||g_mode!=mode_flash)
	{
		SetTiming();
		SetPinfmt();
		SetPdsMemory();
		g_mode=mode_flash;
	}

//--		UTL_ExecCalb("testDATA","NORMAL");
		char real_patfile[100];	
		if(OP_RATE==rate_20mhz){sprintf(real_patfile, "20MHZ_%s", patfile);}
		else if(OP_RATE==rate_15mhz){sprintf(real_patfile, "15MHZ_%s", patfile);}
		else if(OP_RATE==rate_10mhz){sprintf(real_patfile, "10MHZ_%s", patfile);}
		else if(OP_RATE==rate_5mhz){sprintf(real_patfile, "5MHZ_%s", patfile);}
		else{printf("the rate is fault, can't set pat file!!!\n"); UTL_Stop();}
		AlpgPatExec(real_patfile, startlabel);
}
void FlashPatStart(char* patfile, char* startlabel){
	if(in_flash_loop)setRegFlashNo(0x1<<(g_flash_no-1));  //--choose dut number 06S8
	if (g_mode==-1 ||g_mode!=mode_flash)
	{
		SetTiming();
		SetPinfmt();
		SetPdsMemory();
		g_mode=mode_flash;
	}
		char real_patfile[100];	
		if(OP_RATE==rate_20mhz){sprintf(real_patfile, "20MHZ_%s", patfile);}
		else if(OP_RATE==rate_15mhz){sprintf(real_patfile, "15MHZ_%s", patfile);}
		else if(OP_RATE==rate_10mhz){sprintf(real_patfile, "10MHZ_%s", patfile);}
		else if(OP_RATE==rate_5mhz){sprintf(real_patfile, "5MHZ_%s", patfile);}
		else{printf("the rate is fault, can't set pat file!!!\n"); UTL_Stop();}
		AlpgPatStart(real_patfile, startlabel);
}
void FlashPatExec_NoWait(char* patfile, char* startlabel){
	if(in_flash_loop)setRegFlashNo(0x1<<(g_flash_no-1));  //--choose dut number 06S8
	if (g_mode==-1 ||g_mode!=mode_flash)
	{
		SetTiming();
		SetPinfmt();
		SetPdsMemory();
		g_mode=mode_flash;
	}
		char real_patfile[100];	
		if(OP_RATE==rate_20mhz){sprintf(real_patfile, "20MHZ_%s", patfile);}
		else if(OP_RATE==rate_15mhz){sprintf(real_patfile, "15MHZ_%s", patfile);}
		else if(OP_RATE==rate_10mhz){sprintf(real_patfile, "10MHZ_%s", patfile);}
		else if(OP_RATE==rate_5mhz){sprintf(real_patfile, "5MHZ_%s", patfile);}
		else{printf("the rate is fault, can't set pat file!!!\n"); UTL_Stop();}
		AlpgPatStartNowait(real_patfile, startlabel);
}
void LogicPatExec(char *patfile)
{
        sprintf(mpa_name, "%s", patfile);
	if (g_mode==-1 ||g_mode!=mode_logic)
	{
		SetDBMPinfmt();
		SetDBMTiming();
		ClearPdsMemory();
		g_mode=mode_logic;
	}
		char pat[128];
		sprintf(pat,"dbmpat/%s",patfile);
		setDbmPointer(patfile);
		FctHandle hFct=UTL_GetFctHandle();
		UTL_SetFctMpatName(hFct,pat);
		UTL_SetFctFailInhibit(hFct,UT_ON);
		UTL_SetFctStartPc(hFct,0x0);
		UTL_MeasFct(hFct);
		UTL_DeleteHandle(hFct);
}

/*part3*********************** Trim related*********************/ 
/*
void Read_triming(int addr, unsigned int *data, int trim[][FLASH_NUM], char *patname)
{
		int flag[REF_trimcount][DDUTCNT][FLASH_NUM] = {0};
		START_FLASH_LOOP
				for (int i = 0; i < REF_trimcount; i++)
				{

						setRegAddr(AREA_M, addr);
						setRegDataIn(0x0, 0x0, data[i]);
						FlashPatExec("patFlash.mpa", patname);
						START_DUT_LOOP(UT_CDUT)
								if(UTL_ReadMeasResult(dut)==UT_RES_PASSED)
										flag[i][dut-1][g_flash_no-1]=0;
								else
										flag[i][dut-1][g_flash_no-1]=1;
						END_DUT_LOOP
				}
		END_FLASH_LOOP
				Meas_read_trim(flag, trim);
}

*/
#if 1
void Read_triming(int addr, int shift, int trim[][FLASH_NUM], char *patname)
{

	int flag[REF_trimcount][DDUTCNT][FLASH_NUM] = {0};
	int i=0,shift_patch=0;
	unsigned int *cfm_data;
	cfm_data=(unsigned int*)malloc(sizeof(unsigned int)*(REF_trimcount));
	ulConfigUbm("SI", C28, C27);
	ulConfigCfm("OUTPUT_PINS", 0, 5,0,0);
	
	//-- Set config data in UBM mem
	shift_patch=(0x1f<<shift)^0xffffffff;
//	printf("shift patch:0x%x\n",shift_patch);	

	START_DUT_LOOP(UT_CDUT) 
		for(i=0;i<REF_trimcount;i++)
	//	{	ulWriteUbm("SI", PROG_DATA_BITSIZE * i, PROG_DATA_BITSIZE*(i+1)-1, 0x0, 0xf,  0xffffffff&(shift_patch|( REF_trim_value[i] << shift)), dut);}
		{	ulWriteUbm("SI", PROG_DATA_BITSIZE * i, PROG_DATA_BITSIZE*(i+1)-1, 0x0, 0xf,  (shift_patch|( REF_trim_value[i] << shift)), dut);}
	END_DUT_LOOP

	START_FLASH_LOOP
	setRegValue(UT_REG_D1C, addr);			//--  read trim config addr
	setRegValue(UT_REG_CFLG, 0x0);			//--  set loop flag 
	setRegValue(UT_REG_IDX1, REF_trimcount-2);	//--  set loop num 

	//--  set read trim Main addr ; if flash have diffrent capacity, can set in if-else condition *********************************** 
	setRegValue(UT_REG_D1D, DPROG_ADDR_1_FLASH3);
	//reset Cfm
	ulPresetCfm();

	FlashPatExec("patFlash.mpa", patname);
	
	START_DUT_LOOP(UT_CDUT)
	ulReadCfm(cfm_data,REF_trimcount,0,REF_trimcount-1,0,0,dut);

	for (int i = 0; i < REF_trimcount; i++)
	{
		flag[i][dut-1][g_flash_no-1]=cfm_data[i];
	}

	END_DUT_LOOP
	END_FLASH_LOOP

	Meas_read_trim(flag, trim);
	
	free(cfm_data);

}
#endif
void Meas_read_trim(int flag[][DDUTCNT][FLASH_NUM],int trim[][FLASH_NUM])
{
		bool find[DDUTCNT][FLASH_NUM]={0};
		START_FLASH_LOOP    //--no dut break
		START_DUT_LOOP(UT_CDUT)
		trim[dut-1][g_flash_no-1]=0;
                        printf("DUT%d FLASH%d\t",dut,g_flash_no);

				for(int i=0;i<32;i++)
				{
						if(!flag[i][dut-1][g_flash_no-1])
					//--			printf("P\t");
								printf("P ");
						else
								printf("F ");
						//--		printf("F\t");
						if(i<31)
						{
						if(fabs(flag[i+1][dut-1][g_flash_no-1]-flag[i][dut-1][g_flash_no-1])==1)
						{
								trim[dut-1][g_flash_no-1]=i;
								find[dut-1][g_flash_no-1]=1;				
						}
						}
                                   if((i==31)&&(flag[i][dut-1][g_flash_no-1]==0))trim[dut-1][g_flash_no-1]=31;
				}
					//--	printf("BTC:%d\n",REF_trim_value[trim[dut-1][g_flash_no-1]]);
						printf("BTC:%d,BTP:%d\n",REF_trim_value[trim[dut-1][g_flash_no-1]],trim[dut-1][g_flash_no-1]+1);
		END_DUT_LOOP
		END_FLASH_LOOP

		START_DUT_LOOP(UT_CDUT)
		START_FLASH_LOOP    //--no dut break
				if(find[dut-1][g_flash_no-1])
							SetFinalResultOnly(dut,PASS);
				else
							SetFinalResultOnly(dut,FAIL);
		                                         break;
				printf("\n");
			
		END_FLASH_LOOP
		END_DUT_LOOP
				printf("\n");

}
void Caltrimvalue(char *trimlabel,double trim_result[][DDUTCNT],int trim[],unsigned int *trim_value, double target,int trimcount)
{
	int trimcode = 0, i = 0;
//	double  hilimit = target + error, lolimit = target -error;
	double meas_best[DDUTCNT]={0};


	START_DUT_LOOP(UT_CDUT)
	 meas_best[dut-1]=99;
		
		printf("\nDUT%2d:  %s  ", dut,trimlabel);
		for(i = 0; i<trimcount; i++){
	        if(trim_result[i][dut-1]>-30)printf(" %.3lf", trim_result[i][dut-1]);else printf(" _  ");
			if(fabs(trim_result[i][dut-1]-target)<fabs(meas_best[dut-1]-target))
			{
				meas_best[dut-1] = trim_result[i][dut-1];
				trim[dut-1] = i;
			}
		}
		printf("BTV:%2.3lf , Posi:%d, BTC:%d\n",meas_best[dut-1],trim[dut-1],trim_value[trim[dut-1]]);
	END_DUT_LOOP

}
void HV_VSIM_Triming_Binary(int testmode)
{
	int addr;
	double HV_target, hlimit, lolimit,wait_time;
	double target = 0, target_delta = 0.01;//target_delta default UA. 
	unsigned int *trim_value;
	int trimcount = HV_trimcount, trim[DDUTCNT] = {0};
	double value[DDUTCNT] = {0};
	double temp[DDUTCNT][10] = {0}, data[10][DDUTCNT] = {0};
	double temp_1, temp_2;
	double delta[DDUTCNT] = {0};
	double (*vpp_offset)[FLASH_NUM];
	long long int active_dut=0;
	GetActiveDut(&active_dut);
	int i=0;
	UT_DUT sysdut;
	int dut_flag[DDUTCNT] = {0};
	int  temp_count = trimcount, otherbits = 0, shift = 0;
	char patname_enter[50] = {'\0'};
	char patname_exit[50] = {'\0'};
	char testlabel[50] = {'\0'};
	double trim_result[HV_trimcount][DDUTCNT] = {0};
	int testcount = 3;
	int start[DDUTCNT] = {0}, stop[DDUTCNT] = {0}, mid[DDUTCNT] = {0},mid_init_position=0;
	int judge=1;

	ulConfigUbm("SI", C28, C27);
	//--judge mode
	switch (testmode)
	{
	case VEG_TestMode:
		addr = VEG_CBD_Addr;	HV_target = VEG_Trim_Target;hlimit = VEG_Trim_Target + VEG_Trim_error;lolimit = VEG_Trim_Target - VEG_Trim_error;trim_value = VEG_trim_value;
		printf("trim target under VEG_TestMode is %f\n",HV_target);
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= VEG_mid_position;vpp_offset= VPP_VEG_leakage;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "VEG");
		break;
	case VCG_TestMode:
		addr = VCG_CBD_Addr;HV_target = VCG_Trim_Target;hlimit = VCG_Trim_Target + VCG_Trim_error;lolimit = VCG_Trim_Target - VCG_Trim_error;trim_value = VCG_trim_value;
		printf("trim target under VCG_TestMode is %f\n",HV_target);
		shift = VCG_code_shift;	wait_time=VCG_Wait_Time;mid_init_position= VCG_mid_position;	vpp_offset=VPP_VCG_leakage;
		strcpy(patname_enter, "PROG_VCG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");
		break;
	case GOX_TestMode:
		addr = VEG_CBD_Addr;HV_target = GOX_Trim_Target;hlimit = GOX_Trim_Target + GOX_Trim_error;lolimit = GOX_Trim_Target - GOX_Trim_error;trim_value = VEG_trim_value;
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= GOX_mid_position;	vpp_offset=VPP_VEG_leakage;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "GOX");
		break;
	case ENDU_TestMode:
		addr = VEG_CBD_Addr;HV_target = ENDU_Trim_Target;hlimit = ENDU_Trim_Target + ENDU_Trim_error;lolimit = ENDU_Trim_Target - ENDU_Trim_error;trim_value = VEG_trim_value;
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= ENDU_mid_position;	vpp_offset=VPP_VEG_leakage;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "ENDURANCE");
		break;
	case COX_TestMode:
		addr = VCG_CBD_Addr;HV_target = COX_Trim_Target;hlimit  = COX_Trim_Target + COX_Trim_error;lolimit = COX_Trim_Target - COX_Trim_error;	trim_value = VCG_trim_value;
		shift = VCG_code_shift;	wait_time=VCG_Wait_Time;mid_init_position= COX_mid_position;	vpp_offset=VPP_VCG_leakage;
		strcpy(patname_enter, "PROG_VCG_TRIM_UBM");	strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");
		break;


	default:
		printf("Please enter correct mode!!");
		UTL_Stop();
		break;
	}
	//--calculate otherbits
	while (temp_count != 0)
	{
		otherbits|=1<<i;
		i++;
		temp_count = temp_count >> 1;
	}
	printf("\npre shift therbits=%x\n",otherbits);
	otherbits=0xffffffff^(otherbits << shift);
	printf("\notherbits=%x\n",otherbits);

START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")

		setHvUnMask("VPP1_PINS");
		setHvVSIM(g_pinlist[vspinno].vsno, HV_target, R32V, M8uA, 1 MA, -1 MA, 8 UA, -8 UA, 5);
		START_DUT_LOOP(UT_CDUT) 
			for (i = 0; i < trimcount; i++)
			{
				trim_result[i][dut-1] = -999;
			}
			start[dut-1] = 0;
			stop[dut-1] = trimcount;
			//-- mid first test step init.
			if(HV_TRIM_SCAN)mid[dut-1]=0;
			else	mid[dut-1] =mid_init_position;
		END_DUT_LOOP
		printf("HV addr:%d\n",addr);
		int fn_flag = 0;
		ExclusionHandle h = UTL_GetExclusionHandle();
		UTL_SetExclusionIgnoreWet(h, UT_OFF);
		UTL_SetExclusionMask(h, UT_OFF);
		while (UTL_GetDutCount(UT_CDUT))
		{
			START_DUT_LOOP(UT_CDUT) 
		//		ulWriteUbm("SI", 0, PROG_DATA_BITSIZE - 1, 0x0, 0x0,  ((0xffffffe0|trim_value[mid[dut - 1]]) << shift), dut);
				ulWriteUbm("SI", 0, PROG_DATA_BITSIZE - 1, 0x0, 0x0,  (otherbits|(trim_value[mid[dut - 1]] << shift)), dut);
			END_DUT_LOOP
			setRegAddr(AREA_M, addr);
			FlashPatExec_NoWait("patFlash.mpa", patname_enter);
			UTL_WaitTime(wait_time); 
			START_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)			
				for (i = 0; i < testcount; i++)
				{
					DcExec(g_pinlist[vspinno].vsname);
					START_DUT_LOOP(UT_CDUT) 
						ReadMeasData(g_pinlist[vspinno].vsname, 1 UA, data[i], dut);
					//-- cut off vpp leakage .   ********************************
					temp[dut-1][i] = data[i][dut-1]-vpp_offset[dut-1][g_flash_no-1];
					END_DUT_LOOP
				}

				START_DUT_LOOP(UT_CDUT) 
					if (trim_result[mid[dut-1]][dut-1] == -999)
					{
							printf("\n    dut%d %.3f %.3f %.3f ",dut,temp[dut-1][0],temp[dut-1][1],temp[dut-1][2]);
						
						getMidValue(temp[dut-1], testcount, trim_result[mid[dut-1]], dut);
					}
				END_DUT_LOOP


			END_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
			UTL_StopFct();
			FlashPatExec("patFlash.mpa", patname_exit);

			START_DUT_LOOP(UT_CDUT)
				delta[dut-1] = trim_result[mid[dut-1]][dut-1] - target;

			//--	binary mode	
				if ((fabs(delta[dut-1])>0.1) && (HV_TRIM_SCAN==0))
				{
					if (trim_result[mid[dut-1]][dut-1] < target)
						stop[dut-1] = mid[dut-1];
					else
						start[dut-1] = mid[dut-1];
					
					temp_1 = trim_result[start[dut-1]][dut-1];
					temp_2 = trim_result[stop[dut-1]][dut-1];
					if (temp_1 != -999 && temp_2 != -999 && temp_1 * temp_2 < 0)
						mid[dut-1] = (stop[dut-1] * temp_1 - start[dut-1] * temp_2) / (temp_1 - temp_2)  ;
					else
					{
						mid[dut-1] = (start[dut-1] + stop[dut-1]) / 2 ;
					}
				if(mid[dut-1]==stop[dut-1])mid[dut-1]=stop[dut-1]-1;
				if(mid[dut-1]==start[dut-1])mid[dut-1]=start[dut-1]+1;
				
					

				}
			//--	step mode	
				else
				{
					if((delta[dut-1] > target_delta)|| HV_TRIM_SCAN==1)
					{
						start[dut-1] = mid[dut-1];
						mid[dut-1] += 1;
					}
					else if (delta[dut-1] < -target_delta)
					{
						stop[dut-1] = mid[dut-1];
						mid[dut-1] -= 1;
					}
				}
				
			
			if(start[dut-1]+1>=stop[dut-1]||((fabs(delta[dut-1])<=target_delta)&&(HV_TRIM_SCAN==0)))	
				{ 
					UTL_AddExclusionDut(h, dut);
				}
			END_DUT_LOOP
			UTL_SetExclusionSetOrReset(h, UT_ON);
			UTL_SendExclusion(h);
		}
		UTL_DeleteHandle(h);
		ReturnActiveDut(active_dut);

		printf("\nGet best trim value:\n");
		Caltrimvalue(testlabel, trim_result, trim, trim_value, target, trimcount);

		setRegAddr(AREA_M, addr);
		START_DUT_LOOP(UT_CDUT)
		
		
//			ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, (0xffffffe0| (trim_value[trim[dut-1]]) << shift), dut);
			ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, (otherbits|(trim_value[trim[dut - 1]] << shift)), dut);
			switch (testmode)
			{
			case VEG_TestMode:
				VEG_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=1;
				break;
			case GOX_TestMode:
				GOX_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=GOX_Judge;
				break;
			case ENDU_TestMode:
				ENDU_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=ENDU_Judge;
				break;
			case VCG_TestMode:
				VCG_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=1;
				break;
			case COX_TestMode:
				COX_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=COX_Judge;
				break;

			default:
				printf("Please enter correct mode!!");
				return;
				break;
			}

		END_DUT_LOOP

	//--reset trim code and measure HV
	if(judge==1)
	{
		FlashPatExec_NoWait("patFlash.mpa", patname_enter);
		UTL_WaitTime(wait_time); 
		double value_temp[DDUTCNT][FLASH_NUM]={0};
		HVVSIM_Measure_Voltage_ByFlash(testlabel,g_pinlist[vspinno].vsname,g_pinlist[vspinno].vsno, lolimit-0.05, value_temp,vpp_offset, g_flash_no);

		UTL_StopFct();
		FlashPatExec("patFlash.mpa", patname_exit);
		START_DUT_LOOP(UT_CDUT) 
		value[dut-1]=value_temp[dut-1][g_flash_no-1];
			UTL_ConvertDutNumber(UT_SITEDUT, dut, UT_SYSDUT, NULL, &sysdut);
			
			printf("\nx:%3d,y:%3d\t,SDUT%d,DUT%d,Flash%d,%s_trim\t%.3f V", g_die_X[dut - 1], g_die_Y[dut - 1], sysdut, dut, g_flash_no,testlabel, value[dut-1]);
			if (value[dut-1] >= lolimit && value[dut-1] <= hlimit)
			{	fn_flag = 0;
				printf("\tP");}
			else
			{	fn_flag = 1;
		   		printf("\tF");	
			dut_flag[dut - 1] |= fn_flag;}
		END_DUT_LOOP
		printf("\n");
	}

END_FLASH_LOOP_BY_PINLIST

	START_DUT_LOOP(UT_CDUT) 
	if ((!dut_flag[dut - 1]) && judge)
		UTL_ResetFinalResultOnly(dut, UT_RES_ALL);
	else
		UTL_SetFinalResultOnly(dut, UT_RES_ALL);
	END_DUT_LOOP
		setHvVSIM("VPP1_PINS",0 V,R8V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	setHvMask("VPP1_PINS");

}
void HV_MVM_Triming_Binary(int testmode)
{
	setHvMVM("VPP1_PINS", M8V, 7 V, 2 V, 5);
	int dut_flag[DDUTCNT]={0};
	int fn_flag=0,i=0;
	double value[DDUTCNT]={0};
	double value1[DDUTCNT][FLASH_NUM]={0};
	double value1_temp[DDUTCNT]={0};
	int addr = VSL_CBD_Addr;
	int trimcount = HV_trimcount, trim[DDUTCNT] = {0};
	int temp_count=trimcount,otherbits=0,shift=0;
	int start[DDUTCNT] = {0}, stop[DDUTCNT] = {0}, mid[DDUTCNT] = {0}, mid_init_position=trimcount/2;
	double target=0, hlimit = 0,lolimit = 0,wait_time=0,k_fact=1.0;
	long long int active_dut=0;
	double trim_result[HV_trimcount][DDUTCNT] = {0};
	unsigned int *trim_value;

	char patname_enter[50] = {'\0'};
	char patname_exit[50] = {'\0'};
	char testlabel[50] = {'\0'};
	int judge=0;


	ulConfigUbm("SI", C28, C27);

	UT_DUT sysdut;
	GetActiveDut(&active_dut);

	//--judge mode
	switch (testmode)
	{
	case VEG_TestMode:
		addr = VEG_CBD_Addr;target=VEG_Trim_Target;	hlimit = VEG_Trim_Target + VEG_Trim_error;lolimit = VEG_Trim_Target - VEG_Trim_error;trim_value = VEG_trim_value;
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= VEG_mid_position;k_fact=MVM_OHM_FACTOR;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "VEG");
//		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VEG");
		break;
	case VCG_TestMode:
		addr = VCG_CBD_Addr;target=VCG_Trim_Target;	hlimit = VCG_Trim_Target + VCG_Trim_error;lolimit = VCG_Trim_Target - VCG_Trim_error;trim_value = VCG_trim_value;
		shift = VCG_code_shift;	wait_time=VCG_Wait_Time;mid_init_position= VCG_mid_position;	k_fact=MVM_OHM_FACTOR;
		strcpy(patname_enter, "PROG_VCG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");
		break;
	case GOX_TestMode:
		addr = VEG_CBD_Addr;target = GOX_Trim_Target;	hlimit = GOX_Trim_Target + GOX_Trim_error;lolimit = GOX_Trim_Target - GOX_Trim_error;trim_value = VEG_trim_value;
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= GOX_mid_position;	k_fact=MVM_OHM_FACTOR;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "GOX");
		break;
	case ENDU_TestMode:
		addr = VEG_CBD_Addr;target= ENDU_Trim_Target; 	hlimit = ENDU_Trim_Target + ENDU_Trim_error;lolimit = ENDU_Trim_Target - ENDU_Trim_error;trim_value = VEG_trim_value;
		shift = VEG_code_shift;	wait_time=VEG_Wait_Time;mid_init_position= ENDU_mid_position;	k_fact=MVM_OHM_FACTOR;
		strcpy(patname_enter, "CHIP_ERAS_VEG_TRIM_UBM");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "ENDURANCE");
		break;
	case COX_TestMode:
		addr = VCG_CBD_Addr;target=COX_Trim_Target;	hlimit  = COX_Trim_Target + COX_Trim_error;lolimit = COX_Trim_Target - COX_Trim_error;	trim_value = VCG_trim_value;
		shift = VCG_code_shift;	wait_time=VCG_Wait_Time;mid_init_position= COX_mid_position;	k_fact=MVM_OHM_FACTOR;
		strcpy(patname_enter, "PROG_VCG_TRIM_UBM");	strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");
		break;
	case VSL_TestMode:
		addr = VSL_CBD_Addr;target=VSL_Trim_Target;	hlimit  = VSL_Trim_Target + VSL_Trim_error;lolimit = VSL_Trim_Target - VSL_Trim_error;	trim_value = VSL_trim_value;
		shift = VSL_code_shift;	wait_time=VSL_Wait_Time;mid_init_position= VSL_mid_position;k_fact=1.0;
		strcpy(patname_enter, "PROG_VSL_TRIM_UBM");	strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VSL");
		break;



	default:
		printf("Please enter correct mode!!");
		UTL_Stop();
		break;
	}

	
	//--calculate otherbits
	while (temp_count != 0)
	{
		otherbits|=1<<i;
		i++;
		temp_count = temp_count >> 1;
	}
	otherbits=0xffffffff^(otherbits << shift);
	printf("\notherbits=%x\n",otherbits);



//--flash loop
START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")

	setHvMVM(g_pinlist[vspinno].vsno, M8V, 7 V, 2 V, 5);

	//-- init
	START_DUT_LOOP(UT_CDUT) 
	UTL_ConvertDutNumber(UT_SITEDUT, dut, UT_SYSDUT, NULL, &sysdut);
	
	for (int i = 0; i < trimcount + 1; i++)
		trim_result[i][dut - 1] = -999;
		start[dut - 1] = 0;
		stop[dut - 1] = trimcount - 1;
		
		if(HV_TRIM_SCAN)mid[dut-1]=0;
		else mid[dut - 1] = mid_init_position;
	END_DUT_LOOP

	//-- get exclusionhandle
	ExclusionHandle h = UTL_GetExclusionHandle();
	UTL_SetExclusionIgnoreWet(h, UT_OFF);
	UTL_SetExclusionMask(h, UT_OFF);

	//-- trim loop
	while (UTL_GetDutCount(UT_CDUT))
	{
		START_DUT_LOOP(UT_CDUT) 
		ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, otherbits|(trim_value[mid[dut - 1]]<<shift), dut);
		END_DUT_LOOP

		setRegAddr(AREA_M, addr);
		setRegDataIn(0xff, 0xffffffff, 0xffffffff);
		FlashPatExec_NoWait("patFlash.mpa", patname_enter);

		UTL_WaitTime(wait_time);
		START_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
		UTL_WaitTime(10 MS);
		DcExec(g_pinlist[vspinno].vsname); 
		START_DUT_LOOP(UT_CDUT)				
		ReadMeasData(g_pinlist[vspinno].vsname, 1 V,  trim_result[mid[dut - 1]], dut);
		trim_result[mid[dut-1]][dut-1] =trim_result[mid[dut-1]][dut-1]*k_fact;
		END_DUT_LOOP

		END_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
		UTL_StopFct();
		FlashPatExec("patFlash.mpa", patname_exit);

		START_DUT_LOOP(UT_CDUT)
		if(HV_TRIM_SCAN==0)
		{
			if (start[dut - 1] >= stop[dut - 1] || fabs(trim_result[mid[dut - 1]][dut - 1] - target) == 0)
			{
			UTL_AddExclusionDut(h, dut);
			UTL_SetExclusionSetOrReset(h, UT_ON);
			UTL_SendExclusion(h);
			}
			else
			{
			if (trim_result[mid[dut - 1]][dut - 1] > target)
				stop[dut - 1] = mid[dut - 1] - 1;
			else
				start[dut - 1] = mid[dut - 1] + 1;
			}
			mid[dut - 1] = (start[dut - 1] + stop[dut - 1]) / 2;
		}
			//-- for debug scan mode
		else 
		{
			if (start[dut - 1] >= stop[dut - 1] )
			{
			UTL_AddExclusionDut(h, dut);
			UTL_SetExclusionSetOrReset(h, UT_ON);
			UTL_SendExclusion(h);
			}
			else
			{
				start[dut - 1] +=  1;
				mid[dut - 1] = start[dut - 1];
			}	
		}
		END_DUT_LOOP
	}

	UTL_DeleteHandle(h);
	ReturnActiveDut(active_dut);
	Caltrimvalue(testlabel, trim_result, trim,trim_value, target, trimcount);
	printf("\n");

	//-- set the best trimvalue to Reg	
	START_DUT_LOOP(UT_CDUT) 

	
	ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, otherbits|(trim_value[trim[dut - 1]] << shift), dut);

			switch (testmode)
			{
			case VEG_TestMode:
				VEG_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=1;
				break;
			case GOX_TestMode:
				GOX_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=GOX_Judge;
				break;
			case ENDU_TestMode:
				ENDU_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=ENDU_Judge;
				break;
			case VCG_TestMode:
				VCG_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=1;
				break;
			case COX_TestMode:
				COX_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=COX_Judge;
				break;
			case VSL_TestMode:
				VSL_trim[dut-1][g_flash_no-1] = trim[dut-1];judge=1;
				break;
			default:
				printf("Please enter correct mode!!");
				return;
				break;
			}

	END_DUT_LOOP
	printf("\n");
	//-- measure again
	if(judge)
	{
	 //--ente pattern
	 setRegAddr(AREA_M, addr);
	 setRegDataIn(0xff, 0xffffffff, 0xffffffff);
	 FlashPatExec_NoWait("patFlash.mpa", patname_enter);

	
	//--reset hlimit/lolimit to judge result	
	 setHvMVM(g_pinlist[vspinno].vsno,M8V ,hlimit,lolimit, 5);
	 UTL_WaitTime(20 MS);

	 START_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
		UTL_WaitTime(5 MS);
		DcExec(g_pinlist[vspinno].vsname);
		START_DUT_LOOP(UT_CDUT) 
			ReadMeasData(g_pinlist[vspinno].vsname, 1 V, value1_temp,dut);
			value1[dut-1][g_flash_no-1]=value1_temp[dut-1]*k_fact;	
		END_DUT_LOOP

	 END_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
	//--exit pattern
	 UTL_StopFct();
	 FlashPatExec("patFlash.mpa", patname_exit);
	
	 START_DUT_LOOP(UT_CDUT) 
		UTL_ConvertDutNumber(UT_SITEDUT, dut, UT_SYSDUT, NULL, &sysdut);
		printf("\nx:%d,y:%d,SDUT%d,DUT%d,Flash%d,%s_trim\t%.3f V",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no,testlabel,value1[dut-1][g_flash_no-1]);

		if(value1[dut-1][g_flash_no-1]>=lolimit&&value1[dut-1][g_flash_no-1]<=hlimit)
		{	fn_flag=0;
			printf("\tP\n");}
		else	
		{	fn_flag=1;
			printf("\tF\n");
		dut_flag[dut-1]|=fn_flag;}//-- for multy flash channel judge
	 END_DUT_LOOP          
	}

END_FLASH_LOOP_BY_PINLIST
	
	//--set final cp pass/fail
	START_DUT_LOOP(UT_CDUT) 
	if (!dut_flag[dut - 1])
		SetFinalResultOnly(dut, PASS);
	else
		SetFinalResultOnly(dut, FAIL);
	END_DUT_LOOP

//	printf("\n");
}
void HV_internal_set(int testmode)
{
	ulConfigUbm("SI", C28, C27);

	char testlabel[50] = {'\0'};
	int (*TEMP_CBD2)[FLASH_NUM]=CBD2;
	int (*TEMP_CBD3)[FLASH_NUM]=CBD3;
	int (*TEMP_CBD4)[FLASH_NUM]=CBD4;
	int (*TEMP_CBD5)[FLASH_NUM]=CBD5;
	

	//--judge mode
	switch (testmode)
	{
	case VEG_TestMode:
		TEMP_CBD4=CBD4;TEMP_CBD5=CBD5 ;strcpy(testlabel, "VEG");
		break;
	case VCG_TestMode:
		TEMP_CBD2=CBD2;TEMP_CBD3=CBD3 ;strcpy(testlabel, "VCG");
		break;
	case GOX_TestMode:
		TEMP_CBD4=GOX_CBD4;TEMP_CBD5=GOX_CBD5;strcpy(testlabel, "GOX");
		break;
	case ENDU_TestMode:
		TEMP_CBD4=ENDU_CBD4;TEMP_CBD5=ENDU_CBD5;strcpy(testlabel, "ENDURANCE");
		break;
	case COX_TestMode:
		TEMP_CBD2=COX_CBD2;TEMP_CBD3=COX_CBD3;strcpy(testlabel, "VCG");
		break;


	default:
		printf("Please enter correct mode!!");
		break;
	}
	START_FLASH_LOOP
		START_DUT_LOOP(UT_CDUT)
			ulWriteUbm("SI",0*PROG_DATA_BITSIZE,1*PROG_DATA_BITSIZE-1, 0x0, 0x0,      CBD0[dut-1][g_flash_no-1], dut);
			ulWriteUbm("SI",1*PROG_DATA_BITSIZE,2*PROG_DATA_BITSIZE-1, 0x0, 0x0,      CBD1[dut-1][g_flash_no-1], dut);
			ulWriteUbm("SI",2*PROG_DATA_BITSIZE,3*PROG_DATA_BITSIZE-1, 0x0, 0x0, TEMP_CBD2[dut-1][g_flash_no-1], dut);
			ulWriteUbm("SI",3*PROG_DATA_BITSIZE,4*PROG_DATA_BITSIZE-1, 0x0, 0x0, TEMP_CBD3[dut-1][g_flash_no-1], dut);
			ulWriteUbm("SI",4*PROG_DATA_BITSIZE,5*PROG_DATA_BITSIZE-1, 0x0, 0x0, TEMP_CBD4[dut-1][g_flash_no-1], dut);
			ulWriteUbm("SI",5*PROG_DATA_BITSIZE,6*PROG_DATA_BITSIZE-1, 0x0, 0x0, TEMP_CBD5[dut-1][g_flash_no-1], dut);
		END_DUT_LOOP
		//--set FLASH CFG CBD0~CBD5 to set HV voltage
		FlashPatExec("patFlash.mpa", "SET_FLASH_CFG_UBM");

	END_FLASH_LOOP

}
void measure_HV_VSIM(int testmode)
{
	char patname_enter[50] = {'\0'};
	char patname_exit[50] = {'\0'};
	char testlabel[50] = {'\0'};
	double trim_data[DDUTCNT][FLASH_NUM] = {0xf};
	double target = 0;
	double (*offset_current)[FLASH_NUM];
	double hlimit=0,lolimit=0,wait_time=25 MS;
	int dut_flag[DDUTCNT] = {0};
	bool fn_flag = 0;
	UT_DUT sysdut;
	switch (testmode)
	{
		case VEG_TestMode:
			//target=VEG_Trim_Target;hlimit=target+VEG_Meas_error;lolimit=target-VEG_Meas_error;wait_time=VEG_Wait_Time;
			target=VEG_Measure_Target;hlimit=target+VEG_Meas_error;lolimit=target-VEG_Meas_error;wait_time=VEG_Wait_Time;
			printf("measure target under VEG_TestMode is %f\n",target);
			strcpy(patname_enter, "MEAS_VEG_QCHI_ERASE");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "VEG");
			offset_current=VPP_VEG_leakage ;break;
		case VCG_TestMode:
			//target=VCG_Trim_Target;hlimit=target+VCG_Meas_error;lolimit=target-VCG_Meas_error;wait_time=VCG_Wait_Time;
			target=VCG_Measure_Target;hlimit=target+VCG_Meas_error;lolimit=target-VCG_Meas_error;wait_time=VCG_Wait_Time;
			printf("measure target under VCG_TestMode is %f\n",target);
			strcpy(patname_enter, "MEAS_VCG_PROG");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");
			offset_current=VPP_VCG_leakage ;break;

		default:
		printf("enter correct mode!!");
		UTL_Stop();
		break;
	}
	setHvMask("VPP1_PINS");
	setHvVSIM("VPP1_PINS",target, R32V, M8uA, 1 MA, -1 MA, 8 UA, -8 UA, 5);

		FlashPatExec_NoWait("patFlash.mpa", patname_enter);
		UTL_WaitTime(wait_time); 

	//-- group measure
	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
//		FlashPatExec_NoWait("patFlash.mpa", patname_enter);
//		UTL_WaitTime(wait_time); 
		//-- measure by VPP pinlist.
		HVVSIM_Measure_Voltage_ByFlash(testlabel, g_pinlist[vspinno].vsname, g_pinlist[vspinno].vsno, lolimit-0.05 V, trim_data,offset_current,g_flash_no);
//		UTL_StopFct();
//		FlashPatExec("patFlash.mpa", patname_exit);

		START_DUT_LOOP(UT_CDUT)
			UTL_ConvertDutNumber(UT_SITEDUT, dut, UT_SYSDUT, NULL, &sysdut);
	
			printf("\nx:%3d,y:%3d\t,SDUT%d,DUT%d,Flash%d,%s_measure\t%.3f V", g_die_X[dut - 1], g_die_Y[dut - 1], sysdut, dut,g_flash_no,testlabel, trim_data[dut-1][g_flash_no-1]);
		if (trim_data[dut-1][g_flash_no-1] >= lolimit && trim_data[dut-1][g_flash_no-1] <= hlimit)
		{	fn_flag = 0;
			printf("\tP\n");}
		else
		{	fn_flag = 1;
			printf("\tF\n");
			dut_flag[dut - 1] |= fn_flag;
		}
		END_DUT_LOOP
		printf("\n");
	END_FLASH_LOOP_BY_PINLIST
		UTL_StopFct();
		FlashPatExec("patFlash.mpa", patname_exit);


	START_DUT_LOOP(UT_CDUT) 
	if (!dut_flag[dut - 1])
		SetFinalResultOnly(dut, PASS);
	else
		SetFinalResultOnly(dut, FAIL);
	END_DUT_LOOP
	setHvVSIM("VPP1_PINS", 0 V, R32V, M8uA, 1 MA, -1 MA, 8 UA, -8 UA, 5);
	setHvMask("VPP1_PINS");

}
void measure_HV_MVM(int testmode)
{
	char patname_enter[50] = {'\0'};
	char patname_exit[50] = {'\0'};
	char testlabel[50] = {'\0'};
	double  data[FLASH_NUM][DDUTCNT];
	double target = 0,k_fact=1.0;
//	double (*offset_current)[FLASH_NUM];
	double hlimit=0,lolimit=0,wait_time=25 MS;
	bool  Fail_flag[DDUTCNT]={0};
	UT_DUT sysdut;
	switch (testmode)
	{
		case 	VEG_TestMode:
				target=VEG_Trim_Target;hlimit=target+VEG_Meas_error;lolimit=target-VEG_Meas_error;k_fact=MVM_OHM_FACTOR;
				strcpy(patname_enter, "MEAS_VEG_QCHI_ERASE");strcpy(patname_exit, "HV_TRIM_EXIT_QERASE");strcpy(testlabel, "VEG");break;
		case 	VCG_TestMode:
				target=VCG_Trim_Target;hlimit=target+VCG_Meas_error;lolimit=target-VCG_Meas_error;k_fact=MVM_OHM_FACTOR;
				strcpy(patname_enter, "MEAS_VCG_PROG");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VCG");break;
		case 	VSL_TestMode:
				target=VSL_Trim_Target;hlimit=target+VSL_Meas_error;lolimit=target-VSL_Meas_error;k_fact=1.0;
				strcpy(patname_enter, "MEAS_VSL_PROG");strcpy(patname_exit, "HV_TRIM_EXIT_PROG");strcpy(testlabel, "VSL");break;
		default:
		printf("enter correct mode!!");
		UTL_Stop();
		break;
	}
	setHvMask("VPP1_PINS");
	setHvMVM("VPP1_PINS",M8V ,hlimit,lolimit, 2);
	SettlingTimeHandle handle_HV = UTL_GetSettlingTimeHandle();
	UTL_SetSettlingTime(handle_HV, 5 MS);
	UTL_SendSettlingTime(handle_HV);
	UTL_DeleteHandle(handle_HV);

		FlashPatExec_NoWait("patFlash.mpa", patname_enter);
		UTL_WaitTime(5 MS);
		if(testmode!=VSL_TestMode)UTL_WaitTime(10 MS);

	//-- group measure
	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
//		FlashPatExec_NoWait("patFlash.mpa", patname_enter);
//		UTL_WaitTime(5 MS);
//		if(testmode!=VSL_TestMode)UTL_WaitTime(10 MS);
		START_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
		UTL_WaitTime(5 MS);
        	        DcExec(g_pinlist[vspinno].vsname);
 			START_DUT_LOOP(UT_CDUT)
				ReadMeasData(g_pinlist[vspinno].vsname,  1 V, data[g_flash_no-1],dut);
				data[g_flash_no-1][dut-1]=data[g_flash_no-1][dut-1]*k_fact;
			END_DUT_LOOP
		END_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
//		UTL_StopFct();
//		FlashPatExec("patFlash.mpa", patname_exit);
	END_FLASH_LOOP_BY_PINLIST
		UTL_StopFct();
		FlashPatExec("patFlash.mpa", patname_exit);

	//--judge.
	START_DUT_LOOP(UT_CDUT)
			UTL_ConvertDutNumber(UT_SITEDUT, dut, UT_SYSDUT, NULL, &sysdut);
	
		START_FLASH_LOOP
			printf("\nx:%3d,y:%3d\t,SDUT%d,DUT%d,Flash%d,%s_measure\t%.3f V", g_die_X[dut - 1], g_die_Y[dut - 1], sysdut, dut,g_flash_no,testlabel, data[g_flash_no-1][dut-1]);
		if (data[g_flash_no-1][dut-1] >= lolimit && data[g_flash_no-1][dut-1] <= hlimit)
		{
			printf("\tP\n");
		}
		else
		{	Fail_flag[dut-1]= 1;
			printf("\tF\n");
		}
		END_FLASH_LOOP

		if(Fail_flag[dut-1]==0)
			SetFinalResultOnly(dut, PASS);
		else
			SetFinalResultOnly(dut, FAIL);
	END_DUT_LOOP

	setHvMVM("VPP1_PINS",M8V ,hlimit,lolimit, 10);
	setHvMask("VPP1_PINS");
}

void HVVSIM_Measure_Voltage_ByFlash(char* item, char*pin,int VSNO, double target,double value[][FLASH_NUM],double vpp_leakage_offset[][FLASH_NUM],int flash_no)
{
	//-- fast search
	double temp[DDUTCNT][FLASH_NUM][10];
	double vdd[DDUTCNT][FLASH_NUM] = {target},vdd_temp=0,i_fabs=0;
	double data[DDUTCNT][FLASH_NUM]={0},data_temp[DDUTCNT]={0};
	double R = 950000;//-- ohm  VEG-VCG load resistorr,950k ohm	
	double target_current = 5.001 NA; 
	int i=0,n=0,N_overflow;
	long long int active_dut=0;
	GetActiveDut(&active_dut);


	ExclusionHandle h=UTL_GetExclusionHandle();
	START_GROUP_TEST_BY_VPP(VSNO)
		UTL_SetExclusionIgnoreWet  (h,UT_OFF);                            
		UTL_SetExclusionMask       (h,UT_OFF);		

		printf("\n........NEW GROUP TEST...............\n");
		for(i=0; i< 7&& UTL_GetDutCount(UT_CDUT) ;i++)
		{   
			//set vdd of each DUT/Branch
			START_DUT_LOOP(UT_CDUT)
				if (i==0)
					vdd[dut-1][g_flash_no-1]=target ;//-- set target voltage at the first time.
				if(vdd[dut-1][g_flash_no-1]>target+2)vdd[dut-1][g_flash_no-1]=target+1;if(vdd[dut-1][g_flash_no-1] < 0) vdd[dut-1][g_flash_no-1]= target-1;//--safe protect.
			END_DUT_LOOP
			//--set voltage for VSIM test	
			setHvVSIM_ByFlashLoop(VSNO,vdd,R32V ,M8uA,8 UA, -8 UA, 1,flash_no);
			//--measure for each branch
			UTL_WaitTime((20+5*i) MS);
			
			DcExec(pin);

			//--read measure data and cal voltage in dut loop
			START_DUT_LOOP(UT_CDUT)
				N_overflow=	ReadMeasData(pin,1 A, data_temp,dut);
                                data[dut-1][g_flash_no-1]=data_temp[dut-1]-vpp_leakage_offset[dut-1][g_flash_no-1];//-- cut out vpp leakage current.
				printf("dut%2d FLASH:%2d %.3fv %.2fnA\t|",dut,flash_no,vdd[dut-1][g_flash_no-1],data[dut-1][g_flash_no-1]/(1 NA));

				//-- calculate new vdd value 
				vdd[dut-1][g_flash_no-1] -=R*data[dut-1][g_flash_no-1];

				//-- judge current zero crossing point. then the target will be the left or right step
				i_fabs= fabs(data[dut-1][g_flash_no-1]);

				if(i>0&(i_fabs<target_current )||i_fabs>(4 UA)||N_overflow==0) 
				{ UTL_AddExclusionDut(h,dut);					
					value[dut-1][g_flash_no-1] = vdd[dut-1][g_flash_no-1];//-- trans measure value 
					if(i_fabs>(4 UA)) printf("\n DUT%d over flow,current %.3f uA",dut,data[dut-1][g_flash_no-1]/(1 UA));
				}
			END_DUT_LOOP
				//delet the measure ok dut from the dut lains.
			UTL_SetExclusionSetOrReset (h,UT_ON);                             
			UTL_SendExclusion(h);        
		}
  	END_GROUP_TEST_BY_VPP(VSNO)
  	UTL_DeleteHandle(h);

	ReturnActiveDut(active_dut);

}
/*part4*********************** Initial id *********************/
void InitialCpDieId_jx2(unsigned  int *LOTID_h32, unsigned  int *LOTID_l32,unsigned  int *WaferID_h32,unsigned int *WaferID_l32)
{
        int i;
        unsigned int lotid[8]={0};
        unsigned int waferid[8]={0};
        char temp_WaferId[20];
	unsigned int wafernum = 0;
        strcpy(temp_WaferId, WaferId);
        char *p = strtok(temp_WaferId,"-");
	printf("%s,%s\n",p,__FUNCTION__);
        p = strtok(NULL,"-");
	if(p)
			wafernum = atoi(p);
	else
			wafernum = 0;
	printf("wafernum is %d\n",wafernum);
//	printf("%s,%s\n",p,__FUNCTION__);
//	printf("%d,%s\n",atoi(p),__FUNCTION__);
//	printf("0x%x,%s\n",atoi(p),__FUNCTION__);
#if 0 
       if(p)
        {
	    int len=strlen(p);
            waferid[7] = ConvertCharToInt(p,1)&0xff;	
            waferid[6] = ConvertCharToInt(p,0)&0xff;
            printf("waferid[7] is %d\n",waferid[7]);
            printf("waferid[7] is 0x%x\n",waferid[7]);
            //waferid[7] = ConvertCharToInt(p,1)&0xff;	
            //printf("waferid[6] is %d\n",waferid[6]);
            //printf("waferid[6] is 0x%x\n",waferid[6]);
	}
#endif   
        //lotid[0]=0x30;
        lotid[0]=0xff;
        //lotid[1]=0x30;
        lotid[1]=0xff;
	//printf("lotid[0] is %d\n",lotid[0]);
	printf("\nlotid[0] is 0x%x\n",lotid[0]);
	//printf("lotid[1] is %d\n",lotid[1]);
	printf("lotid[1] is 0x%x\n",lotid[1]);
	for(i=2;i<8;i++)
	{ lotid[i] = ConvertCharToInt(LotNumber, i-2)&0xff;
          //printf("lotid[%d] is %d\n",i,lotid[i]);
          printf("lotid[%d] is 0x%x\n",i,lotid[i]);
          }

	START_DUT_LOOP(UT_CDUT)
            waferid[7] = g_die_Y[dut-1];
            waferid[6] = g_die_X[dut-1];
            waferid[5] = wafernum; 
            waferid[4] = 0x0; 
            waferid[3] = g_die_Y[dut-1];
            waferid[2] = g_die_X[dut-1]; 
            waferid[1] = wafernum;
            waferid[0] = 0x0;
	for(i=0;i<8;i++)
{
      printf("waferid[%d] is 0x%x\n",i,waferid[i]);
}
printf("\n");
            unsigned char test_buff_ecid[]={lotid[0],lotid[1],lotid[2],lotid[3],lotid[4],lotid[5],lotid[6],lotid[7],
                                            waferid[0],waferid[1],waferid[2],waferid[3],waferid[4],waferid[5],waferid[6],waferid[7]};
	    for(i=0;i<16;i++)
{
	    printf("0x%x\t",test_buff_ecid[i]); 
}
            LOTID_h32[dut-1]|=(lotid[0] << 24)|(lotid[1] << 16)|(lotid[2] << 8)|(lotid[3]);
            LOTID_l32[dut-1]|=(lotid[4] << 24)|(lotid[5] << 16)|(lotid[6] << 8)|(lotid[7]);
            WAFERID_h32[dut-1]|=(waferid[0] << 24)|(waferid[1] << 16)|(waferid[2] << 8)|(waferid[3]);
            WAFERID_l32[dut-1]|=(waferid[4] << 24)|(waferid[5] << 16)|(waferid[6] << 8)|(waferid[7]);
            printf("\nLOTID_h32 is 0x%x\n",LOTID_h32[dut-1]);
            printf("LOTID_l32 is 0x%x\n",LOTID_l32[dut-1]);
            printf("WAFERID_h32 is 0x%x\n",WAFERID_h32[dut-1]);
            printf("WAFERID_l32 is 0x%x\n",WAFERID_l32[dut-1]);
	END_DUT_LOOP
}


void InitialCpDieId(unsigned  int *XYh32, unsigned  int *XYl32,unsigned int *idh32,unsigned int *idl32)
{
		unsigned int lotid0 = ConvertCharToInt(LotNumber, 0)&0xff; 
		unsigned int lotid1 = ConvertCharToInt(LotNumber, 1)&0xff;
		unsigned int lotid2 = ConvertCharToInt(LotNumber, 2)&0xff;
		unsigned int lotid3 = ConvertCharToInt(LotNumber, 3)&0xff;
		unsigned int lotid4 = ConvertCharToInt(LotNumber, 4)&0xff;
		unsigned int lotid5 = ConvertCharToInt(LotNumber, 5)&0xff;


		char temp_WaferId[20];
		unsigned int wafernum = 0;
		strcpy(temp_WaferId, WaferId);
		char *p = strtok(temp_WaferId,"-");
		p = strtok(NULL,"-"); //--get the wafer number
		if(p)
				wafernum = atoi(p);
		else
				wafernum = 0;
		START_DUT_LOOP(UT_CDUT)
				XYh32[dut-1] = ((g_die_X[dut-1]&0xffff)<<16)|(g_die_Y[dut-1]&0xffff);
				XYl32[dut-1]=(0xffff<<16)|0x0001;
		idl32[dut-1] =	wafernum&0xffff|(lotid0<<16)|(lotid1<<24); 
		idh32[dut-1] =	lotid2|(lotid3<<8)|(lotid4<<16)|(lotid5<<24); 
//--		printf("XYh32:0x%x",XYh32[dut-1]);
//--		printf("idl32:0x%x",idl32[dut-1]);
		END_DUT_LOOP
}
/*part5*********************** 1R1C*********************/
void ConfigCFM_Sector()
{
	int sector_start_bit = 0, sector_end_bit=0,temp=0;	
//--	int sector_start_bit = 0, sector_end_bit=FLASH_NUM,temp=0;	

	    sector_start_bit = log2(SECT_SHIFT_ADDR);	
	    temp = ((M_SECT_NUM-1)*SECT_SHIFT_ADDR);

	while(temp!=0)
	{
		sector_end_bit++;
		temp=temp>>1;
		}
//	printf("sector_end_bit %d\n",sector_end_bit);	
	ulConfigCfm("OUTPUT_PINS", sector_start_bit, sector_end_bit,0,0);
	ulPresetCfm();

}
void ReadCFM_Sector()
{
	int i;
	unsigned int *cfm_data;
	cfm_data=(unsigned int*)malloc(sizeof(unsigned int)*(M_SECT_NUM));
	
	START_DUT_LOOP(UT_CDUT)
	Column_fail_sec_num[dut-1][g_flash_no-1]=0;
	ulReadCfm(cfm_data,M_SECT_NUM,0,M_SECT_NUM-1,0,0,dut);
		for(i=0;i<M_SECT_NUM;i++){
			if(cfm_data[i]!=0){ 
				printf("\n////////////dut:%d,fail sector:0x%x",dut,i);

				//--FAIL sector num&addr save
				if(Column_fail_sec_num[dut-1][g_flash_no-1]<3)Column_fail_sec_reg[dut-1][g_flash_no-1][Column_fail_sec_num[dut-1][g_flash_no-1]]=i;
 				Column_fail_sec_num[dut-1][g_flash_no-1]++;
				printf("\tfail sector num:%d/////////////\n",Column_fail_sec_num[dut-1][g_flash_no-1]);
			}
		}
	END_DUT_LOOP
free(cfm_data);
}
/*part6*********************** repair related*********************/
void tb_clear_bist_fail_reg(){
		FlashPatExec("patFlash.mpa", "FLASH_BI_CLEAR_FAIL");
}
void tb_main_check_error(){
	START_FLASH_LOOP
		FlashPatExec("patFlash.mpa", "CHECK_ERROR_MAIN_BFRE");
	END_FLASH_LOOP
}
void tb_nvr_cfg_program_repaired_rows(){

		unsigned int data[DDUTCNT][FLASH_NUM][48];
                int fail_sect_addr[DDUTCNT][FLASH_NUM][2]={0};
                int fail_word_addr[DDUTCNT][FLASH_NUM][2]={0};
		unsigned int repair_flag_data[DDUTCNT][FLASH_NUM]={0};
		int fail_sect_ECC[DDUTCNT][FLASH_NUM][4]={0};	//--only for ECC solution
		int fail_flag_ECC[DDUTCNT][FLASH_NUM]={0};	//--only for ECC solution
		int repair_flag_ECC[DDUTCNT][FLASH_NUM]={0};	//--only for ECC solution
		int HMask_bit=0;

		if(REPAIR_MASK_H!=0xffffffff)HMask_bit=1;	

		ulConfigFcm("SO", 0, 6, 8192);
	START_FLASH_LOOP
		ulPresetFcm();
		
		FlashPatStart("patFlash.mpa", "CHECK_SORT2_BFRE");
		START_DUT_LOOP(UT_CDUT)
		int fail_sect_addr1=0x0;
		int fail_sect_addr2=0x0;
		int fail_word_addr1=0x0;
		int fail_word_addr2=0x0;
		 
		fail_sect_ECC[dut-1][g_flash_no-1][0]=0xffffffff;
		fail_sect_ECC[dut-1][g_flash_no-1][1]=0xffffffff;
		repair_flag_ECC[dut-1][g_flash_no-1]=0xffffffff;//--init repair para
		fail_sect_addr[dut-1][g_flash_no-1][0]=0x0;
		fail_sect_addr[dut-1][g_flash_no-1][1]=0x0;
	
			int i=0;
			int bist_fail3=0, bist_fail2=0, bist_fail1=0;
			ulReadFcm(data[dut-1][g_flash_no-1], 48, 0, 0, 0, 47, dut);

			bist_fail3=data[dut-1][g_flash_no-1][5];
			bist_fail2=data[dut-1][g_flash_no-1][26];
			bist_fail1=data[dut-1][g_flash_no-1][47];
		
			//for(i=38;i>=31;i--){
			for(i=(21+SECT_ADDR_BIT+SECT_SHIFT_BIT);i>=(22+SECT_SHIFT_BIT);i--){
			//for(i=(22+SECT_ADDR_BIT+SECT_SHIFT_BIT);i>=(22+SECT_SHIFT_BIT);i--){
				//fail_sect_addr2|=data[dut-1][g_flash_no-1][47-i]<<(i-31);
				fail_sect_addr2|=data[dut-1][g_flash_no-1][47-i]<<(i-22-SECT_SHIFT_BIT);
			}
			//for(i=17;i>=10;i--){
			for(i=SECT_ADDR_BIT+SECT_SHIFT_BIT;i>=1+SECT_SHIFT_BIT;i--){
			//for(i=1+SECT_ADDR_BIT+SECT_SHIFT_BIT;i>=1+SECT_SHIFT_BIT;i--){
			//for(i=17;i>=8;i--){
				//fail_sect_addr1|=data[dut-1][g_flash_no-1][47-i]<<(i-10);
				fail_sect_addr1|=data[dut-1][g_flash_no-1][47-i]<<(i-1-SECT_SHIFT_BIT);
			}
		
			//for(i=38;i>=22;i--){
			for(i=(21+SECT_ADDR_BIT+SECT_SHIFT_BIT);i>=22;i--){
			//for(i=(22+SECT_ADDR_BIT+SECT_SHIFT_BIT);i>=22;i--){
				fail_word_addr2|=data[dut-1][g_flash_no-1][47-i]<<(i-22);
			}
			//for(i=17;i>=1;i--){
			for(i=SECT_ADDR_BIT+SECT_SHIFT_BIT;i>=1;i--){
			//for(i=1+SECT_ADDR_BIT+SECT_SHIFT_BIT;i>=1;i--){
				fail_word_addr1|=data[dut-1][g_flash_no-1][47-i]<<(i-1);
			}
		//--case fail num=0: set repair flag and fail sec addr
		if(bist_fail1==0)
			{fail_sect_addr[dut-1][g_flash_no-1][0]=0xffffffff;fail_sect_addr[dut-1][g_flash_no-1][1]=0xffffffff;
				//--*******************repair flag case fail num=0; ******************
			repair_flag_data[dut-1][g_flash_no-1]=0xffffffff;} 

		else if(bist_fail2==0){fail_sect_addr[dut-1][g_flash_no-1][1]=0xffffffff;}
		//--case fail num>0: set repair flag  
		if(bist_fail1==1)
		{
		isRepair[dut-1]=1;
			//--case fail num=1:
		
			fail_sect_addr[dut-1][g_flash_no-1][0]=fail_sect_addr1;
			fail_sect_addr[dut-1][g_flash_no-1][1]=0xffffffff;
				//--*******************repair flag case fail num=1; edit from checklist rull******************
			repair_flag_data[dut-1][g_flash_no-1]=CP1_ID_DATA_REPAIR;
		
			//--case fail num=2:
			if(bist_fail2==1)
			{
			fail_sect_addr[dut-1][g_flash_no-1][1]=fail_sect_addr2;
				//--*******************repair flag case fail num=1;  edit from checklist rull******************
			repair_flag_data[dut-1][g_flash_no-1]=CP1_ID_DATA_REPAIR;
			}

		}
		if(bist_fail1)printf("DUT[%d]Flash[%d]\tfail1 secaddr:0x%x,wordaddr:0x%x;",dut,g_flash_no,fail_sect_addr[dut-1][g_flash_no-1][0],fail_word_addr1);
		if(bist_fail2)printf("fail2 secaddr:0x%x,wordaddr:0x%x;",fail_sect_addr[dut-1][g_flash_no-1][1],fail_word_addr2);
		if(bist_fail3)printf("bist fail3:0x1");
		if(bist_fail1)printf("\n");
		END_DUT_LOOP

		ulConfigUbm("SI", C28, C27);

		setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW, and add+data group (IDX1) for  address loop num
		START_DUT_LOOP(UT_CDUT)

		if(NC_ECC_FLAG)		//ECC data function in 72bit 
		{
		  fail_sect_ECC[dut-1][g_flash_no-1][0]=CalculateECC(REPAIR_MASK_H ,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][0]);	
		  fail_sect_ECC[dut-1][g_flash_no-1][1]=CalculateECC(REPAIR_MASK_H ,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][1]);	
		  repair_flag_ECC[dut-1][g_flash_no-1]=CalculateECC(0xffffffff,repair_flag_data[dut-1][g_flash_no-1]);	
		}

	
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",R0_ADDR	    , NC_ECC_FLAG|HMask_bit,fail_sect_ECC[dut-1][g_flash_no-1][0] ,REPAIR_MASK_H ,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][0],  dut);
 		Ubm_Write_Push("SI",R1_ADDR	    , NC_ECC_FLAG|HMask_bit,fail_sect_ECC[dut-1][g_flash_no-1][1] ,REPAIR_MASK_H ,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][1],  dut);
 		Ubm_Write_Push("SI",REPAIR_FLAG_ADDR, NC_ECC_FLAG,repair_flag_ECC[dut-1][g_flash_no-1]  ,0xffffffff ,repair_flag_data[dut-1][g_flash_no-1] ,  dut);

		END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PRO_REPAIRED_ROWS_UBM");
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");

		ulConfigUbm("SIO_PINS", C28, C27);
		setRegAddr(AREA_NC);//--set 3 group add+data/IDX1 loop num  for  NVR_CFG_READ_UBM_UBM
		START_DUT_LOOP(UT_CDUT)
			Ubm_Read_Push("SIO_PINS",0,-1,0, 0,0, dut);//--init push func
			Ubm_Read_Push("SIO_PINS",R0_ADDR	 ,0,fail_sect_ECC[dut-1][g_flash_no-1][0],REPAIR_MASK_H,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][0], dut);
			Ubm_Read_Push("SIO_PINS",R1_ADDR         ,0,fail_sect_ECC[dut-1][g_flash_no-1][1],REPAIR_MASK_H,REPAIR_MASK_L|fail_sect_addr[dut-1][g_flash_no-1][1], dut);
			Ubm_Read_Push("SIO_PINS",REPAIR_FLAG_ADDR,0,repair_flag_ECC[dut-1][g_flash_no-1] , 0xffffffff,repair_flag_data[dut-1][g_flash_no-1], dut);
		END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_READ_REPAIRED_ROWS_UBM");
		FlashPatExec("patFlash.mpa", "NVR_CFG_READ_UBM_UBM");
	END_FLASH_LOOP


}
void tb_read_repair_addr(){

		int i=0,bitwith=0,temp=DATA_BITSIZE;
		int flag=0;
		unsigned int fail_data[DDUTCNT][FLASH_NUM][DATA_BITSIZE*2]={0};
		while(temp!=0)
		{
		bitwith++;
		temp=temp>>1;
		}

//	printf("bitwith:%d",bitwith);
		ulConfigFcm("SO", 1, bitwith, 8192);
//		ulConfigFcm("SO", 1, 6, 64*DATA_BITSIZE);
		ulConfigUbm("SI", C28, C27);


		//-- Set config data in UBM mem
		START_DUT_LOOP(UT_CDUT) 
			ulWriteUbm("SI", 0  			, UBM_ADDR_BITSIZE*1-1, 0x0, 0x0,  R0_ADDR, dut);
			ulWriteUbm("SI", UBM_ADDR_BITSIZE*1  	, UBM_ADDR_BITSIZE*2-1, 0x0, 0x0,  R1_ADDR, dut);
		END_DUT_LOOP


	START_FLASH_LOOP
		ulPresetFcm();

		setRegAddrNUM(AREA_NC,2);
		FlashPatExec("patFlash.mpa", "NVR_CFG_READ_UBM_MMA");
		START_DUT_LOOP(UT_CDUT)

			unsigned int fail_sect_addr1=0;
		        unsigned int fail_sect_addr2=0;
			ulReadFcm(fail_data[dut-1][g_flash_no-1],DATA_BITSIZE*2, 0, 1, 0, DATA_BITSIZE-1, dut);
			for(i=DATA_BITSIZE/2;i<DATA_BITSIZE;i++){
				fail_sect_addr1=fail_sect_addr1|(fail_data[dut-1][g_flash_no-1][i]<<(DATA_BITSIZE-1-i));
			}

			for(i=DATA_BITSIZE*3/2;i<DATA_BITSIZE*2;i++){
				fail_sect_addr2|=fail_data[dut-1][g_flash_no-1][i]<<(DATA_BITSIZE*2-1-i);
			}
		
			fail_sect_addr[dut-1][g_flash_no-1][0]=fail_sect_addr1&0x1ff;
			fail_sect_addr[dut-1][g_flash_no-1][1]=fail_sect_addr2&0x1ff;
			printf("x:%d,y:%d\t,DUT%d flash:%d,fail sec addr1:0x%x\t,fail2:0x%x\t;",g_die_X[dut-1], g_die_Y[dut-1],dut,g_flash_no,fail_sect_addr1,fail_sect_addr2);
			if(fail_sect_addr1!=0xffffffff)
				{fail_sect_num[dut-1][g_flash_no-1]=1;
				printf("repair sectaddr1:0x%x,",fail_sect_addr[dut-1][g_flash_no-1][0]);
				}
			if(fail_sect_addr2!=0xffffffff)
				{fail_sect_num[dut-1][g_flash_no-1]=2;
				printf("sectaddr2:0x%x,",fail_sect_addr[dut-1][g_flash_no-1][1]);
				}
			if(fail_sect_num[dut-1][g_flash_no-1]>0)printf("fail_sect_num:%d\n ",fail_sect_num[dut-1][g_flash_no-1]);
			printf("\n");
		END_DUT_LOOP
		FlashPatStart("patFlash.mpa", "FLASH_SH_RECAL_00");
	END_FLASH_LOOP

		START_DUT_LOOP(UT_CDUT)
				SetFinalResultOnly(dut,PASS);
		END_DUT_LOOP
printf("\n");
	



}
void tb_main_check_sort2_error(){

	unsigned int data[DDUTCNT][FLASH_NUM][48];
	int bist_fail3=0, bist_fail2=0, bist_fail1=0;
	int i=0;	
	int flag_new=0;
	ulConfigFcm("SO", 0, 6, 8192);
	START_FLASH_LOOP
		ulPresetFcm();
	//	FlashPatExec("patFlash.mpa", "CHECK_SORT2_BFRE");
		FlashPatExec("patFlash.mpa", "FN_NORL_READ_BFAIL_REG_MMA");
		START_DUT_LOOP(UT_CDUT)
			ulReadFcm(data[dut-1][g_flash_no-1], 48, 0, 0, 0, 47, dut);
		END_DUT_LOOP
	END_FLASH_LOOP
	START_DUT_LOOP(UT_CDUT)

	 flag_new=0;


		START_FLASH_LOOP
	int fail2_sect_addr=0, fail1_sect_addr=0;
	int retry_num=0;

			bist_fail3=data[dut-1][g_flash_no-1][5];
			bist_fail2=data[dut-1][g_flash_no-1][26];
			bist_fail1=data[dut-1][g_flash_no-1][47];
	
			for(i=46;i>=43;i--){
				retry_num|=data[dut-1][g_flash_no-1][47-i]<<(i-43);
			}
			//for(i=38;i>=31;i--){
			for(i=(21+SECT_ADDR_BIT+SECT_SHIFT_BIT);i>=(22+SECT_SHIFT_BIT);i--){
				//fail2_sect_addr|=data[dut-1][g_flash_no-1][47-i]<<(i-31);
				fail2_sect_addr|=data[dut-1][g_flash_no-1][47-i]<<(i-22-SECT_SHIFT_BIT);
			}
			//for(i=17;i>=10;i--){
			for(i=SECT_ADDR_BIT+SECT_SHIFT_BIT;i>=1+SECT_SHIFT_BIT;i--){
				//fail1_sect_addr|=data[dut-1][g_flash_no-1][47-i]<<(i-10);
				fail1_sect_addr|=data[dut-1][g_flash_no-1][47-i]<<(i-1-SECT_SHIFT_BIT);
			}




			printf("\nDUT%d flash%d",dut,g_flash_no);
			printf("retry_num:%d ,",retry_num);
			printf("fail bist1:%d ,",bist_fail1);
			printf("bist2:%d ,",bist_fail2);
			printf("bist3:%d\n ,",bist_fail3);
			if(bist_fail1)	printf("fail1 sectaddr:0x%x",fail1_sect_addr);
			if(bist_fail2)	printf("fail2 sectaddr:0x%x\n",fail2_sect_addr);
						


		if(bist_fail3){
				flag_new=1;
				printf("fail sect num > 2, can't repair.");
			}
				if(fail_sect_num[dut-1][g_flash_no-1]==2) 
			{
					if(bist_fail2 && fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][1] &&  fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0])
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail2_sect_addr);
				}
				
					if(bist_fail1 && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][1] && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0])
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail1_sect_addr);
				}
			
			}
			else if(fail_sect_num[dut-1][g_flash_no-1]==1)  
			{

					if( bist_fail1 && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0]  )
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail1_sect_addr);
				}
					if(bist_fail2 && fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0]  )
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail2_sect_addr);
				}
						
					
			}
			else if(fail_sect_num[dut-1][g_flash_no-1]==0)   
			{
			
					if(bist_fail1)
				{  flag_new=1;
					printf("fail new sector addr: 0x%x ",fail1_sect_addr);
					if(bist_fail2)printf(",0x%x",fail2_sect_addr);
					printf("\n");
				}
					if(bist_fail2 )
				{	flag_new=1;
					printf(" new sector addr: 0x%x\n",fail2_sect_addr);
				}
			
			
			
			}

		END_FLASH_LOOP

		if(flag_new)
				SetFinalResultOnly(dut,FAIL);
		else
				SetFinalResultOnly(dut,PASS);

	END_DUT_LOOP
}
void tb_check_sort2_1column_error_main_array()
{
		START_DUT_LOOP(UT_CDUT)
		int flag_new=0;
		START_FLASH_LOOP
		//-- to check 1 column new fail sector addr.
		if(Column_fail_sec_num[dut-1][g_flash_no-1]>2 ){
				flag_new=1;
				printf("column fail sect num > 2, can't repair\n");
			}
			
			else 	flag_new = check_new_sect_addr(Column_fail_sec_num[dut-1][g_flash_no-1],Column_fail_sec_reg[dut-1][g_flash_no-1][0],Column_fail_sec_reg[dut-1][g_flash_no-1][1],dut); 
	

		END_FLASH_LOOP


		if(flag_new!=0)
				SetFinalResultOnly(dut,FAIL);
		else
				SetFinalResultOnly(dut,PASS);
		
		END_DUT_LOOP

}


/*part7*********************** Data process*********************/
void ReadReg_STB_BFAIL_REG(int select_STB_or_BFAIL)
{	//--define select_STB_or_BFAIL 1: BFAIL REG read, others: STB reg read
	unsigned int DATA3[1][DDUTCNT][FLASH_NUM],DATA2[1][DDUTCNT][FLASH_NUM],DATA1[1][DDUTCNT][FLASH_NUM];
	if(select_STB_or_BFAIL==1)
	{
		printf("BFAIL_READ\n");
		FCM_READ_DATA_BITSIZE=48;
		ReadFlash(AREA_M, 0x0, 0x0,0x1,0x0,DATA3,DATA2,DATA1,"FN_NORL_READ_BFAIL_REG_MMA");
			START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
				temp2[dut-1][g_flash_no-1]=DATA2[0][dut-1][g_flash_no-1];
				temp1[dut-1][g_flash_no-1]=DATA1[0][dut-1][g_flash_no-1];
			END_DUT_LOOP
			END_FLASH_LOOP
		FCM_READ_DATA_BITSIZE=READ_DATA_BITSIZE;
	}
	else
	{
		printf("STB_READ\n");
		FCM_READ_DATA_BITSIZE=64;
	//	ReadFlash(AREA_M, 0x0, 0x0,0x1,0x0,DATA3,DATA2,DATA1,"READ_STB_REG_FCM");
		ReadFlash(AREA_M, 0x0, 0x0,0x1,0x0,DATA3,DATA2,DATA1,"READ_STB_REG_MMA");
			START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
				temp2[dut-1][g_flash_no-1]=DATA2[0][dut-1][g_flash_no-1];
				temp1[dut-1][g_flash_no-1]=DATA1[0][dut-1][g_flash_no-1];
			END_DUT_LOOP
			END_FLASH_LOOP
		FCM_READ_DATA_BITSIZE=READ_DATA_BITSIZE;
	}
}
void ReadFlash(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname)
{
	int ybits = 0, xbits=0;
	int transfer_bits = 13;
	int tempyaddr=FCM_READ_DATA_BITSIZE-1;
	int round_startaddr=(startaddr/(1<<transfer_bits))*(1<<transfer_bits);
	int tempxaddr=endaddr-round_startaddr;
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

	unsigned int i,j,d,fn;
	unsigned int *fcm_data=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits) * (1<<ybits));
	unsigned int *d1=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d2=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int *d3=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));

 START_FLASH_LOOP
	setRegValue(UT_REG_D2C, area<<8);//--set for Y15-8
	setRegValue(UT_REG_D1C, round_startaddr|addr_mask);//--set for X23-0
	setRegValue(UT_REG_D1B, stepaddr);
	setRegDataIn(0x0,0x0,0x0);

	RegHandle h = UTL_GetRegHandle();
	if((xbits+ybits) > transfer_bits){                                                                                                                                                                                                                  
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<(transfer_bits-ybits))/stepaddr - 2);  //-- bytes for each fcm transfer//error
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 0);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, (1<<(xbits+ybits-transfer_bits))-2);  // transfer count - 1
	}
	else{
	    UTL_SetRegUs (h, UT_REG_IDX1, 1, (1<<xbits)/stepaddr - 2 );  //-- bytes for each fcm transfer
	    UTL_SetRegUs (h, UT_REG_CFLG, 1, 2);
	    UTL_SetRegUs (h, UT_REG_IDX2, 1, 0              );  //-- transfer count - 1
	}
	UTL_SendReg  (h);
	UTL_DeleteHandle(h);

	ulPresetFcm();
//--	FlashPatExec("patFlash.mpa", "FN_NORL_READ_FCM");
	FlashPatStart("patFlash.mpa", patname);
	

	
	START_DUT_LOOP(UT_CDUT)
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			memset(d1, 0, sizeof(unsigned int)*(1<<xbits));//--bit31:0
			memset(d2, 0, sizeof(unsigned int)*(1<<xbits));//--bit63:32
			memset(d3, 0, sizeof(unsigned int)*(1<<xbits));//--bit71:64
			ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, endaddr-round_startaddr, 0, (1<<ybits)-1, dut);	
			for(i=startaddr-round_startaddr;i<=endaddr-round_startaddr;i++)
			{
				for(j=0;j<(FCM_READ_DATA_BITSIZE<32?FCM_READ_DATA_BITSIZE:32);j++)
				{
					d1[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<j;
				}
				for(j=32;j<(FCM_READ_DATA_BITSIZE<64?FCM_READ_DATA_BITSIZE:64);j++)
				{
					d2[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-32);
				}
				for(j=64;j<(FCM_READ_DATA_BITSIZE<72?FCM_READ_DATA_BITSIZE:72);j++)
				{
					d3[i] |= (((fcm_data[i*(1<<ybits)+FCM_READ_DATA_BITSIZE-1-j]))&1)<<(j-(1<<ybits));
				}
				DATA3[i][dut-1][g_flash_no-1]=d3[i];
				DATA2[i][dut-1][g_flash_no-1]=d2[i];
				DATA1[i][dut-1][g_flash_no-1]=d1[i];
		//--		printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, 0, d3[i], d2[i], d1[i]);fflush(stdout);
				printf("DUT[%i]\tFLASH[%d] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut,g_flash_no, 0, d3[i], d2[i], d1[i]);fflush(stdout);
			}
END_DUT_LOOP
END_FLASH_LOOP
	free(fcm_data);
		free(d1);
		free(d2);
		free(d3);
}
int check_new_sect_addr(int fail_num,int fail1_sect_addr ,int fail2_sect_addr,int dut)
{
	int   flag_new=0;
		
			 if(fail_sect_num[dut-1][g_flash_no-1]==2) //--record 2 fail sector in CP1
			{
					if((fail_num==2) && fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][1] &&  fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0])
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail2_sect_addr);
				}
				
					if((fail_num>0) && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][1] && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0])
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail1_sect_addr);
				}
			
			}
			else if(fail_sect_num[dut-1][g_flash_no-1]==1)  //--record 1 fail sector in CP1
			{
					if( (fail_num>0) && fail1_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0]  )
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail1_sect_addr);
				}
					if((fail_num==2) && fail2_sect_addr != fail_sect_addr[dut-1][g_flash_no-1][0]  )
				{	flag_new=1;
					printf("fail new sector addr: 0x%x\n",fail2_sect_addr);
				}
						
			}
			else if(fail_sect_num[dut-1][g_flash_no-1]==0)   //--record 0 fail sector in CP1
			{
					if(fail_num>0)
				{  flag_new=1;
					printf("fail new sector addr: 0x%x ",fail1_sect_addr);
					if(fail_num>1)printf(",0x%x",fail2_sect_addr);
					printf("\n");
				}
			
			}

				
	
return flag_new;


}


//--debug cell current

void tb_cell_current()
{

    	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 5);
	ulSetSettlingTime(5 MS,5 MS,3 MS);//t:MVM/VSIM t1/t2
	int i,j,m,vddflag=0;
	//int add[6]={0x7b,0x7f,0x4080,0x7f80,0x2080,0x7f7};
	//int add[6]={0x7af,0x7b3,0x7b7,0x7ef,0x7f3,0x7f7};
	int add[6]={0x07b3,0x07f3,0x1fb3,0x1ff3,0x7f,0x7b};
	unsigned int addr,bit_high_low,bit_mask;
	double temp[72][DDUTCNT][FLASH_NUM]={0};
	double temp_BUF[72][DDUTCNT*FLASH_NUM][5]={0};
	//		ReadFLASH(AREA_M,0x7b3);
	//		ReadFLASH(AREA_M,0x7f3);
	// 
	//	for (j=0x7b3;j<0xfffff;j++)
	//		for (j=0x780;j<0x800;j++)
//	for (j=0;j<0x3F;j++)
	//for (j=1;j<5;j++)
	
FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
				UTL_WaitTime(5 MS);
				UTL_StopFct();			


	for (j=1;j<4;j++)
	{     
		addr=add[j];
	//	addr=j;
		//addr=0x31;
		printf("\naddr:0x%x",addr);

		bit_high_low=0x800000;
		//setRegAddr(AREA_M,addr|bit_high_low);
		setRegAddr(AREA_R0,addr|bit_high_low);
		//setRegAddr(AREA_N,addr|bit_high_low);
//		FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
//				UTL_WaitTime(5 MS);
//				UTL_StopFct();			

		//START_FLASH_LOOP
			for(i=0;i<72;i++)	 
			{
				//	setRegDataIn(0xff,0xffff,~(0x1<<i));	
				if(i>35)bit_high_low=0x400000; 
				else bit_high_low=0x800000;

				if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
				else if(i>31 && i<36)  setRegDataIn(0xff,~(0x1<<(i-32)),0xffffffff);	
				else if(i>35 && i<68) setRegDataIn(0xff,0xffff,~(0x1<<(i-36)));
				else setRegDataIn(0xff,~(0x1<<(i-68)),0xffffffff);

				setRegAddr(AREA_M,addr|bit_high_low);
				//setRegAddr(AREA_NC,addr|bit_high_low);
		//		FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
				FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT_SET");
				UTL_WaitTime(2 MS);
				DcExec("TM0");
		START_FLASH_LOOP
//				DcExec(Flash_TM0[g_flash_no-1]);

				START_DUT_LOOP(UT_CDUT)//init data 
					ReadMeasData_ByFlash(Flash_TM0[g_flash_no-1], 1 UA, temp[i], dut, g_flash_no);
				//	temp_BUF[i][dut-1][m]=temp[i][dut-1];
				END_DUT_LOOP

		END_FLASH_LOOP
				UTL_StopFct();			
		//		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT");
				//	UTL_WaitTime(15 MS);
			}

		//	UTL_StopFct();			
		//	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT");
	//	END_FLASH_LOOP

			//	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
			//	FlashPatExec("patFlash.mpa","TMODE_EXIT");
			START_DUT_LOOP(UT_CDUT)//init data 
		START_FLASH_LOOP
				printf("\naddr:0x%x 72IO DUT%d_F-Nu%d(uA):\t",addr,dut, g_flash_no);
				for(i=0;i<72; i++)printf("|%.2f|,",temp[i][dut-1][g_flash_no-1]);
		END_FLASH_LOOP
			END_DUT_LOOP

	}

				FlashPatExec("patFlash.mpa","TMODE_EXIT");
	printf("\n");		
}



//--***********************************************************************************************************************************
//-- bake delete rows
//--void measure_VSL_program(double hlimit, double lolimit)
//--{
//--	setHvUnMask("VPP1_PINS");
//--	setHvMVM("VPP1_PINS",M8V ,hlimit,lolimit, 5);
//--	SettlingTimeHandle handle_HV = UTL_GetSettlingTimeHandle();
//--	UTL_SetSettlingTime(handle_HV, 10 MS);
//--	UTL_SendSettlingTime(handle_HV);
//--	UTL_DeleteHandle(handle_HV);
//--	UT_DUT sysdut;
//--	double temp[DDUTCNT][FLASH_NUM]={0}, data[DDUTCNT]={0},data1[FLASH_NUM][DDUTCNT];
//--	bool  Fail_flag[DDUTCNT]={0};
//--
//--	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
//--		FlashPatExec_NoWait("patFlash.mpa","MEAS_VSL_PROG");
//--
//--		UTL_WaitTime(50 MS);
//--		START_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
//--        	        DcExec(g_pinlist[vspinno].vsname);
//-- 			START_DUT_LOOP(UT_CDUT)
//--				ReadMeasData(g_pinlist[vspinno].vsname,  1 V, data1[g_flash_no-1],dut);
//--				temp[dut-1][g_flash_no-1]= data1[g_flash_no-1][dut-1];			
//--
//--			END_DUT_LOOP
//--		END_GROUP_TEST_BY_VPP(g_pinlist[vspinno].vsno)
//--		UTL_StopFct();
//--		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_PROG");
//--
//--                 
//--	END_FLASH_LOOP_BY_PINLIST
//--
//--	START_DUT_LOOP(UT_CDUT)
//--		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
//--		START_FLASH_LOOP
//--			if(temp[dut-1][g_flash_no-1]>hlimit||temp[dut-1][g_flash_no-1]<lolimit)
//--		{	Fail_flag[dut-1]=1;
//--			printf("\nx:%d,y:%d\t,SDUT%d,DUT%d,FLASH:%d,measure_VSL%.3f V\tF\n",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, temp[dut-1][g_flash_no-1]);
//--		}	
//--			else
//--	{		printf("\nx:%d,y:%d\t,SDUT%d,DUT%d,FLASH:%d,measure_VSL%.3f V\tP\n",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, temp[dut-1][g_flash_no-1]);
//--	}
//--	END_FLASH_LOOP
//--	END_DUT_LOOP
//--	START_DUT_LOOP(UT_CDUT)
//--		if(Fail_flag[dut-1]==0)
//--				SetFinalResultOnly(dut,PASS);
//--			else	
//--				SetFinalResultOnly(dut,FAIL);
//--
//--	END_DUT_LOOP
//--	setHvMask("VPP1_PINS");
//--	setHvMVM("VPP1_PINS",M8V ,hlimit,lolimit, 10);
//--}




































///--- double HV_measure_VEG_temp[DDUTCNT][FLASH_NUM]={0};

///--- void measure_VEG_VSIM(double hlimit,double lolimit)
///--- {
///--- 
///--- 	double trim_data[DDUTCNT][FLASH_NUM]={0xf};
///--- 	double VEG_target=4.8 V;
///--- 	bool Fail_flag[DDUTCNT]={0};
///--- 	UT_DUT sysdut;
///--- 	SettlingTimeHandle handle_HV = UTL_GetSettlingTimeHandle();
///--- 	UTL_SetSettlingTimeAfterRon(handle_HV, 5 MS);
///--- 	UTL_SetSettlingTimeAfterRof(handle_HV, 3 MS);
///--- 	UTL_SendSettlingTime(handle_HV);
///--- 	UTL_DeleteHandle(handle_HV);
///--- 
///--- 
///--- 	setHvVSIM("VPP1_PINS",VEG_target ,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
///--- 	setHvUnMask("VPP1_PINS");
///--- 	START_FLASH_LOOP_BY_PINLIST("VPP1_PINS")
///--- 		FlashPatExec_NoWait("patFlash.mpa","MEAS_VEG_CHIP_ERASE");
///--- 		UTL_WaitTime(15 MS);  
///--- 	
///--- 		HVVSIM_Measure_Voltage_ByFlash("VEG HV",g_pinlist[vspinno].vsname,vspinno,VEG_target , trim_data, g_flash_no);
///--- 		UTL_StopFct();
///--- 		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
///--- 
///--- 		START_DUT_LOOP(UT_CDUT)
///--- 			UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
///--- 			HV_measure_VEG_temp[dut-1][g_flash_no-1]=trim_data[dut-1][g_flash_no-1];
///--- 			if(trim_data[dut-1][g_flash_no-1]>=lolimit&&trim_data[dut-1][g_flash_no-1]<=hlimit)
///--- 				printf("\nx:%d,y:%d\t,SDUT%d,DUT%d FLASH%d VEG measure\t%.3f pass",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, trim_data[dut-1][g_flash_no-1]);
///--- 			else	{
///--- 				printf("\nx:%d,y:%d\t,SDUT%d,DUT%d FLASH%d VEG measure\t%.3f fail",g_die_X[dut-1], g_die_Y[dut-1],sysdut,dut,g_flash_no, trim_data[dut-1][g_flash_no-1]);
///--- 				Fail_flag[dut-1]=1;
///--- 				}
///--- 		END_DUT_LOOP
///--- 	END_FLASH_LOOP_BY_PINLIST
///--- 	START_DUT_LOOP(UT_CDUT)
///--- 			if(Fail_flag[dut-1]==0)
///--- 				SetFinalResultOnly(dut,PASS);
///--- 			else	
///--- 				SetFinalResultOnly(dut,FAIL);
///--- 	END_DUT_LOOP
///--- 	setHvVSIM("VPP1_PINS", 0 V ,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
///--- 	setHvMask("VPP1_PINS");
///--- }

///--- void Caltrimvalue(char *trimlabel,double trim_result[][DDUTCNT],int trim[],double target,int trimcount)
///--- {
///--- 	int trimcode = 0, i = 0;
///--- 	double meas_best[DDUTCNT]={0};
///--- 
///--- 
///--- 	START_DUT_LOOP(UT_CDUT)
///--- 	 meas_best[dut-1]=99;
///--- 		
///--- 		printf("\nDUT%2d:  %s  ", dut,trimlabel);
///--- 		for(i = 0; i<trimcount; i++){
///--- 	        if(trim_result[i][dut-1]>-30)printf(" %.3lf", trim_result[i][dut-1]);else printf(" _  ");
///--- 			if(fabs(trim_result[i][dut-1]-target)<fabs(meas_best[dut-1]-target))
///--- 			{
///--- 				meas_best[dut-1] = trim_result[i][dut-1];
///--- 				trim[dut-1] = i;
///--- 			}
///--- 		}
///--- 		printf("BTV:%2.3lf , Posi:%d\n",meas_best[dut-1],trim[dut-1]);
///--- 	END_DUT_LOOP
///--- 
///--- }
