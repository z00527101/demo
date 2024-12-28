//----------------------------------------------------------------------------
//--Section:			Include Files
//---------------------------------------------------------------------------
#include "common.h"
#include "stdlib.h"

int in_flash_loop=0;
int g_flash_no=1;
PIN_STRUCT g_pinlist[64];
char strNextSeq[DDUTCNT][100];
bool bFlowDutActive[DDUTCNT];
int g_die_X[DDUTCNT] = {0};
int g_die_Y[DDUTCNT] = {0};
int g_BinNo[DDUTCNT] = {0};
char *g_BinName[DDUTCNT];
int g_Rejected[DDUTCNT] = {0};
double g_tRjectTitemTime[DDUTCNT];
char *DeviceName = {'\0'};
char *LotNumber = {'\0'};
char *ProberID = {'\0'};
char *WaferId = {'\0'};
unsigned int shotcounter;
char *WaferSlotNumber = {'\0'};
int FK[20];
int FirstRun=1;
PinHandle PIN_;
char mpa_name[50]="NULL";


//--part1-------------------------- Test Flow-----------------------------------
void InitialVariable()
{
	DeviceName = UTL_ReadHeaderDeviceName();
//--	LotNumber = "EPA696-ENG";
#ifdef PRIVATE_LOT_ID
	LotNumber = "UPE108";
#endif
#ifdef PROBER_LOT_ID
	LotNumber = UTL_ReadHeaderLotNumber();
#endif
//--	WaferId = "EPA696-04-D0";
#ifdef PRIVATE_LOT_ID
	WaferId = "UPE108-17";
#endif
#ifdef PROBER_LOT_ID
	WaferId = UTL_ReadHeaderWaferId();
#endif
	WaferSlotNumber = UTL_ReadWaferSlotNumber();
	ProberID = UTL_ReadProberID();
	shotcounter = UTL_ReadShotCounter();
	START_DUT_LOOP(UT_SDUT)
#ifdef PRIVATE_LOT_ID
		g_die_X[dut-1] = 74;
		g_die_Y[dut-1] = 77;

#endif
#ifdef PROBER_LOT_ID
		g_die_X[dut-1] = UTL_ReadDutXLocation(dut);
		g_die_Y[dut-1] = UTL_ReadDutYLocation(dut);
#endif
		g_BinNo[dut-1] = 1;
		g_BinName[dut-1] = "PASS";
		bFlowDutActive[dut-1]=true;
	END_DUT_LOOP
	START_DUT_LOOP(UT_DDUT)
		sprintf(strNextSeq[dut-1],"%s","NEXT");
	END_DUT_LOOP
	ReadFk(FK);
	if(UTL_ReadFirstFlag()==UT_ON) FirstRun=1;
	else FirstRun=0;
}

void TestStartLog()
{
	int i;
	setlinebuf   (stdout);
	
	START_DUT_LOOP(UT_SDUT)
		printf((char *)"[D%02d] (%d,%d)\n",dut, g_die_X[dut-1], g_die_Y[dut-1]);		
	END_DUT_LOOP
	printf((char *)"TOUCHDOWN_NUM %d\n",shotcounter);		
	printf("DeviceName = %s\n", DeviceName);
	printf("shotcounter = %d\n", shotcounter);
	printf("LotNumber = %s\n", LotNumber);
	printf("WaferId = %s\n", WaferId);
	printf("WaferSlotNumber = %s\n", WaferSlotNumber);
	printf("ProberID = %s\n", ProberID);
	
	char date[256];
	time_t timep;
	timep = time (0);
	strftime(date,128,(char *)"%Y/%m/%d %H:%M:%S",localtime(&timep));
	printf((char *)"\nTEST_START_TIME:\t%s\n", date);		
	printf("Test Program Compile time : %s %s\n", __DATE__, __TIME__);
}
void TestEndLog()
{
	CheckButton of;
	double dbTotal_Time;
	char szResult[32];
	UT_DUT sysdut;

	START_DUT_LOOP(UT_SDUT)
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		if(g_Rejected[dut-1])
		{
			sprintf(szResult, "FAIL");
			dbTotal_Time = g_tRjectTitemTime[dut-1]; 
		}
		else
		{
			sprintf(szResult, "PASS"); 
			dbTotal_Time = UTL_ReadTimer(&of); 
			if(isRepair[dut-1])g_BinNo[dut-1]=2; 
			UTL_SetCategory( dut, g_BinNo[dut-1]);
		}
		printf((char *)"$RESULT_DUT_INFO %5d %5d %5d %5d %5d %5d, TT = %.0f MS, BIN NAME = %s\n",\
				g_die_X[dut-1], g_die_Y[dut-1], g_BinNo[dut-1], g_BinNo[dut-1], UTL_ReadSiteNumber(), sysdut, dbTotal_Time * 1000.0, g_BinName[dut-1]);
	END_DUT_LOOP
	char date[256];
	time_t timep;
	timep = time (0);
	strftime(date,128,(char *)"%Y/%m/%d %H:%M:%S",localtime(&timep));
	printf((char *)"\n");	
	printf((char *)"TEST_END_TIME:\t%s\n\n", date);	
}
void FlowPreProcess(char * label)
{
	START_DUT_LOOP(UT_MDUT)
		if( strcmp(strNextSeq[dut-1],"NEXT")==0)
		{
			bFlowDutActive[dut-1]=true;
		}
		else if ( strcmp(strNextSeq[dut-1],label)==0)
		{
			bFlowDutActive[dut-1]=true;			
		}
		else
		{
			bFlowDutActive[dut-1]=false;			
		}			
	END_DUT_LOOP
	

	ExclusionHandle exclusion_handle = UTL_GetExclusionHandle();
	LevelFixHandle level_fix_handle = UTL_GetLevelFixHandle();
	//--------------------------------- Reset Exclusion -----------------------------------------------------
	UTL_SetExclusionSetOrReset( exclusion_handle, UT_OFF);
	UTL_ClearExclusionDut     ( exclusion_handle );									
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_NONE);
	if ( (UTL_GetDutCount( UT_MDUT) - UTL_GetDutCount( UT_CDUT)) > 0 ) {			
		START_PINLIST_LOOP("MASK_PINS")
		START_DUT_LOOP(UT_MDUT)                                                     
			UTL_AddExclusionDut( exclusion_handle , dut);                           
			UTL_SendLevelFix(level_fix_handle, pinno, dut);
		END_DUT_LOOP                                         
		END_PINLIST_LOOP                       
		UTL_SendExclusion( exclusion_handle);                                   
	}

	//----------------------------------- Set Exclusion ------------------------------------------------------
	UTL_ClearExclusionDut     ( exclusion_handle );                                 
	UTL_SetExclusionSetOrReset( exclusion_handle, UT_ON); //-- set-exclusion(Not Test Target)

	int exclusion_dut_count = 0;
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_FIXH);
	START_PINLIST_LOOP("MASK_PINS")
	START_DUT_LOOP(UT_MDUT)                      
		if( bFlowDutActive[dut-1]==false) {
			UTL_AddExclusionDut( exclusion_handle , dut);
			UTL_SendLevelFix(level_fix_handle, pinno, dut);
			exclusion_dut_count++;
		}
	END_DUT_LOOP
	END_PINLIST_LOOP                       
	if ( exclusion_dut_count ) UTL_SendExclusion( exclusion_handle);
	UTL_DeleteHandle          (exclusion_handle);
	UTL_DeleteHandle	  (level_fix_handle);

}

