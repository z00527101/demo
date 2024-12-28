#include "common.h"

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1: variable define
unsigned int temp1[DDUTCNT][FLASH_NUM]={0};
unsigned int temp2[DDUTCNT][FLASH_NUM]={0};
unsigned int temp3[DDUTCNT][FLASH_NUM]={0};
//int FCM_READ_DATA_BITSIZE=DATA_BITSIZE;

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2: debug test block
void debug_ReadReg_Print()
{
	printf("\n");
	ReadReg_STB_BFAIL_REG(0);
	START_DUT_LOOP(UT_CDUT)
	START_FLASH_LOOP
		printf("DUT[%i] FLASH%d STB REG data: 0x%08X_%08X ", dut, g_flash_no, temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);fflush(stdout);
		
//		if((temp3[dut-1]==0x0)&&((temp2[dut-1]|0xff0==0xff8))&&(temp1[dut-1]|0xf00000==0x20f10000))SetFinalResultOnly(dut,PASS);
//		else SetFinalResultOnly(dut,FAIL);
		
		if(temp1[dut-1][g_flash_no-1] & 0x8000)printf(" TMEN");
		if(!(temp1[dut-1][g_flash_no-1]&0x4000))printf(" PORb");
		if(!(temp1[dut-1][g_flash_no-1] & 0x2000))printf(" CEb");
		if(temp1[dut-1][g_flash_no-1] & 0x1000)printf(" DPD");
		if(temp1[dut-1][g_flash_no-1] & 0x800)printf(" RDEN");
		if(temp1[dut-1][g_flash_no-1] & 0x400)printf(" RECALL");
		if(temp1[dut-1][g_flash_no-1] & 0x200)printf(" VREAD");
		if(!(temp1[dut-1][g_flash_no-1] & 0x100))printf(" WEb");
		if(temp1[dut-1][g_flash_no-1] & 0x80)printf(" PROG");
		if(temp1[dut-1][g_flash_no-1] & 0x40)printf(" PROG2");
		if(temp1[dut-1][g_flash_no-1] & 0x20)printf(" PREPG");
		if(temp1[dut-1][g_flash_no-1] & 0x10)printf(" CHIP");
		if(temp1[dut-1][g_flash_no-1] & 0x8)printf(" BLK");
		if(temp1[dut-1][g_flash_no-1] & 0x4)printf(" ERASE");
		if(temp1[dut-1][g_flash_no-1] & 0x3)printf(" RETRY%x",temp1[dut-1][g_flash_no-1]&0x3);
		if(temp1[dut-1][g_flash_no-1] & 0xc0000000)printf(" WriteMASK%x",(temp1[dut-1][g_flash_no-1]&0xc0000000)>>30);
		if(temp1[dut-1][g_flash_no-1] & 0x20000000)printf(" BUSY");
		if(temp1[dut-1][g_flash_no-1] & 0x10000000)printf(" COUNT");
		if(temp1[dut-1][g_flash_no-1] & 0x8000000)printf(" BIST_SEL");
		if(temp1[dut-1][g_flash_no-1] & 0x4000000)printf(" CONFEN");
		if(temp1[dut-1][g_flash_no-1] & 0x2000000)printf(" LVCTL");
		if(temp1[dut-1][g_flash_no-1] & 0x1000000)printf(" Addr_Ctl");
		if(temp1[dut-1][g_flash_no-1] & 0x400000)printf(" NVR");
		if(temp1[dut-1][g_flash_no-1] & 0x200000)printf(" LCK_CFG");
		if(temp1[dut-1][g_flash_no-1] & 0x100000)printf(" NVR_CFG");
		if(temp1[dut-1][g_flash_no-1] & 0x30000)printf(" ADDRN%x",temp1[dut-1][g_flash_no-1]&0x3);
		printf("\n");

		END_FLASH_LOOP
	END_DUT_LOOP

	ReadReg_STB_BFAIL_REG(1);

	START_DUT_LOOP(UT_CDUT)
	START_FLASH_LOOP
		printf("DUT[%i] flash_no%d BIST FAIL REG: 0x%08X_%08X\n", dut, g_flash_no, temp2[dut-1][g_flash_no-1], temp1[dut-1][g_flash_no-1]);fflush(stdout);
//		SetFinalResultOnly(dut,PASS);
	END_FLASH_LOOP
	END_DUT_LOOP

}
void  debug_ReadFlash(unsigned int AREA,unsigned int addr)
{
	unsigned int DATA3[1][DDUTCNT][FLASH_NUM],DATA2[1][DDUTCNT][FLASH_NUM],DATA1[1][DDUTCNT][FLASH_NUM];
		ReadFlash(AREA, 0x0,0x0,0x1,addr,DATA3,DATA2,DATA1);
			START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
				temp3[dut-1][g_flash_no-1]=DATA3[0][dut-1][g_flash_no-1];
				temp2[dut-1][g_flash_no-1]=DATA2[0][dut-1][g_flash_no-1];
				temp1[dut-1][g_flash_no-1]=DATA1[0][dut-1][g_flash_no-1];
			END_DUT_LOOP
			END_FLASH_LOOP


}
void debug_ReadFlash_SaveFile(char* identifier, int area, int startaddr, int endaddr, int stepaddr)
{
	int ybits = 0, xbits=0;
	int transfer_bits = 13;
	//int tempyaddr=DATA_BITSIZE-1;
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

	unsigned int*d1=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int*d2=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	unsigned int*d3=(unsigned int*)malloc(sizeof(unsigned int)*(1<<xbits));
	START_FLASH_LOOP
	setRegValue(UT_REG_D2C, area<<8);//set for Y15-8
	setRegValue(UT_REG_D1C, round_startaddr);//set for X23-0
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
	FlashPatStart("patFlash.mpa", "FN_NORL_READ_FCM");
	

	
	START_DUT_LOOP(UT_CDUT)
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			memset(d1, 0, sizeof(unsigned int)*(1<<xbits));//bit31:0
			memset(d2, 0, sizeof(unsigned int)*(1<<xbits));//bit63:32
			memset(d3, 0, sizeof(unsigned int)*(1<<xbits));//bit71:64
		ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, endaddr-round_startaddr, 0, (1<<ybits)-1, dut);	
			for(i=startaddr-round_startaddr;i<=endaddr-round_startaddr;i++)
			{
				for(j=0;j<(DATA_BITSIZE<32?DATA_BITSIZE:32);j++)
				{
					d1[i] |= (((fcm_data[i*(1<<ybits)+DATA_BITSIZE-1-j]))&1)<<j;
				}
				for(j=32;j<(DATA_BITSIZE<64?DATA_BITSIZE:64);j++)
				{
					d2[i] |= (((fcm_data[i*(1<<ybits)+DATA_BITSIZE-1-j]))&1)<<(j-32);
				}
				for(j=64;j<(DATA_BITSIZE<72?DATA_BITSIZE:72);j++)
				{
					d3[i] |= (((fcm_data[i*(1<<ybits)+DATA_BITSIZE-1-j]))&1)<<(j-(1<<ybits));
				}
//				DATA3[i][dut-1][g_flash_no-1]=d3[i];
//				DATA2[i][dut-1][g_flash_no-1]=d2[i];
//				DATA1[i][dut-1][g_flash_no-1]=d1[i];
//				printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, 0, d3[0], d2[0], d1[0]);fflush(stdout);
			}
			// write readdata to file
		//	sprintf(fname,"/home/fsdiag/SMIC/data_detail/%s_%s_%sSITE%d_%04d-%02d-%02d_%02d%02d%02d_%x_%x.csv", LotNumber, WaferId, identifier,UTL_ReadSiteNumber() ,lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, startaddr, endaddr);
			sprintf(fname,"../data_detail/%s_%s_%sSITE%dSYSDUT%d_%04d-%02d-%02d_%02d%02d%02d_%x_%x.csv", LotNumber, WaferId, identifier,UTL_ReadSiteNumber() ,sysdut,lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, startaddr, endaddr);
			if((fp=fopen(fname,"aw")) == NULL)
			{
				printf("An error happens while opening a file named \" %s \".\n",fname);
			}
			else
			{
			//	fprintf(fp,"SYSDUT:%d\tDUT:%d\t(%d,%d)\taddress,data[71:64],data[63:32],data[31:0]\n",sysdut, dut, g_die_X[dut-1], g_die_Y[dut-1]);
				fprintf(fp,"SYSDUT:%d\tDUT:%d\tFLASHNUM:%d\t(%d,%d)\taddress,data[71:64],data[63:32],data[31:0]\n",sysdut, dut,g_flash_no, g_die_X[dut-1], g_die_Y[dut-1]);
				int addr = 0;
					for(i=startaddr-round_startaddr;i<=endaddr-round_startaddr;i++)
					{
						fprintf(fp,"\t\t\t\t0x%05X", (area<<24)|(i+round_startaddr));
//						for(fn=0;fn<CHANNEL_NUM;fn++)
//						{
					//	fprintf(fp,"\t\t\t\t0x%05X,0x%02X,0x%08X,0x%08X,", (area<<24)|(i+round_startaddr), d3[i], d2[i], d1[i]);
						fprintf(fp,"\t\t\t\t0x%05X,%d,0x%02X,0x%08X,0x%08X", (area<<24)|(i+round_startaddr),g_flash_no, d3[i], d2[i],d1[i]);
//						}
//						if(g_flash_no==FLASH_NUM)	
						fprintf(fp,"\n");
					}
                                        
						fclose(fp);
						printf("DUT%d save file:%s\n", dut, fname);
			}	
	END_DUT_LOOP
