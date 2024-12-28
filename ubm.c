#include "common.h"
#include "flash_info.h"

//-- write ubm data sequency: low bit -> high bit
void ulWriteUbm_L32(char *pinlist, int startaddr, int stopaddr,  unsigned int data1, int dut)
{
	unsigned int ubmdata[32];
	int i, d;
	UbmAccessHandle h = UTL_GetUbmAccessHandle();

	for (i = 0; i < 32; i++){
		//ubmdata[31-i] = ((data1>>i)&1)?0xffffffff:0;
		ubmdata[i] = ((data1>>i)&1)?0xffffffff:0;//low bit first out
	}

	UTL_SetUbmAccessDut      (h, dut);
	UTL_SetUbmAccessPin      (h, pinlist);
	UTL_SetUbmAccessStartAddr(h, startaddr+1);
	UTL_SetUbmAccessStopAddr (h, stopaddr+1);
	UTL_WriteUbm             (h, ubmdata, sizeof(ubmdata));
//	printf("DUT[%d]UBM DATA: addr %x-%x, data %x\n", dut, startaddr, stopaddr, data1);

	UTL_DeleteHandle(h);
}


//-- write ubm data sequency: high bit -> low bit
void ulWriteUbm(char *pinlist, int startaddr, int stopaddr, unsigned int data3, unsigned int data2, unsigned int data1, int dut)
{
 	unsigned int ubmdata[72];
 	int i, d;
 	UbmAccessHandle h = UTL_GetUbmAccessHandle();
 	
 	for (i = 0; i < 32; i++){
 	 	if(JTAG_SEQ==0) ubmdata[71-i] = ((data1>>i)&1)?0xffffffff:0;
		else	ubmdata[i] = ((data1>>i)&1)?0xffffffff:0;
 	}
 	for (i = 32; i < 64; i++){
 	    	if(JTAG_SEQ==0)	ubmdata[71-i] = ((data2>>i-32)&1)?0xffffffff:0;
		else 	ubmdata[i] = ((data2>>i-32)&1)?0xffffffff:0;
 	}
 	for (i = 64; i < 72; i++){
 	    	if(JTAG_SEQ==0) ubmdata[71-i] = ((data3>>i-64)&1)?0xffffffff:0;
		else	ubmdata[i] = ((data3>>i-64)&1)?0xffffffff:0;
 	}
    
    UTL_SetUbmAccessDut      (h, dut);
    UTL_SetUbmAccessPin      (h, pinlist);
    UTL_SetUbmAccessStartAddr(h, startaddr+1);
    UTL_SetUbmAccessStopAddr (h, stopaddr+1);
    UTL_WriteUbm             (h, ubmdata+(72-(stopaddr-startaddr+1)), sizeof(ubmdata));
//--	printf("DUT[%d]UBM DATA: addr %x-%x, data %x-%x-%x\n", dut, startaddr, stopaddr, data3,data2,data1);

    UTL_DeleteHandle(h);
}

void ulPresetUbm(USlider data)
{
	UbmAccessHandle h = UTL_GetUbmAccessHandle();
	UTL_PresetUbm(h, data);
	UTL_DeleteHandle(h);
}
void ulConfigUbm(char *pinlist, int func_select_cbit, int jump_select_cbit) 
{
	ulDisableUbm();
	UbmConfigHandle h = UTL_GetUbmConfigHandle();
	UTL_InitializeUbmConfigHandle (h);
	UTL_AddUbmConfigFunctionSelect(h, UT_UBM_UNIT_PM_EXP, func_select_cbit);
	UTL_SetUbmConfigPin           (h, UT_UBM_UNIT_PM_EXP, pinlist);
	UTL_SetUbmConfigJumpAddr      (h, UT_UBM_UNIT_PM_EXP, 0, 0);
	UTL_SetUbmConfigJumpCycle     (h, UT_UBM_UNIT_PM_EXP, 0, jump_select_cbit);
	UTL_SetUbmConfigPmMode        (h, UT_UBM_MODE_DRPAT);
	UTL_SendUbmConfig(h);
	UTL_DeleteHandle(h);
}
void ulDisableUbm(void)
{
	UbmConfigHandle h = UTL_GetUbmConfigHandle();
	UTL_ClearUbmConfigPin           (h, UT_UBM_UNIT_PM);
	UTL_ClearUbmConfigPin           (h, UT_UBM_UNIT_SCRESULTMEMORY);
	UTL_ClearUbmConfigPin           (h, UT_UBM_UNIT_EXP);
	UTL_ClearUbmConfigFunctionSelect(h);
	UTL_SendUbmConfig(h);
	UTL_DeleteHandle(h);
}