void RUN_TEST(char * label, void (*func)(), char *tname, void (*pre_exec)(), void (*post_exec)(), char *pass_branch, char * fail_branch ,  int bin)
{
	char sync_name[50]={'\0'};
	sprintf(sync_name,"%s_START",tname);
	WAIT_SITE_SYNC(sync_name)
	unsigned int overflow;
	struct timespec t_start, t_end;
	double t;
	clock_gettime(CLOCK_REALTIME, &t_start);   
	FlowPreProcess(label);

	if( UTL_GetDutCount(UT_CDUT) == 0) 
	{
		DebugPrintf((char *)"no die is active......................%s\n", tname);
		return;
	}
	pre_exec();
    printf("====================================================Begin Test Block %-3s %-30s================================\n",label,tname);
	char testname[100];
	sprintf(testname, "%s%s", label, tname);
	{
		TestHandle h = UTL_GetTestHandle();
		UTL_SetTestAction(h, func);
		UTL_Test         (h, testname);
		UTL_DeleteHandle (h);
	}
	post_exec();
	clock_gettime(CLOCK_REALTIME, &t_end);
	t = (double)(t_end.tv_sec - t_start.tv_sec) * 1e3 + ((double)(t_end.tv_nsec - t_start.tv_nsec) * 1e-6);
	START_DUT_LOOP(UT_CDUT)
		printf((char *)"[D%02d] %-3s %-30s ETT %10.0f MS      %s", dut, label, tname, t, UTL_ReadFinalResult(dut, UT_RES_ALL)==UT_RES_PASSED? "P":"F");
		if ((GetFinalResult(dut)!=PASS) && (strcmp(fail_branch,"STOP")!=0))
		{
			printf((char *)" (force passed)\n");
		}
		else
		{
			printf((char *)"\n");
		}
	END_DUT_LOOP


	START_DUT_LOOP(UT_MDUT)
		if(bFlowDutActive[dut-1]==true)
		{		
			if(UTL_ReadFinalResult(dut,UT_RES_ALL) == UT_RES_FAILED)
			{
				if( strcmp(fail_branch,"STOP")==0)//--bin out
				{
					if(FK[3]!=1)
					{
					g_BinNo[dut-1]=bin;				
					g_BinName[dut-1] = tname; 
					g_Rejected[dut-1]=1;
					UTL_SetCategory( dut, bin);
					UTL_SetRejection(dut, UT_OFF);
					DebugPrintf((char *)"***** DUT%d Rejected ******\n",dut);
					g_tRjectTitemTime[dut-1] = UTL_ReadTimer(&overflow);
					}
					else
					{

						sprintf(strNextSeq[dut-1],"%s","NEXT");
						UTL_ResetFinalResultOnly(dut,UT_RES_ALL);
					}
				}
				else//--force PASS
				{
					sprintf(strNextSeq[dut-1],"%s",fail_branch);
					UTL_ResetFinalResultOnly(dut,UT_RES_ALL);
				}
			}
			else	
			{
				if( strcmp(pass_branch,"STOP")==0)//--bin out
				{
					g_BinNo[dut-1]=bin;				
					g_BinName[dut-1] = tname; 
					g_Rejected[dut-1]=1;
					UTL_SetCategory( dut, bin);
					UTL_SetRejection(dut, UT_OFF);
					DebugPrintf((char *)"***** DUT%d Rejected ******\n",dut);
					g_tRjectTitemTime[dut-1] = UTL_ReadTimer(&overflow);
				}
				else if( strcmp(pass_branch,"END")==0)//--bin out
				{
					g_BinNo[dut-1]=1;				
					g_BinName[dut-1] = "PASS"; 
					g_Rejected[dut-1]=1;
					UTL_SetCategory( dut, 1);
					UTL_SetRejection(dut, UT_OFF);
					DebugPrintf((char *)"***** DUT%d Rejected ******\n",dut);
					g_tRjectTitemTime[dut-1] = UTL_ReadTimer(&overflow);
				}
				else						
				{
					sprintf(strNextSeq[dut-1],"%s",pass_branch);
				}
			}
		}
	END_DUT_LOOP
}

void Dummy(){}
//--part2----------------------- Pin settings ---------------------
void ulCreatePinListWithNumber(char *plst, int type, int num)
{
	UTL_CreatePinList(plst);
	UTL_SetPinType   (plst, type);
	UTL_AddPinNumber (plst, num);

	if(type==UT_PINTYPE_PIN){
		g_pinlist[num].pinno=num;
		strcpy(g_pinlist[num].pinname, plst);
	}else{
		g_pinlist[num].vsno=num;
		strcpy(g_pinlist[num].vsname, plst);
	}
}
void ulCreatePinListWithList(char *plst, int type, ...)
{
	va_list va;
	char   *ap;

	va_start(va, type);
	UTL_CreatePinList(plst);
	UTL_SetPinType   (plst, type);
	while((ap = va_arg(va, char *))!= NULL)
	{
		UTL_AppendPinList(plst, ap);
	}
	va_end(va);
}

void setVi(double vihigh, double vilow, int vino)
{
	ViHandle h = UTL_GetViHandle();
	UTL_SetViHigh(h, vihigh);
	UTL_SetViLow (h, vilow);
	UTL_SendVi   (h, vino);
	UTL_DeleteHandle(h);
}

void setVo(double vohigh, double volow, int vono)
{
	VoHandle h = UTL_GetVoHandle();
	UTL_SetVoHigh(h, vohigh);		
	UTL_SetVoLow (h, volow);
	UTL_SendVo   (h, vono);  
	UTL_DeleteHandle(h);
}

void setVt(double vt, int vtno)
{
	VtHandle h = UTL_GetVtHandle(); 
	UTL_SetVt    (h, vt);
	UTL_SendVt   (h, vtno);	
	UTL_DeleteHandle(h);
}