END_FLASH_LOOP
	free(fcm_data);
		free(d1);
		free(d2);
		free(d3);
}
void  debug_Ref_current()
{
		int ft_refset[8]={0,1,2,3,7,6,5,4};
		int i,j,m,ref_set;
		double temp[72][DDUTCNT][FLASH_NUM]={0};
//		double temp_buf[72][DDUTCNT][5]={0};
		int bits=32;
		unsigned int data=0xffffffff,CBD3=0;
		
//	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 5);
	SetLevelHigh();
//	setDcVSIM(0.6 V, R20V, R20uA,20 UA, -20 UA, 20 UA,-20 UA, 5);
	setDcVSIM(0.5 V, R20V, R5uA,5 UA, -5 UA, 5 UA,-5 UA, 5);
	ulSetSettlingTime(5 MS,5 MS,5 MS);
//test ref current of Margin 0
//		printf("\nRef current of Margin0:");

		ref_set= (( ft_refset[0]<<13) | 0xffff1fc9);
				for(j=0;j<32; j++)
			{
//			setRegValue(UT_REG_TPH2B,ref_set);//set for X23-0
			setRegValue(UT_REG_TPH2,ref_set);//set for X23-0
				//	M0 
					setRegDataIn(0x0,0x0,(REF_trim_value[j]|0xffe0)<<10);	
					setRegAddr(AREA_M,0xfffffe);
					FlashPatExec_NoWait("patFlash.mpa","REF_CURRENT_M0");
					UTL_WaitTime(25 MS);
				START_FLASH_LOOP
				
					DcExec("TM0");
				//UTL_StopFct();			
 					START_DUT_LOOP(UT_CDUT)//init data 
		//			TransMeasData("TM0", "REF_current",200 UA , -200 UA, 1 UA, "UA",temp[j],dut);
					ReadMeasData_ByFlash("TM0", 1 UA, temp[j], dut, g_flash_no);
					END_DUT_LOOP
				
				END_FLASH_LOOP
				UTL_StopFct();			
		FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
			}
		START_FLASH_LOOP
 		START_DUT_LOOP(UT_CDUT)//init data 
		  printf("\nx:%3d,y:%3d,DUT%d F-NO%d M0_REFcur(uA) 0.5v:",g_die_X[dut-1], g_die_Y[dut-1],dut,g_flash_no);
//		for(m=0;m<5;m++)
//		  	printf("\n time%d :",m+1);
			for(j=0;j<32; j++)printf("%.2f,",temp[j][dut-1][g_flash_no-1]);
		
		END_DUT_LOOP
		END_FLASH_LOOP
//test ref current of Margin 1
		
		printf("\n");
	printf("\nRef current of Margin1:");
		ref_set= (( ft_refset[0]<<13) | 0xffff1fc9);
				for(j=0;j<32; j++)
			{
//			setRegValue(UT_REG_TPH2B,ref_set);//set for X23-0
			setRegValue(UT_REG_TPH2,ref_set);//set for X23-0
				//	M0 
				//	M1 
					setRegDataIn(0x0,0x0,(REF_trim_value[j]|0xffe0));	
					setRegAddr(AREA_M,0xfffffe);
					FlashPatExec_NoWait("patFlash.mpa","REF_CURRENT_M1");
					UTL_WaitTime(25 MS);
			
	START_FLASH_LOOP			
					DcExec("TM0");
 					START_DUT_LOOP(UT_CDUT)//init data 
				//	TransMeasData("TM0", "REF_current",200 UA , -200 UA, 1 UA, "UA",temp[j],dut);
		ReadMeasData_ByFlash("TM0", 1 UA, temp[j], dut, g_flash_no);
					
					END_DUT_LOOP
			
			UTL_StopFct();			
			END_FLASH_LOOP
			FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
			
		}

 		START_DUT_LOOP(UT_CDUT)//init data 
		START_FLASH_LOOP  	
		printf("\nx:%3d,y:%3d, DUT%d F-NO%d M1_REFcur(uA) 0.5v:",g_die_X[dut-1], g_die_Y[dut-1],dut,g_flash_no);
			for(j=0;j<32; j++)printf("%.2f,",temp[j][dut-1][g_flash_no-1]);
		END_FLASH_LOOP
		END_DUT_LOOP


		printf("\n");		

}
void debug_cell_current()
{

		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");
    	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 2);
		ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2
		int i,j,m,vddflag=0;
		int add[6]={0x7af,0x7b3,0x7b7,0x7ef,0x7f3,0x7f7};
		int sector_addr=0x0;
		unsigned int addr,bit_high_low,bit_mask;
		double temp[72][DDUTCNT][FLASH_NUM]={0};
		double temp_BUF[72][DDUTCNT][FLASH_NUM][5]={0};