//--UBM push stack function for Flash read and write
void Ubm_Write_Push(char *pinlist,unsigned int flash_addr, int reset_and_datanum, unsigned int data3, unsigned int data2,unsigned int data1, int dut)
{
	static int ubmaddr[DDUTCNT]={0},addnum=0;
	int ipshift=PROG_DATA_BITSIZE-32;
//-- reset_and_datanum: -1, reset;   0: 1data with 1 flash addr,          1: 2 data(  addr+ low bits data, addr Mask low addr + high bits data) 
	if(reset_and_datanum==-1){ ubmaddr[dut-1]=0;addnum=0;}
	else if(reset_and_datanum==0)
	{
	ulWriteUbm( pinlist,  ubmaddr[dut-1]				, ubmaddr[dut-1]+24-1				,  0,  0,  flash_addr,  dut);
	ulWriteUbm( pinlist,  ubmaddr[dut-1]+ 24			, ubmaddr[dut-1]+PROG_DATA_BITSIZE+24-1	, data3, data2,  data1 ,  dut);
	ubmaddr[dut-1]+=24+PROG_DATA_BITSIZE;
	addnum+=1;
	}
	else if(reset_and_datanum==1)	//-- to set 72bit IP ( high/low 36 bits data ) and 64bit IP (high/low 32 bits data),can't compatable with 150bits IP
	{
	ulWriteUbm( pinlist,  ubmaddr[dut-1]				, ubmaddr[dut-1]+24-1				,  0,  0,  flash_addr,  dut);	//+addr low
	ulWriteUbm( pinlist,  ubmaddr[dut-1]+ 24			, ubmaddr[dut-1]+PROG_DATA_BITSIZE  +24  -1	,  0, data2,  data1,  dut);		//+data low bits
	ulWriteUbm( pinlist,  ubmaddr[dut-1]+ PROG_DATA_BITSIZE+24	, ubmaddr[dut-1]+PROG_DATA_BITSIZE  +24*2-1,  0,  0,  flash_addr^ADDR_HL_MASK_BIT,  dut);//+addr high
	ulWriteUbm( pinlist,  ubmaddr[dut-1]+ PROG_DATA_BITSIZE+24*2	, ubmaddr[dut-1]+PROG_DATA_BITSIZE*2+24*2-1,  0,  data3>>ipshift,(data2>>ipshift)|(data3<<(32-ipshift)),dut);//+data high bits
	ubmaddr[dut-1]+=24*2+PROG_DATA_BITSIZE*2;
	addnum+=2;
	}
	else 
	{
	printf("Ubm_Write_Push para reset_and_datanum illegal.");
	}	
	//set groups num 
	if(addnum==1 || addnum==0)setRegValue(UT_REG_CFLG, 0x1);
	else if(addnum>=2){setRegValue(UT_REG_CFLG, 0x0); setRegValue(UT_REG_IDX1, addnum-2);}
	else{printf("wrong setting when set multi addr reg!!!"); UTL_StopFct();}

}
void Ubm_Read_Push(char *pinlist,unsigned int flash_addr, int reset_and_datanum, unsigned int data3, unsigned int data2,unsigned int data1, int dut)
{
	static int ubmaddr[DDUTCNT]={0},addnum=0;
//-- reset_and_datanum: -1, reset;   0: 1data with 1 flash addr,           
	if(reset_and_datanum==-1){ ubmaddr[dut-1]=0;addnum=0;}
	else if(reset_and_datanum==0)
	{
	ulWriteUbm( pinlist,  ubmaddr[dut-1]				, ubmaddr[dut-1]+24-1				,  0,  0,  flash_addr,  dut);
	ulWriteUbm( pinlist,  ubmaddr[dut-1]+ 24			, ubmaddr[dut-1]+READ_DATA_BITSIZE+24-1	,  data3,  data2,  data1 ,  dut);
	ubmaddr[dut-1]+=24+READ_DATA_BITSIZE;
	addnum+=1;
	}

	else 
	{
	printf("Ubm_Read_Push para reset_and_datanum illegal.");
	}
	//set groups num 
	if(addnum==1 || addnum==0)setRegValue(UT_REG_CFLG, 0x1);
	else if(addnum>=2){setRegValue(UT_REG_CFLG, 0x0); setRegValue(UT_REG_IDX1, addnum-2);}
	else{printf("wrong setting when set multi addr reg!!!"); UTL_StopFct();}


}