void setVload(double vload, int vload_no)
{
	VloadHandle vload_H = UTL_GetVloadHandle ();
	UTL_SetVload (vload_H,	vload);			//-- Set the desired Vload level
	UTL_SendVload (vload_H, vload_no);	    //-- Use the Vload number
	UTL_DeleteHandle (vload_H);	    
}
void SetAllPin2OneLevel(char *pinlist, float value)
{

		static ViHandle vi_h = -1;
		if(vi_h == -1) {
				vi_h = UTL_GetViHandle();
		}
		UTL_SetViHigh(vi_h, value);
		UTL_SetViLow (vi_h, -0.5);
		UTL_SendVi   (vi_h, 1);

		static PinHandle p_h = -1;
		if(p_h == -1) {
				p_h = UTL_GetPinHandle();
		}
		UTL_SetPinDrWaveform   (p_h, UT_WAV_FIXH);
		UTL_SetPinVihhNumber   (p_h, UT_PIN_NONE);	
		UTL_SetPinViNumber     (p_h, 1);
		UTL_SendPin            (p_h, pinlist);
}
void setPinLevelFixh(char *pinlist){
	LevelFixHandle level_fix_handle = UTL_GetLevelFixHandle();
	UT_PIN pin=0;
	PinCursor pc=UTL_GetPinCursor(pinlist);
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_FIXH);
	while((pin = UTL_NextPin(pc))!=UT_NOMORE){
		START_DUT_LOOP(UT_CDUT)
			UTL_SendLevelFix(level_fix_handle, pin, dut);
		END_DUT_LOOP
	}
	UTL_DeleteHandle  (level_fix_handle);
	UTL_DeleteCursor(pc);
}
void setPinLevelFixh(char *pinlist,int dut){
	LevelFixHandle level_fix_handle = UTL_GetLevelFixHandle();
	UT_PIN pin=0;
	PinCursor pc=UTL_GetPinCursor(pinlist);
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_FIXH);
	while((pin = UTL_NextPin(pc))!=UT_NOMORE){
		UTL_SendLevelFix(level_fix_handle, pin, dut);
	}
	UTL_DeleteHandle  (level_fix_handle);
	UTL_DeleteCursor(pc);
}
void setPinLevelNone(char *pinlist){
	LevelFixHandle level_fix_handle = UTL_GetLevelFixHandle();
	UT_PIN pin=0;
	PinCursor pc=UTL_GetPinCursor(pinlist);
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_NONE);
	while((pin = UTL_NextPin(pc))!=UT_NOMORE){
		START_DUT_LOOP(UT_CDUT)
			UTL_SendLevelFix(level_fix_handle, pin, dut);
		END_DUT_LOOP
	}
	UTL_DeleteHandle  (level_fix_handle);
	UTL_DeleteCursor(pc);
}
void setPinLevelNone(char *pinlist,int dut){
	LevelFixHandle level_fix_handle = UTL_GetLevelFixHandle();
	UT_PIN pin=0;
	PinCursor pc=UTL_GetPinCursor(pinlist);
	UTL_SetLevelFixOutputLevel(level_fix_handle, UT_PIN_DR, UT_PIN_NONE);
	while((pin = UTL_NextPin(pc))!=UT_NOMORE){
			UTL_SendLevelFix(level_fix_handle, pin, dut);
	}
	UTL_DeleteHandle  (level_fix_handle);
	UTL_DeleteCursor(pc);

}
void setPinPds( const char *pinlist, int datamux)
{
	PdsMemorySelHandle	pds_memory_sel_handle;
	pds_memory_sel_handle = UTL_GetPdsMemorySelHandle();
	UTL_SetPdsMemorySelPinList( pds_memory_sel_handle, (char *)pinlist   );
	UTL_SetPdsMemorySelDataMux( pds_memory_sel_handle, UT_PORT_A, datamux);
	UTL_SetPdsMemorySelDataMux( pds_memory_sel_handle, UT_PORT_B, datamux);
	UTL_ClearPdsMemorySelDut  ( pds_memory_sel_handle                    );
	UTL_SendPdsMemorySel      ( pds_memory_sel_handle                    );
	UTL_DeleteHandle          ( pds_memory_sel_handle                    );
}
void setPdsMemoryPat( PdsMemoryHandle pds_memory_handle, int pds_adr, int mut_a, int mut_b, int mut_c, int mut_d)
{
	UTL_SetPdsMemoryPat       ( pds_memory_handle, pds_adr, UT_PDS_DATAMUX_A, mut_a);
	UTL_SetPdsMemoryPat       ( pds_memory_handle, pds_adr, UT_PDS_DATAMUX_B, mut_b);
	UTL_SetPdsMemoryPat       ( pds_memory_handle, pds_adr, UT_PDS_DATAMUX_C, mut_c);
	UTL_SetPdsMemoryPat       ( pds_memory_handle, pds_adr, UT_PDS_DATAMUX_D, mut_d);
}
//--part3----------------------- DC settings ---------------------
//--#---------------------------------------------------------------------------------------
//--# DC:VRange
//--#---------------------------------------------------------------------------------------
//--#  20V  : -1.0V ~ +13.0V   Reso=2mV
//--#---------------------------------------------------------------------------------------
//--# DC:IRange
//--#---------------------------------------------------------------------------------------
//--#  20uA : -20uA  ~ +20uA  ( -1.0V ~ +13.0V) Reso=2.5nA
//--# 200uA : -200uA ~ +200uA ( -1.0V ~ +13.0V) Reso=25nA
//--#   2mA : -2mA   ~ +2mA   ( -1.0V ~ +13.0V) Reso=250nA
//--#  20mA : -10mA  ~ +10mA  ( -1.0V ~   0.0V) Reso=10uA
//--#       : -20mA  ~ +20mA  (  0.0V ~  +9.0V) Reso=10uA
//--#       : -5mA   ~ +5mA   ( +1.0V ~ +12.5V) Reso=10uA
//--#       : -2.5mA ~ +2.5mA (+12.5V ~ +13.0V) Reso=10uA
//--#---------------------------------------------------------------------------------------
//--@d	enum eDC_VRNG { R20V };
//--@d	enum eDC_IRNG { R5uA, R20uA, R200uA, R2mA, R20mA, Rm20mA };

//----------------------------------------------------
//--T5830
//----------------------------------------------------
//--VRange :  -1V    ~ +13V
//--IRange :  -20uA  ~ +20uA    Reso= 2.5nA 
//--          -200uA ~ +200uA   Reso=  25nA 
//--          -2mA   ~ +2mA     Reso= 250nA 
//--          -20mA  ~ +20mA    Reso=(-0.3V ` +13V) 10uA 
//--          -10mA  ~ +10mA    Reso=(-1V ` +0.3V) 10uA 
//----------------------------------------------------
void setDcVSIM( double sval, enum eDC_VRNG srng, enum eDC_IRNG mrng, double pclamp, double mclamp, double hlimit, double llimit, int cnt)
{
	DcHandle hDc = UTL_GetDcHandle();
	UTL_SetDcMode     ( hDc, UT_DCT_VSIM );
	UTL_SetDcSource   ( hDc, sval );

	if( srng==R20V )
		UTL_SetDcSrange   ( hDc, 13.0, -1.0 );
	else { fprintf(stderr, "Unsupported srange is set on setDcVSIM()." ); UTL_Stop(); }

	if     ( mrng==R5uA   ) UTL_SetDcMrange   ( hDc,   5.0e-6,   -5.0e-6 );
	else if( mrng==R20uA  ) UTL_SetDcMrange   ( hDc,  20.0e-6,  -20.0e-6 );
	else if( mrng==R200uA ) UTL_SetDcMrange   ( hDc, 200.0e-6, -200.0e-6 );
	else if( mrng==R2mA   ) UTL_SetDcMrange   ( hDc,   2.0e-3,   -2.0e-3 );
	else if( mrng==R20mA  ) UTL_SetDcMrange   ( hDc,  20.0e-3,  -20.0e-3 );
	else{ fprintf(stderr, "Unsupported mrange is set on setDcVSIM()." ); UTL_Stop(); }

	UTL_SetDcPclamp   ( hDc, pclamp );
	UTL_SetDcMclamp   ( hDc, mclamp );

	UTL_SetDcLimitHigh( hDc, hlimit, UT_ON );
	UTL_SetDcLimitLow ( hDc, llimit, UT_ON );
	UTL_SetDcMeasCnt  ( hDc, cnt );
	UTL_AddDcWetItem      ( hDc, UT_DCT_MRANGE     );
	SendDc_ForceWet( hDc) ;
	UTL_DeleteHandle  ( hDc );
}
void setDcISVM( double sval, enum eDC_IRNG srng, enum eDC_VRNG mrng, double pclamp, double mclamp, double hlimit, double llimit, int cnt)
{

	DcHandle hDc = UTL_GetDcHandle();
	UTL_SetDcMode     ( hDc, UT_DCT_ISVM );
	UTL_SetDcSource   ( hDc, sval );

	if     ( srng==R20uA  ) UTL_SetDcSrange   ( hDc,  20.0e-6,  -20.0e-6 );
	else if( srng==R200uA ) UTL_SetDcSrange   ( hDc, 200.0e-6, -200.0e-6 );
	else if( srng==R2mA   ) UTL_SetDcSrange   ( hDc,   2.0e-3,   -2.0e-3 );
	else if( srng==R20mA  ) UTL_SetDcSrange   ( hDc,  20.0e-3,  -20.0e-3 );
	else if( srng==Rm20mA ) UTL_SetDcSrange   ( hDc,  10.0e-3,  -10.0e-3 );
	else{ fprintf(stderr, "Unsupported srange is set on setDcISVM()." ); UTL_Stop(); }

	if( mrng==R20V )
		UTL_SetDcMrange   ( hDc, 13.0, -0.3 );
	else if( mrng==Rm20V )
		UTL_SetDcMrange   ( hDc,  0.3, -1.8 );
	else{ fprintf(stderr, "Unsupported mrange is set on setDcIS()." ); UTL_Stop(); }

	UTL_SetDcPclamp   ( hDc, pclamp );
	UTL_SetDcMclamp   ( hDc, mclamp );

	UTL_SetDcLimitHigh( hDc, hlimit, UT_ON );
	UTL_SetDcLimitLow ( hDc, llimit, UT_ON );
	UTL_SetDcMeasCnt  ( hDc, cnt );
	UTL_AddDcWetItem      ( hDc, UT_DCT_MRANGE     );
	SendDc_ForceWet( hDc) ;
	UTL_DeleteHandle  ( hDc );
}