//		ReadFLASH(AREA_M,0x7b3);
//		ReadFLASH(AREA_M,0x7f3);
	FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
	UTL_StopFct();			
 
//		setRegAddr(AREA_M,SECTOR_ADDR);
	//	for (j=0x7b3;j<0xfffff;j++)
//		for (j=0x780;j<0x800;j++)
//		for (j=0x2b83;j<0x2b84;j++)
		for (j=sector_addr;j<sector_addr+0x200;j++)
	{   //  addr=add[j];
			addr=j;
		  printf("\naddr:0x%x",addr);
		for(i=0;i<64;i++)	 
			{
				//	setRegDataIn(0xff,0xffff,~(0x1<<i));	
					if(i>32)bit_high_low=0x400000; 
			//		if(i>35)bit_high_low=0x400000; 
					else bit_high_low=0x800000;
			
				//	if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
				//	else if(i>31 && i<36)  setRegDataIn(0xff,~(0x1<<(i-32)),0xffffffff);	
				//	else if(i>35 && i<68) setRegDataIn(0xff,0xffff,~(0x1<<(i-36)));
				//	else setRegDataIn(0xff,~(0x1<<(i-68)),0xffffffff);

					if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
					else setRegDataIn(0xff,0xffff,~(0x1<<(i-32)));

					
					setRegAddr(AREA_M,addr|bit_high_low);
					FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT_SET");
					UTL_WaitTime(3 MS);
					DcExec("TM0");

				
					START_FLASH_LOOP
 					START_DUT_LOOP(UT_CDUT)//init data 
					ReadMeasData_ByFlash("TM0",  1 UA, temp[i],dut,g_flash_no);
				//	temp_BUF[i][dut-1][m]=temp[i][dut-1];
					END_DUT_LOOP
					UTL_StopFct();			
				}
		 			END_FLASH_LOOP	
				

		START_FLASH_LOOP
 		START_DUT_LOOP(UT_CDUT)//init data 
		
		printf("\nflash1  ref:11\taddr:0x%x\tDUT%2dA):\t",addr,dut);  
		printf("\naddr:0x%x 64IO DUT%d_F-Nu%d(uA):\t",addr,dut, g_flash_no);
//		printf("\nref:11\taddr:0x%x\tDUT%02d(uA):\t",addr,dut);
			for(i=0;i<64; i++)printf("%.2f,",temp[i][dut-1][g_flash_no-1]);
		END_DUT_LOOP
		END_FLASH_LOOP
	
				
				//	UTL_WaitTime(15 MS);



	}
												
		printf("\n");		
}
void debug_cell_current_one_addr()
{

		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");
    	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 2);
		ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2
		int i,j,m,vddflag=0;
		int add[6]={0x69ac,0x7b3,0x7b7,0x7ef,0x7f3,0x7f7};
		unsigned int addr,bit_high_low,bit_mask;
		double temp[72][DDUTCNT]={0};
		double temp_BUF[72][DDUTCNT][5]={0};