//--part4----------------------- PPS&HV settings ---------------------
//---------------------------------------------------------------
//--T5830  
//---------------------------------------------------------------
//--  SRng  : SVal           
//---------------------------------------------------------------
//--     2V : 0.0V ~ 2.2V   
//--     3V : 0.0V ~ 3.3V   
//--     4V : 0.0V ~ 4.0V   
//--    16V :-1.0V ~13.0V   
//---------------------------------------------------------------
//--  MRng  : SRng             SVal
//---------------------------------------------------------------
//--   40uA :  2V,3V,4V,16V : +/-FSR
//--  400uA :               
//--    4mA :               
//--   40mA :               
//---------------------------------------------------------------
//--  400mA :  2V,3V,4V     : +/-FSR
//--        : 16V           : 8V~13V
//--        :               : 0V~7.999V
//--        :               :-1V~0V
//---------------------------------------------------------------
//--     8A :  2V           : 0.7V~2.2V
//--        :               : 0.0V~0.699V
//--        :  3V           : 2.0V~3.3V
//--        :               : 0.0V~1.999V
//--        :  4V           : 2.6V~4.0V
//--        :               : 0.0V~2.599V
//--        : 16V           : not supported 
//---------------------------------------------------------------
void setVs(int vsno, double val, enum ePPS_SRNG srng, enum ePPS_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt)
{
	double sRngMax;
	double sRngMin;
	double mRngMax;
	double mRngMin;
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_VSIM);
	UTL_SetVsMeasCnt (Vs_H, cnt);
	UTL_SetVsSource  (Vs_H, val);
	switch( srng )
	{
	case R2V  : sRngMax= 2.2; sRngMin= 0.0; break;
	case R3V  : sRngMax= 3.3; sRngMin= 0.0; break;
	case R4V  : sRngMax= 4.0; sRngMin= 0.0; break;
	case R16V : sRngMax=13.0; sRngMin=-1.0; break;
	default   : fprintf( stderr, "Out of range : VCC : SRng\n" ); UTL_Stop(); break;
	}

	switch( mrng )
	{
	case M40uA  : mRngMax=25e-6;  mRngMin=-25e-6;  break;
	case M400uA : mRngMax=250e-6; mRngMin=-250e-6; break;
	case M4mA   : mRngMax=2.5e-3; mRngMin=-2.5e-3; break;
	case M40mA  : mRngMax=25e-3;  mRngMin=-25e-3;  break;
	case M400mA : mRngMax=400e-3; mRngMin=-200e-3; break;
	case M1200mA : mRngMax=1200e-3; mRngMin=-200e-3; break;
	default   : fprintf( stderr, "Out of range : VCC : MRng\n" ); UTL_Stop(); break;
	}

	UTL_SetVsSrange  (Vs_H, sRngMax, sRngMin );
	UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );
	UTL_SetVsPclamp  (Vs_H, pclamp );
	UTL_SetVsMclamp  (Vs_H, mclamp );
	UTL_AddVsWetItem      ( Vs_H, UT_DCT_MRANGE     );
	
	UTL_SetVsPhaseCompensation(Vs_H, UT_DCT_C1);

	UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
	UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );

	UTL_SendVs       (Vs_H, vsno);
	UTL_DeleteHandle (Vs_H);
}

//---------------------------------------------------------------
//--T5830  (VSIM)
//---------------------------------------------------------------
//--  SRng  : SVal           
//---------------------------------------------------------------
//--     8V :  -8V ~ 8V
//--    32V : -10V ~ 32V
//---------------------------------------------------------------
//--  MRng  : 
//---------------------------------------------------------------
//--    8uA :   -8uA ~   8uA
//--   80uA :  -80uA ~  80uA
//--  800uA : -800uA ~ 800uA
//--    8mA :   -8mA ~   8mA
//--  128mA :  -80mA ~  90mA
//---------------------------------------------------------------
//---------------------------------------------------------------
//--T5830  (MVM)
//---------------------------------------------------------------
//--  MRng  : 
//---------------------------------------------------------------
//--    +8V :  -0.3V ~  8.0V
//--    -8V :  -8.0V ~  0.3V
//--   +32V :  -0.3V ~ 32.0V
//--   -32V : -10.0V ~  0.3V
//---------------------------------------------------------------
void setHvVSIM(char *pinlist, double val, enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt)
{
        double sRngMax;
        double sRngMin;
        double mRngMax;
        double mRngMin;
        VsHandle Vs_H = UTL_GetVsHandle();
        UTL_SetVsMode    (Vs_H, UT_DCT_VSIM);
        UTL_SetVsMeasCnt (Vs_H, cnt);
        UTL_SetVsSource  (Vs_H, val);
        switch( srng )
        {
        case R8V  : sRngMax=  8.0; sRngMin=  -8.0; break;
        case R32V : sRngMax= 32.0; sRngMin= -10.0; break;
        default   : fprintf( stderr, "%s : Out of range : VCC : SRng\n", __FUNCTION__ ); UTL_Stop(); break;
        }

        switch( mrng )
        {
        case M8uA   : mRngMax=  8e-6; mRngMin=  -8e-6;  break;
        case M80uA  : mRngMax= 80e-6; mRngMin= -80e-6;  break;
        case M800uA : mRngMax=800e-6; mRngMin=-800e-6;  break;
        case M8mA   : mRngMax=  8e-3; mRngMin=  -8e-3;  break;
        case M128mA : mRngMax= 90e-3; mRngMin= -80e-3;  break;
        default   : fprintf( stderr, "%s : Out of range : VCC : MRng\n", __FUNCTION__ ); UTL_Stop(); break;
        }

        UTL_SetVsSrange  (Vs_H, sRngMax, sRngMin );
        UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );

        UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
        UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );
        UTL_SetVsPclamp  (Vs_H, pclamp );
        UTL_SetVsMclamp  (Vs_H, mclamp );
        UTL_AddVsWetItem      ( Vs_H, UT_DCT_MRANGE     );

        PinCursor pincursor;
        int vsno=1;
        pincursor = UTL_GetPinCursor(pinlist);
        while((vsno=UTL_NextPin(pincursor))!=UT_NOMORE){
                SendVs_ForceWet       (Vs_H, vsno);
        }
        UTL_DeleteHandle (Vs_H);
        UTL_DeleteCursor (pincursor);
}



void setHvVSIM(int vsno, double val, enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt)
{
	double sRngMax;
	double sRngMin;
	double mRngMax;
	double mRngMin;
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_VSIM);
	UTL_SetVsMeasCnt (Vs_H, cnt);
	UTL_SetVsSource  (Vs_H, val);
	switch( srng )
	{
	case R8V  : sRngMax=  8.0; sRngMin=  -8.0; break;
	case R32V : sRngMax= 32.0; sRngMin= -10.0; break;
	default   : fprintf( stderr, "%s : Out of range : VCC : SRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	switch( mrng )
	{
	case M8uA   : mRngMax=  8e-6; mRngMin=  -8e-6;  break;
	case M80uA  : mRngMax= 80e-6; mRngMin= -80e-6;  break;
	case M800uA : mRngMax=800e-6; mRngMin=-800e-6;  break;
	case M8mA   : mRngMax=  8e-3; mRngMin=  -8e-3;  break;
	case M128mA : mRngMax= 90e-3; mRngMin= -80e-3;  break;
	default   : fprintf( stderr, "%s : Out of range : VCC : MRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	UTL_SetVsSrange  (Vs_H, sRngMax, sRngMin );
	UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );

	UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
	UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );
	UTL_SetVsPclamp  (Vs_H, pclamp );
	UTL_SetVsMclamp  (Vs_H, mclamp );
	UTL_AddVsWetItem      ( Vs_H, UT_DCT_MRANGE     );

	SendVs_ForceWet(Vs_H, vsno);
	UTL_DeleteHandle (Vs_H);
}

void setHvVSIM_ByFlashLoop(int vsno, double val[][FLASH_NUM], enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt,int flash_no)
{
	double sRngMax;
	double sRngMin;
	double mRngMax;
	double mRngMin;
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_VSIM);
	UTL_SetVsMeasCnt (Vs_H, cnt);
	switch( srng )
	{
	case R8V  : sRngMax=  8.0; sRngMin=  -8.0; break;
	case R32V : sRngMax= 32.0; sRngMin= -10.0; break;
	default   : fprintf( stderr, "%s : Out of range : VCC : SRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	switch( mrng )
	{
	case M8uA   : mRngMax=  8e-6; mRngMin=  -8e-6;  break;
	case M80uA  : mRngMax= 80e-6; mRngMin= -80e-6;  break;
	case M800uA : mRngMax=800e-6; mRngMin=-800e-6;  break;
	case M8mA   : mRngMax=  8e-3; mRngMin=  -8e-3;  break;
	case M128mA : mRngMax= 90e-3; mRngMin= -80e-3;  break;
	default   : fprintf( stderr, "%s : Out of range : VCC : MRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	UTL_SetVsSrange  (Vs_H, sRngMax, sRngMin );
	UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );

	UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
	UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );
//--	UTL_SetVsPclamp  (Vs_H, pclamp );
//--	UTL_SetVsMclamp  (Vs_H, mclamp );
	UTL_AddVsWetItem      ( Vs_H, UT_DCT_MRANGE     );

	START_DUT_LOOP(UT_CDUT)
		UTL_SetVsSource  (Vs_H, val[dut-1][flash_no-1]);
		SendVs_ForceWet       (Vs_H, vsno);//--preset vpp to specified mode to aoid srof
		break;
	END_DUT_LOOP
	START_DUT_LOOP(UT_CDUT)
		UTL_SetVsSource  (Vs_H, val[dut-1][flash_no-1]);
		UTL_SendVsTargetDut(Vs_H,vsno,dut);
//--		printf("\ndut %d set vdd:%.3fv",dut,val[(dut-1)][FLASH_NUM+flash_no-1]); 
	END_DUT_LOOP


	UTL_DeleteHandle (Vs_H);
}


void setHvMVM(char *pinlist,enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt)
{
	double mRngMax;
	double mRngMin;
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_MVM);
	UTL_SetVsMeasCnt (Vs_H, cnt);

	switch( mrng )
	{
	case M8V    : mRngMax=  8.00; mRngMin=  -0.30;  break;
	case Mm8V   : mRngMax=  0.30; mRngMin=  -8.00;  break;
	case M32V   : mRngMax= 32.00; mRngMin=  -0.30;  break;
	case Mm32V  : mRngMax=  0.30; mRngMin= -10.00;  break;
	default   : fprintf( stderr, "%s : Out of range : VCC : MRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );

	UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
	UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );

	PinCursor pincursor;
	int vsno=1;
	pincursor = UTL_GetPinCursor(pinlist);
	while((vsno=UTL_NextPin(pincursor))!=UT_NOMORE){
		SendVs_ForceWet       (Vs_H, vsno);
	}
	UTL_DeleteCursor(pincursor);
	UTL_DeleteHandle (Vs_H);
}


void setHvMVM(int vsno,enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt)
{
	double mRngMax;
	double mRngMin;
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_MVM);
	UTL_SetVsMeasCnt (Vs_H, cnt);

	switch( mrng )
	{
	case M8V    : mRngMax=  8.00; mRngMin=  -0.30;  break;
	case Mm8V   : mRngMax=  0.30; mRngMin=  -8.00;  break;
	case M32V   : mRngMax= 32.00; mRngMin=  -0.30;  break;
	case Mm32V  : mRngMax=  0.30; mRngMin= -10.00;  break;
	default   : fprintf( stderr, "%s : Out of range : VCC : MRng\n", __FUNCTION__ ); UTL_Stop(); break;
	}

	UTL_SetVsMrange  (Vs_H, mRngMax, mRngMin );

	UTL_SetVsLimitHigh    ( Vs_H, upperLimit, UT_ON );
	UTL_SetVsLimitLow     ( Vs_H, lowerLimit, UT_ON );
	SendVs_ForceWet(Vs_H, vsno);

	UTL_DeleteHandle (Vs_H);
}
void setHvMask(int vsno)
{
	VsMaskHandle hvsmask= UTL_GetVsMaskHandle();
	START_DUT_LOOP(UT_CDUT)                        
		UTL_AddVsMaskDutVsMask(hvsmask,dut,vsno,UT_ON);	//--mask eelay (disconnect eelay)		
	END_DUT_LOOP
	UTL_SendVsMask(hvsmask);                                        
	UTL_DeleteHandle(hvsmask);	

}


void setHvMask(char *pinlist)
{
        PinCursor pincursor;
        int vsno=1;
        pincursor = UTL_GetPinCursor(pinlist);
        VsMaskHandle hvsmask= UTL_GetVsMaskHandle();
        while((vsno=UTL_NextPin(pincursor))!=UT_NOMORE){
        START_DUT_LOOP(UT_CDUT)                        
                UTL_AddVsMaskDutVsMask(hvsmask,dut,vsno,UT_ON); //--mask delay (disconnect delay)         
        END_DUT_LOOP
        }                                
        UTL_SendVsMask(hvsmask);
        UTL_DeleteHandle(hvsmask);
        UTL_DeleteCursor(pincursor);    

}

void setHvUnMask(char *pinlist)
{
        PinCursor pincursor;
        int vsno=1;
        pincursor = UTL_GetPinCursor(pinlist);
        VsMaskHandle hvsmask= UTL_GetVsMaskHandle();
        while((vsno=UTL_NextPin(pincursor))!=UT_NOMORE){
        START_DUT_LOOP(UT_CDUT)                        
                UTL_AddVsMaskDutVsMask(hvsmask,dut,vsno,UT_OFF);        //--unmask relay (connect relay)         
        END_DUT_LOOP
        }              
        UTL_SendVsMask(hvsmask);        
        UTL_DeleteHandle(hvsmask);
        UTL_DeleteCursor(pincursor);
}

void setHvUnMask(int vsno)
{
	VsMaskHandle hvsmask= UTL_GetVsMaskHandle();
	START_DUT_LOOP(UT_CDUT)                        
		UTL_AddVsMaskDutVsMask(hvsmask,dut,vsno,UT_OFF);			
	END_DUT_LOOP
	UTL_SendVsMask(hvsmask);                                        
	UTL_DeleteHandle(hvsmask);	

}

//--part5----------------------- DC Exec ---------------------
void ulSetSettlingTime(double t,double t1,double t2)//--t:MVM/VSIM t1/t2:ISVM
{
	SettlingTimeHandle h = UTL_GetSettlingTimeHandle();
    	UTL_SetSettlingTime(h, t);
    	UTL_SetSettlingTimeAfterRon(h, t1);
    	UTL_SetSettlingTimeAfterRof(h,  t2);
    	UTL_SendSettlingTime(h);
    	UTL_DeleteHandle(h);
}

void DcExec(char* pinname)
{
	DctHandle hDct = UTL_GetDctHandle();
	UTL_SetDctPinList	( hDct,  pinname);
	UTL_SetDctRelayMode	( hDct,  UT_OFF );
	UTL_MeasDct		( hDct );
	UTL_DeleteHandle	( hDct );
}
//--part6----------------------- Pattern Exec--------------------
void setRegValue(RadioButton reg, USlider value)	
{
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs(h, reg, 1 ,value);
	UTL_SendReg(h);
	UTL_DeleteHandle(h);
}
void setRegValue2(RadioButton reg, USlider value)	
{
	RegHandle h = UTL_GetRegHandle();
	UTL_SetRegUs(h, reg, 2,value);
	UTL_SendReg(h);
	UTL_DeleteHandle(h);
}

void AlpgPatExec( char *patfile, char *startlabel)
{
	char pat_path[50] ;
	USlider pc;
	RadioButton rtn;
	sprintf(pat_path, "pat/%s", patfile);
	if(strcmp(mpa_name,patfile)!=0)
	{
		sprintf(mpa_name,"%s",patfile);
		MpatHandle Mpat_h = UTL_GetMpatHandle();
		UTL_SetMpatFileName(Mpat_h,pat_path);
		UTL_SendMpat	   (Mpat_h);
		UTL_DeleteHandle   (Mpat_h);

	}
	ReadMpatPcHandle pch = UTL_GetReadMpatPcHandle();
	UTL_SetReadMpatStartName      (pch, startlabel);
	UTL_SetReadMpatFileName       (pch, pat_path);
	rtn = UTL_ReadMpatStartPc     (pch, &pc);
	if(rtn == UT_NOT_FOUND )
	{
		printf("runtest_pattern:  start label \"%s\" was not found in pat file %s\n", startlabel, patfile);
		assert(rtn == UT_FOUND);
	}
	UTL_DeleteHandle(pch);

	if(FK[1]==1){
		printf("\nRUNTEST_PATTERN: START_LABEL\t%s \n", startlabel);
	}

	FctHandle h = UTL_GetFctHandle();
	UTL_SetFctFailInhibit(h, UT_ON);
	UTL_SetFctMpatName   (h, NULL);
	UTL_SetFctStartPc    (h, pc);
	UTL_MeasFct(h);
	UTL_DeleteHandle(h);
}

void AlpgPatStartNowait(char *patfile, char *startlabel)
{
	char pat_path[50];
	USlider pc;
	RadioButton rtn;
	sprintf(pat_path, "pat/%s", patfile);
	if(strcmp(mpa_name,patfile)!=0)
	{
		sprintf(mpa_name,"%s",patfile);
		MpatHandle Mpat_h = UTL_GetMpatHandle();
		UTL_SetMpatFileName(Mpat_h,pat_path);
		UTL_SendMpat	   (Mpat_h);
		UTL_DeleteHandle   (Mpat_h);

	}
	ReadMpatPcHandle pch = UTL_GetReadMpatPcHandle();
	UTL_SetReadMpatStartName      (pch, startlabel);
	UTL_SetReadMpatFileName       (pch, pat_path);
	rtn = UTL_ReadMpatStartPc     (pch, &pc);
	if(rtn == UT_NOT_FOUND )
	{
		printf("runtest_pattern:  start label \"%s\" was not found in pat file %s\n", startlabel, patfile);
		assert(rtn == UT_FOUND);
	}
	UTL_DeleteHandle(pch);

	if(FK[1]==1){
		printf("\nSTART_PATTERN_NOWAIT: START_LABEL\t%s \n", startlabel);
	}

	FctHandle h = UTL_GetFctHandle();
	UTL_SetFctFailInhibit(h, UT_ON);
	UTL_SetFctMpatName   (h, NULL);
	UTL_SetFctStartPc    (h, pc);
	UTL_SetFctStartPc    (h, pc);
	UTL_SetFctNoWait(h, UT_ON);
	UTL_StartFct(h);
	UTL_DeleteHandle(h);
}
void AlpgPatStart(char *patfile, char *startlabel)
{
	char pat_path[50] ;
	USlider pc;
	RadioButton rtn;
	sprintf(pat_path, "pat/%s", patfile);
	if(strcmp(mpa_name,patfile)!=0)
	{
		sprintf(mpa_name,"%s",patfile);
		MpatHandle Mpat_h = UTL_GetMpatHandle();
		UTL_SetMpatFileName(Mpat_h,pat_path);
		UTL_SendMpat	   (Mpat_h);
		UTL_DeleteHandle   (Mpat_h);

	}
	ReadMpatPcHandle pch = UTL_GetReadMpatPcHandle();
	UTL_SetReadMpatStartName      (pch, startlabel);
	UTL_SetReadMpatFileName       (pch, pat_path);
	rtn = UTL_ReadMpatStartPc     (pch, &pc);
	if(rtn == UT_NOT_FOUND )
	{
		printf("runtest_pattern:  start label \"%s\" was not found in pat file %s\n", startlabel, patfile);
		assert(rtn == UT_FOUND);
	}
	UTL_DeleteHandle(pch);

	if(FK[1]==1){
		printf("\nSTART_PATTERN: START_LABEL\t%s \n", startlabel);
	}

	FctHandle h =UTL_GetFctHandle();
	UTL_SetFctFailInhibit(h, UT_ON);
	UTL_SetFctMpatName   (h, NULL);
	UTL_SetFctStartPc    (h, pc);
	UTL_SetFctNoWait(h, UT_OFF);
	UTL_StartFct(h);
	UTL_DeleteHandle(h);
}

//--part7-----------------------Test Result ---------------------/
int GetFinalResult(int dut)
{
	if(UTL_ReadFinalResult(dut,UT_RES_ALL) == UT_RES_PASSED)
	{
		return PASS;
	}
	else if(UTL_ReadFinalResult(dut,UT_RES_ALL) == UT_RES_NOT_TESTED)
	{
		return NOTTEST;
	}
	else
	{
		return FAIL;
	}

}

int SetFinalResultOnly(int dut, int result)
{
	if(result == FAIL)
	{
		UTL_SetFinalResultOnly(dut, UT_RES_ALL);
	}
	else
	{
		UTL_ResetFinalResultOnly(dut, UT_RES_ALL);
	}

	return 0;
}

void TransMeasData(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr, double *data,int dut)
{

	int ofdata[DDUTCNT];
	UT_DUT sysdut;
		PinCursor pincur;
		UT_PIN pin;
		DctReadPinHandle h = UTL_GetDctReadPinHandle();
		UTL_SetDctReadPinDut   (h, dut);
		RadioButton pintype; 
		UTL_GetPinType(pinlist, &pintype);
		UTL_SetDctReadPinType  (h, pintype);
		UTL_SetDctReadPinMode  (h,UT_RES_OVERFLOW); //--for overflow pin
		pincur = UTL_GetPinCursor(pinlist);
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		pin=UTL_NextPin(pincur);
		UTL_SetDctReadPinNumber(h, pin);
		data[dut-1] = UTL_ReadDctPinData(h)/(unit);
		ofdata[dut-1] = UTL_ReadDctPin(h); //--for overflow pin
		if(UTL_ReadDctPin(h)) //--for overflow pin
		{printf("\nDUT%d over flow, %.4f",dut,data[dut-1]);}
	UTL_DeleteHandle(h);
	UTL_DeleteCursor(pincur);

}
	
void Read_Meas_Result2(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr)
{
	int ofdata[DDUTCNT];
	double data[DDUTCNT];
	UT_DUT sysdut;
	START_DUT_LOOP(UT_CDUT)
	{

		PinCursor pincur;
		UT_PIN pin;
		DctReadPinHandle h = UTL_GetDctReadPinHandle();
		UTL_SetDctReadPinDut   (h, dut);
		RadioButton pintype; 
		UTL_GetPinType(pinlist, &pintype);
		UTL_SetDctReadPinType  (h, pintype);
		UTL_SetDctReadPinMode  (h,UT_RES_OVERFLOW); 
		pincur = UTL_GetPinCursor(pinlist);
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);

		while((pin=UTL_NextPin(pincur))!=UT_NOMORE){
			UTL_SetDctReadPinNumber(h, pin);
			data[dut-1] = UTL_ReadDctPinData(h);
			ofdata[dut-1] = UTL_ReadDctPin(h); 
			if(pintype == UT_PINTYPE_VS){

				if(ofdata[dut-1] !=0)
					printf((char *)"[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\tof\t%s\tSITE%d\tSDUT%d\t*\n", dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,unitstr,UTL_ReadSiteNumber(),sysdut);
				else if(data[dut-1]<llimit||data[dut-1]>hlimit)
					printf((char *)"x:%d\ty:%d\t[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.2f %.3f\t%.2f  %s\tSITE%d\tSDUT%d\tF\n",g_die_X[dut-1],g_die_Y[dut-1],dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,llimit/(unit),data[dut-1]/(unit),hlimit/(unit), unitstr,UTL_ReadSiteNumber(),sysdut);
				else
					printf((char *)"x:%d\ty:%d\t[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.2f %.3f\t%.2f  %s\tSITE%d\tSDUT%d\tP\n",g_die_X[dut-1],g_die_Y[dut-1],dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,llimit/(unit),data[dut-1]/(unit),hlimit/(unit), unitstr,UTL_ReadSiteNumber(),sysdut);

			}
			else{

				if(ofdata[dut-1] !=0)
					printf((char *)"\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\tof\t%s\tSITE%d\tSDUT%d\t*\n",dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,unitstr,UTL_ReadSiteNumber(),sysdut);
				else if(data[dut-1]<llimit||data[dut-1]>hlimit)
					printf((char *)"x:%d\ty:%d\t\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.3f\t%.3f\t%.3f  %s\tSITE%d\tSDUT%d\tF\n",g_die_X[dut-1],g_die_Y[dut-1],dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,llimit/(unit),data[dut-1]/(unit), hlimit/(unit),unitstr,UTL_ReadSiteNumber(),sysdut);
				else
					printf((char *)"x:%d\ty:%d\t\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.3f\t%.3f\t%.3f  %s\tSITE%d\tSDUT%d\tP\n",g_die_X[dut-1],g_die_Y[dut-1],dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,llimit/(unit),data[dut-1]/(unit), hlimit/(unit),unitstr,UTL_ReadSiteNumber(),sysdut);

			}
		}
		UTL_DeleteCursor(pincur);
		UTL_DeleteHandle(h);
	}

	END_DUT_LOOP

}