// 
//		setRegAddr(AREA_M,SECTOR_ADDR);
	//	for (j=0x7b3;j<0xfffff;j++)
//yy		for (j=0x780;j<0x800;j++)
//		for (j=0x2b83;j<0x2b84;j++)
//		for (j=SECTOR_ADDR;j<SECTOR_ADDR+0x80;j++)
		for (j=0;j<1;j++)
	{  
//   addr=add[j];
   addr=ONE_ADDR;
		//	addr=j;
//		  printf("\naddr:0x%x",addr);
//		for(i=0;i<72;i++)	 
		for(i=0;i<64;i++)	 
//		for(i=0;i<8;i++)	 
//		for(i=20;i<24;i++)	 
			{
//		setRegDataIn(0x0,0x0,0x0);
//		setRegAddr(AREA_M,j);
//		setRegAddr(AREA_M,ONE_ADDR);
//		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");
				//	setRegDataIn(0xff,0xffff,~(0x1<<i));	
					if(i>=32)bit_high_low=0x400000; 
			//		if(i>35)bit_high_low=0x400000; 
					else bit_high_low=0x800000;
			
				//	if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
				//	else if(i>31 && i<36)  setRegDataIn(0xff,~(0x1<<(i-32)),0xffffffff);	
				//	else if(i>35 && i<68) setRegDataIn(0xff,0xffff,~(0x1<<(i-36)));
				//	else setRegDataIn(0xff,~(0x1<<(i-68)),0xffffffff);

					
					if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
					else setRegDataIn(0xff,0xffff,~(0x1<<(i-32)));

//		  printf("\ni==:%d  ADDR:%x\t",i,addr|bit_high_low);
					
//					setRegAddr(AREA_M,addr|bit_high_low);
					setRegAddr(AREA_N,addr|bit_high_low);
					FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
					UTL_WaitTime(1 MS);
					DcExec("TM0");

		//		DcExecQuick2("TM0");
				//	UTL_StopFct();			
 					START_DUT_LOOP(UT_CDUT)//init data 
	//				TransMeasData("TM0", "REF_current",200 UA , -200 UA, 1 UA, "UA",temp[i],dut);
					ReadMeasData("TM0", 1 UA, temp[i],dut);
				//	temp_BUF[i][dut-1][m]=temp[i][dut-1];
					END_DUT_LOOP
			
					
					UTL_StopFct();			
					FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
				//	UTL_WaitTime(15 MS);
			}

	//	UTL_StopFct();			
	//	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");


 		START_DUT_LOOP(UT_CDUT)//init data 
//		  printf("\nDUT%2d Cell cur(uA):\t",dut);
		  printf("\nref:11\taddr:0x%x\tDUT%02d(uA):\t",addr,dut);
//			for(i=0;i<72; i++)printf("%.2f,",temp[i][dut-1]);
//			for(i=0;i<64; i++)printf("%.2f(%d),",temp[i][dut-1],i);
			for(i=0;i<64; i++)printf("%.2f,",temp[i][dut-1]);
//		printf("");		
		END_DUT_LOOP
	}
					
		printf("\n");		
}
void debug_cell_current_one_addr_scan_1()
{

		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");
    	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 2);
		ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2
		int i,j,m,vddflag=0;
		int add[6]={0x69ac,0x7b3,0x7b7,0x7ef,0x7f3,0x7f7};
		unsigned int addr,bit_high_low,bit_mask;
		double temp[72][DDUTCNT]={0};
		double temp_BUF[72][DDUTCNT][5]={0};