void Read_Meas_Result(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr)	
{
	int ofdata[DDUTCNT];
	double data[DDUTCNT];
	UT_DUT sysdut;
	START_DUT_LOOP(UT_CDUT)
	{
		PinCursor pincur;
		UT_PIN pin;
		DctReadPinHandle h = UTL_GetDctReadPinHandle();
		UTL_SetDctReadPinDut   (h, dut);
		RadioButton pintype; 
		UTL_GetPinType(pinlist, &pintype);
		UTL_SetDctReadPinType  (h, pintype);
		UTL_SetDctReadPinMode  (h,UT_RES_OVERFLOW); //--for overflow pin
		pincur = UTL_GetPinCursor(pinlist);
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		while((pin=UTL_NextPin(pincur))!=UT_NOMORE){
			UTL_SetDctReadPinNumber(h, pin);
			data[dut-1] = UTL_ReadDctPinData(h);
			ofdata[dut-1] = UTL_ReadDctPin(h); //--for overflow pin
			if(pintype == UT_PINTYPE_VS){
				if(ofdata[dut-1] !=0)//--for overflow pin
					printf((char *)"[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\tof\t%s\tSITE%d\tSDUT%d\t*\n", dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,unitstr,UTL_ReadSiteNumber(),sysdut);//for overflow pin
				else if(data[dut-1]<llimit||data[dut-1]>hlimit)
					printf((char *)"[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.2f %.3f\t%.2f  %s\tSITE%d\tSDUT%d\tF\n",dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,llimit/(unit),data[dut-1]/(unit),hlimit/(unit), unitstr,UTL_ReadSiteNumber(),sysdut);
				else
					printf((char *)"[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.2f %.3f\t%.2f  %s\tSITE%d\tSDUT%d\tP\n",dut, test_item, pin, g_pinlist[pin].vsno,g_pinlist[pin].vsname,llimit/(unit),data[dut-1]/(unit),hlimit/(unit), unitstr,UTL_ReadSiteNumber(),sysdut);
			}
			else{
				if(ofdata[dut-1] !=0)//--for overflow pin
					printf((char *)"\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\tof\t%s\tSITE%d\tSDUT%d\t*\n",dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,unitstr,UTL_ReadSiteNumber(),sysdut);//for overflow pin
				else if(data[dut-1]<llimit||data[dut-1]>hlimit)
					printf((char *)"\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.3f\t%.3f\t%.3f  %s\tSITE%d\tSDUT%d\tF\n",dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,llimit/(unit),data[dut-1]/(unit), hlimit/(unit),unitstr,UTL_ReadSiteNumber(),sysdut);
				else
					printf((char *)"\n[D%2d]\t%s\tPIN%d\tCHN%d\t%8s\t%.3f\t%.3f\t%.3f  %s\tSITE%d\tSDUT%d\tP\n",dut, test_item, pin, g_pinlist[pin].pinno,g_pinlist[pin].pinname,llimit/(unit),data[dut-1]/(unit), hlimit/(unit),unitstr,UTL_ReadSiteNumber(),sysdut);
			}
		}
		UTL_DeleteCursor(pincur);	
		UTL_DeleteHandle(h);
	}	
	END_DUT_LOOP

}

int ReadMeasData_ByFlash(char *pinlist, double unit,double data[DDUTCNT][FLASH_NUM],int dut,int flash_no)
{

	int ofdata[DDUTCNT][FLASH_NUM];
	UT_DUT sysdut;
	PinCursor pincur;
	UT_PIN pin;
	DctReadPinHandle h = UTL_GetDctReadPinHandle();
	UTL_SetDctReadPinDut   (h, dut);
	RadioButton pintype; 
	UTL_GetPinType(pinlist, &pintype);
	UTL_SetDctReadPinType  (h, pintype);
	UTL_SetDctReadPinMode  (h,UT_RES_OVERFLOW); //--for overflow pin
	pincur = UTL_GetPinCursor(pinlist);
	UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
	pin=UTL_NextPin(pincur);
	UTL_SetDctReadPinNumber(h, pin);
	data[dut-1][flash_no-1] = UTL_ReadDctPinData(h)/(unit);
	ofdata[dut-1][flash_no-1] = UTL_ReadDctPin(h); //--for overflow pin
	if(UTL_ReadDctPin(h)) //--for overflow pin
	{printf("\n over flow,dut%d %.3fUA ",dut,data[dut-1][flash_no-1]);
		UTL_DeleteHandle(h);
		UTL_DeleteCursor(pincur);
		return 0;
	}
	UTL_DeleteHandle(h);
	UTL_DeleteCursor(pincur);
	return 	1;
}