// 
//		setRegAddr(AREA_M,SECTOR_ADDR);
	//	for (j=0x7b3;j<0xfffff;j++)
//		for (j=0x780;j<0x800;j++)
//		for (j=0x2b83;j<0x2b84;j++)
//		for (j=SECTOR_ADDR+0x0;j<SECTOR_ADDR+0x2;j=j+1)
//		for (j=SECTOR_ADDR+0x0;j<SECTOR_ADDR+0x65;j=j+1)
//		for (j=SECTOR_ADDR+0x80;j<SECTOR_ADDR+0xe1;j=j+1)
		for (j=ROW_ADDR;j<ROW_ADDR+0x14;j=j+1)
//		for (j=SECTOR_ADDR+0x0;j<SECTOR_ADDR+0x6c;j=j+1)
//		for (j=0;j<1;j++)
	{  
//   addr=add[j];
   addr=ONE_ADDR;
		//	addr=j;
		  printf("\naddr:0x%x",j);
		setRegDataIn(0x0,0x0,0x0);
//		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
//		setRegAddr(AREA_M,ONE_ADDR);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");
		setRegDataIn(0x0,0x0,0x0);
//		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
//		setRegAddr(AREA_M,ONE_ADDR);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");
		setRegDataIn(0x0,0x0,0x0);
//		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
//		setRegAddr(AREA_M,ONE_ADDR);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");
//		ReadFlash_SaveFile("Main_Disturb", AREA_M, 0x000, 0x1ffff, 0x1);

//		ReadFlash_SaveFile("NVR_Disturb1", AREA_N, 0x000, 0x1ffff, 0x1);
//		for(i=0;i<72;i++)	 
		for(i=0;i<64;i++)	 
//		for(i=0;i<8;i++)	 
//		for(i=20;i<24;i++)	 
			{
				//	setRegDataIn(0xff,0xffff,~(0x1<<i));	
					if(i>=32)bit_high_low=0x400000; 
			//		if(i>35)bit_high_low=0x400000; 
					else bit_high_low=0x800000;
			
				//	if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
				//	else if(i>31 && i<36)  setRegDataIn(0xff,~(0x1<<(i-32)),0xffffffff);	
				//	else if(i>35 && i<68) setRegDataIn(0xff,0xffff,~(0x1<<(i-36)));
				//	else setRegDataIn(0xff,~(0x1<<(i-68)),0xffffffff);

					if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
					else setRegDataIn(0xff,0xffff,~(0x1<<(i-32)));

					
//					setRegAddr(AREA_M,addr|bit_high_low);
					setRegAddr(AREA_N,addr|bit_high_low);
					FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
					UTL_WaitTime(1 MS);
					DcExec("TM0");

		//		DcExecQuick2("TM0");
				//	UTL_StopFct();			
 					START_DUT_LOOP(UT_CDUT)//init data 
					ReadMeasData("TM0",  1 UA, temp[i],dut);
				//	temp_BUF[i][dut-1][m]=temp[i][dut-1];
					END_DUT_LOOP
			
					
					UTL_StopFct();			
					FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
				//	UTL_WaitTime(15 MS);
			}

	//	UTL_StopFct();			
	//	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");


 		START_DUT_LOOP(UT_CDUT)//init data 
//		  printf("\nDUT%2d Cell cur(uA):\t",dut);
		  printf("\nref:11\taddr:0x%x\tDUT%02d(uA):\t",addr,dut);
//			for(i=0;i<72; i++)printf("%.2f,",temp[i][dut-1]);
			for(i=0;i<64; i++)printf("%.2f,",temp[i][dut-1]);
//		printf("");		
		END_DUT_LOOP
	}
					
		printf("\n");		
}
void debug_cell_current_one_addr_scan_2()
{

		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");
    	setDcVSIM(0.5 V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 2);
		ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2
		int i,j,m,vddflag=0;
		int add[6]={0x69ac,0x7b3,0x7b7,0x7ef,0x7f3,0x7f7};
		unsigned int addr,bit_high_low,bit_mask;
		double temp[72][DDUTCNT]={0};
		double temp_BUF[72][DDUTCNT][5]={0};
// 
//		setRegAddr(AREA_M,SECTOR_ADDR);
	//	for (j=0x7b3;j<0xfffff;j++)
//yy		for (j=0x780;j<0x800;j++)
//		for (j=0x2b83;j<0x2b84;j++)
//	for (j=SECTOR_ADDR+0x03;j<SECTOR_ADDR+0x80;j++)
//	for (j=SECTOR_ADDR+0xe2;j<SECTOR_ADDR+0x100;j++)
	for (j=ROW_ADDR+0x15;j<ROW_ADDR+0x100;j++)
//  	for (j=SECTOR_ADDR+0x6d;j<SECTOR_ADDR+0x80;j++)
//		for (j=0;j<1;j++)
	{  
//   addr=add[j];
   addr=ONE_ADDR;
		//	addr=j;
		  printf("\naddr:0x%x",j);
//		setRegDataIn(0x0,0x0,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
//		setRegAddr(AREA_M,ONE_ADDR);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");

//		setRegDataIn(0x0,0x0,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");
//		setRegDataIn(0x0,0x0,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
//		setRegAddr(AREA_M,j);
		setRegAddr(AREA_N,j);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_ALL_64");

//		ReadFlash_SaveFile("NVR_Disturb2", AREA_N, 0x000, 0x1ffff, 0x1);
//		for(i=0;i<72;i++)	 
		for(i=0;i<64;i++)	 
//		for(i=0;i<8;i++)	 
//		for(i=20;i<24;i++)	 
			{
				//	setRegDataIn(0xff,0xffff,~(0x1<<i));	
					if(i>=32)bit_high_low=0x400000; 
			//		if(i>35)bit_high_low=0x400000; 
					else bit_high_low=0x800000;
			
				//	if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
				//	else if(i>31 && i<36)  setRegDataIn(0xff,~(0x1<<(i-32)),0xffffffff);	
				//	else if(i>35 && i<68) setRegDataIn(0xff,0xffff,~(0x1<<(i-36)));
				//	else setRegDataIn(0xff,~(0x1<<(i-68)),0xffffffff);

					if (i<32)setRegDataIn(0xff,0xffff,~(0x1<<i));
					else setRegDataIn(0xff,0xffff,~(0x1<<(i-32)));

					
//					setRegAddr(AREA_M,addr|bit_high_low);
					setRegAddr(AREA_N,addr|bit_high_low);
					FlashPatExec_NoWait("patFlash.mpa","CELL_CURRENT");
					UTL_WaitTime(1 MS);
					DcExec("TM0");

		//		DcExecQuick2("TM0");
				//	UTL_StopFct();			
 					START_DUT_LOOP(UT_CDUT)//init data 
					ReadMeasData("TM0", 1 UA, temp[i],dut);
				//	temp_BUF[i][dut-1][m]=temp[i][dut-1];
					END_DUT_LOOP
			
					
					UTL_StopFct();			
					FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");
				//	UTL_WaitTime(15 MS);
			}

	//	UTL_StopFct();			
	//	FlashPatExec("patFlash.mpa","HV_TRIM_EXIT_ERASE");


 		START_DUT_LOOP(UT_CDUT)//init data 
//		  printf("\nDUT%2d Cell cur(uA):\t",dut);
		  printf("\nref:11\taddr:0x%x\tDUT%02d(uA):\t",addr,dut);
//			for(i=0;i<72; i++)printf("%.2f,",temp[i][dut-1]);
			for(i=0;i<64; i++)printf("%.2f,",temp[i][dut-1]);
//		printf("");		
		END_DUT_LOOP
	}
					
		printf("\n");		
}
void debug_read_Margin0_ckbd_cg3p5_scan(){
		setHvUnMask("VPP1_PINS");
		FlashPatExec("patFlash.mpa", "MARGIN0_CG3P5_MODE");
		double cg3p5=0;
		for(cg3p5=1;cg3p5<7;cg3p5=cg3p5+0.2)
		{
		printf("cg3p5=:%2.3lf \n",cg3p5);
		setHvVSIM("VPP1_PINS",cg3p5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_CSLRD_CKBD_ALL");
	}
		FlashPatExec("patFlash.mpa", "TMODE_EXIT");
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void debug_endurance_screen(){
		setHvUnMask("VPP1_PINS");
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0x0,0xffffffff,0xffffffff);
double VEG=0;	
		for(VEG=8;VEG<10;VEG=VEG+0.1)
		{
		printf("VEG=:%2.3lf \n",VEG);
		setHvVSIM("VPP1_PINS",VEG,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		FlashPatExec("patFlash.mpa", "FN_ENDURANCE_SCR");
		setRegAddr(AREA_M,0x0);
		setRegDataIn(0xff,0xffffffff,0xffffffff);
		FlashPatExec("patFlash.mpa", "BI_CREAD_NORL_ALL");
		}
		setHvVSIM("VPP1_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
		setHvMask("VPP1_PINS");
}
void debug_nvrx_program_debug2(){ 

//		setRegAddr(AREA_M,0x899);
//		setRegDataIn(0x0,0x0,0x0);
//		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
		setRegAddr(AREA_M,0x403);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
		setRegAddr(AREA_M,0x303);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_DEBUG");
}
void debug_nvrx_program_debug(){ 
	


		
	setRegAddr(AREA_M,ONE_ADDR);
		setRegDataIn(0x0,0xFFFFFFFF,0xFFFF5555);
		setRegValue(UT_REG_TPH1B,0xffffffff);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_CKBD");
}
void debug_program_one_row(){
		setRegAddr(AREA_N,ROW_01_ADDR);
		setRegDataIn(0x0,0x0,0x0);
		FlashPatExec("patFlash.mpa", "BI_RPROG_NORL");
}
void debug_sector_erase_one(){
		setRegAddr(AREA_M,0x0);
		FlashPatExec("patFlash.mpa", "FN_SECT_ERAS");
}

void tb_nvrx_program_debug2(){ 
	START_FLASH_LOOP
	   if(g_flash_no==1){
		setRegAddr(AREA_M,0x1902);
		setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
           }
	   if(g_flash_no==2){
		setRegAddr(AREA_M,0x1e98);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
		setRegAddr(AREA_M,0x1903);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	   }
	   if(g_flash_no==3){
		setRegAddr(AREA_M,0x1f98);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	   }
	   if(g_flash_no==4){
		setRegAddr(AREA_M,0x1f98);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
		setRegAddr(AREA_M,0x1a82);
		setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	   }
	END_FLASH_LOOP
}




#if 0
void tb_nvrx_program_debug2(){ 
	START_FLASH_LOOP
//		setRegAddr(AREA_M,0x1);
//		setRegAddr(AREA_M,0x803);
		//setRegAddr(AREA_M,0x1a82);
		//setRegAddr(AREA_M,0x5902);
		setRegAddr(AREA_M,0x1902);
		setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
	 //  	FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n1.flash_no:%d",g_flash_no);
	  // if(g_flash_no==1)
	   if(g_flash_no==2)
		{
			FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  		printf("\n1-1.flash_no:%d",g_flash_no);
		}
	  printf("\n2.flash_no:%d",g_flash_no);
//	END_FLASH_LOOP
UTL_WaitTime(5 MS);
//	START_FLASH_LOOP
		//setRegAddr(AREA_M,0x7a98);
		setRegAddr(AREA_M,0x1e98);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
	  // FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n3.flash_no:%d",g_flash_no);
	   //if(g_flash_no==1)
	   if(g_flash_no==2)
	{
//		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	
//		setRegAddr(AREA_M,0x7a98);
		setRegAddr(AREA_M,0x1e98);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);

		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n3-2.flash_no:%d\n",g_flash_no);
	}




#if 1
//		setRegAddr(AREA_M,0x1982);
		setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
		setRegAddr(AREA_M,0x1a82);
		//setRegAddr(AREA_M,0x1902);
		//setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
	 //  	FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n1.flash_no:%d",g_flash_no);
	   if(g_flash_no==1)
	  // if(g_flash_no==2)
		{
			FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  		printf("\n1-1.flash_no:%d",g_flash_no);
		}
	  printf("\n2.flash_no:%d",g_flash_no);
//	END_FLASH_LOOP
UTL_WaitTime(5 MS);
//	START_FLASH_LOOP
		//setRegAddr(AREA_M,0x7e18);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		setRegAddr(AREA_M,0x7a98);
		//setRegAddr(AREA_M,0x1e98);
		//setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
	  // FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n3.flash_no:%d",g_flash_no);
	   if(g_flash_no==3)
	   //if(g_flash_no==2)
	{
//		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	
		setRegAddr(AREA_M,0x1903);
		//setRegAddr(AREA_M,0x3a98);
//		setRegAddr(AREA_M,0x7e18);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);

		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n3-2.flash_no:%d\n",g_flash_no);
	}


	   if(g_flash_no==4)
	   //if(g_flash_no==2)
	{
//		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	
		setRegAddr(AREA_M,0x1902);
		//setRegAddr(AREA_M,0x3a98);
//		setRegAddr(AREA_M,0x7e18);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);

		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	  printf("\n3-2.flash_no:%d\n",g_flash_no);
	}
#endif
	END_FLASH_LOOP


#if 0
	START_FLASH_LOOP
//		setRegAddr(AREA_M,0x1);
//		setRegAddr(AREA_M,0x803);
		setRegAddr(AREA_M,0x283);
		setRegDataIn(0xff,0xf7f7cfcf,0x3f3f2f2f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	END_FLASH_LOOP
	START_FLASH_LOOP
		setRegAddr(AREA_M,0x800603);
		setRegDataIn(0xff,0x3f3f4f4f,0x6f6f8f8f);
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG");
	END_FLASH_LOOP
#endif
}
#endif