int ReadMeasData(char *pinlist, double unit,double *data,int dut)
{

	int ofdata[DDUTCNT];
	UT_DUT sysdut;
		PinCursor pincur;
		UT_PIN pin;
		DctReadPinHandle h = UTL_GetDctReadPinHandle();
		UTL_SetDctReadPinDut   (h, dut);
		RadioButton pintype; 
		UTL_GetPinType(pinlist, &pintype);
		UTL_SetDctReadPinType  (h, pintype);
		UTL_SetDctReadPinMode  (h,UT_RES_OVERFLOW); //--for overflow pin
		pincur = UTL_GetPinCursor(pinlist);
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
		pin=UTL_NextPin(pincur);
		UTL_SetDctReadPinNumber(h, pin);
		data[dut-1] = UTL_ReadDctPinData(h)/(unit);
		ofdata[dut-1] = UTL_ReadDctPin(h); //--for overflow pin
		if(UTL_ReadDctPin(h)) //--for overflow pin
		{printf("\n over flow,dut%d %.3fUA ",dut,data[dut-1]);
	UTL_DeleteHandle(h);
	UTL_DeleteCursor(pincur);
		return 0;
		}
	UTL_DeleteHandle(h);
	UTL_DeleteCursor(pincur);
		return 	1;
}


int GetActiveDut(long long int *active_dut){
	*active_dut=0;
	START_DUT_LOOP(UT_CDUT)
		*active_dut|=(1<<(dut-1));
	END_DUT_LOOP
}

int ReturnActiveDut(long long int active_dut){
	ExclusionHandle hex=UTL_GetExclusionHandle();
	UTL_SetExclusionIgnoreWet  (hex,UT_OFF);                            
	UTL_SetExclusionMask       (hex,UT_OFF);		                    
	START_DUT_LOOP(UT_MDUT)
			if((1<<(dut-1))&active_dut){
				UTL_AddExclusionDut(hex,dut);
			}else{continue;}
	END_DUT_LOOP          
	UTL_SetExclusionSetOrReset (hex,UT_OFF);                             
	UTL_SendExclusion(hex);                                             
	UTL_DeleteHandle(hex);
}
//--part8----------------------- Data process ---------------------/
void getMidValue(double *source,int cnt, double *destination, int dut)
{
	qsort(source,cnt,sizeof(source[0]),compare); 
	destination[dut-1]= source[cnt/2];
		//--printf("\ndut%d id %d data %.3f",dut, cnt/2, destination[dut-1]);
}
int compare(const void* a, const void* b)
{
	if( (*(double*)a)-(*(double*)b)>0) return 1;

}

int ConvertCharToInt(char *lotnumber, int index)
{
	char id = lotnumber[index];
	if(index>=strlen(lotnumber))
		return 0;

	return id;
}
//--part9----------------------- Debug ---------------------/

int ReadFk(int *array)
{
	char svname[100];
	char *rdata, label[5], *value;
	UTSC_size_t size;
	UTSC_Pf pfdesc;
	UT_LKstream lks, lobj;
	int stn, st, ival = UT_OFF;
	int fknum;


	pfdesc = NULL;
	lks = NULL;

	if ((st = UTSC_Pf_Open(&pfdesc, NULL)) != 0) {
		printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
		goto END;
	}

	stn = UTL_ReadStationNumber();
	sprintf(svname, "UTPFV_SystemValue_FK%d", stn);
	if ((st = UTSC_Pf_ReadData(pfdesc, svname, (void **)&rdata, &size)) != 0) {
		printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
		goto END;
	}
	if ((st = UTHN_LKstream_Construct(&lks, rdata)) != UTHN_LKSTREAM_OK) {
		printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
		goto END;
	}
	for (fknum = 1; fknum <= 16; fknum++) {
		snprintf(label, sizeof(label), (char *)"FK%d", fknum);
		if ((st = UTHN_LKstream_FindLabel(lks, label, &lobj)) != UTHN_LKSTREAM_OK) {
			printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
			goto END;
		}
		if ((st = UTHN_LKstream_FindKey(lobj, (char *)"switch", &value)) != UTHN_LKSTREAM_OK) {
			printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
			goto END;
		}
		if (strcmp(value, (char *)"on") == 0)
			array[fknum] = UT_ON;
		else
			array[fknum] = UT_OFF;
	}

END:	if (lks) {
			if ((st = UTHN_LKstream_Destruct(lks)) != UTHN_LKSTREAM_OK) {
				printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
			}
		}
		if (pfdesc) {
			if ((st = UTSC_Pf_Close(pfdesc)) != 0) {
				printf((char *)"%s:%d %d\n", __FILE__, __LINE__, st);
			}
		}
		return ival;

}


void Pause2(char *fmt, ...)
{
	va_list ap;
	char buf[100];
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	fflush(stdout);
	fgets(buf, 100, stdin);
}

void Pause(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	fflush(stdout);
	UTL_CheckRepeatPause();
}

void DebugPrintf(char *fmt, ...)
{
	if(FK[2]==1){
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
		fflush(stdout);
	}
}
void VsOpenByList(char *pinlist)
{
	int vsno = 1;
	PinCursor pincursor;
	pincursor = UTL_GetPinCursor(pinlist);
	VsHandle Vs_H = UTL_GetVsHandle();
	UTL_SetVsMode    (Vs_H, UT_DCT_OPEN);
	UTL_SetVsLimitHigh    ( Vs_H, 0, UT_OFF );
	UTL_SetVsLimitLow     ( Vs_H, 0, UT_OFF );
	while((vsno=UTL_NextPin(pincursor))!=UT_NOMORE){
		UTL_SendVs(Vs_H, vsno);
	}
	UTL_DeleteHandle (Vs_H);
	UTL_DeleteCursor (pincursor);
}

