#include "common.h"

void ulConfigFcm(char *pinlist, int xbit, int ybit, int transfer_size)
{
	int bit, ad, pin;
	PinCursor pinc;
	FcmConfigHandle h = UTL_GetFcmConfigHandle();

	bit = 0;
	pinc = UTL_GetPinCursor(pinlist);
	while((pin = UTL_NextPin(pinc)) != UT_NOMORE)
	{
		UTL_AddFcmConfigPinAssign(h, bit++, pin);
	}
	UTL_SetFcmConfigPinAssignTarget(h,UT_FCM_FLEX_PINASSIGN);
	UTL_SetFcmConfigDutBitSize(h,bit);
	UTL_SetFcmConfigBitSize(h, bit);
	UTL_DeleteCursor(pinc);

	bit = 0;
	for (ad = 0; ad < ybit; ad++)
	{
		UTL_SetFcmConfigAddrAssign(h, bit++, UT_SIG_Y(ad));
	}
	for (ad = 0; ad < xbit; ad++)
	{
		UTL_SetFcmConfigAddrAssign(h, bit++, UT_SIG_X(ad));
	}
	UTL_AddFcmConfigAction(h, 1, UT_FCM_AF);
	UTL_SetFcmConfigRate(h, UT_FCM_LS);
	UTL_SetFcmConfigStoreSignalByMMType(h, UT_FCM_AF, UT_SIG_MM);
	UTL_SetFcmConfigTransferSize (h, transfer_size);
	UTL_SendFcmConfig(h);
	UTL_DeleteHandle(h);
}

void ulDisableFcm(void)
{
	FcmConfigHandle h = UTL_GetFcmConfigHandle();
	UTL_AddFcmConfigAction(h, 1, UT_FCM_DISABLE);
	UTL_SendFcmConfig(h);
	UTL_DeleteHandle(h);
}

void ulReadFcm(unsigned int *data, int size, int x_st, int x_sp, int y_st, int y_sp, int dut)
{
	assert ( data != NULL );
	assert ( x_sp>=x_st && x_st>=0 );
	assert ( y_sp>=y_st && y_st>=0 );
	int actual_size = ( x_sp - x_st +1 ) * ( y_sp - y_st +1 );
	assert ( size >= actual_size );

	static FcmAccessHandle h = -1;
	if (h == -1) {
		h = UTL_GetFcmAccessHandle();
		UTL_SetFcmAccessAutoAddrAssignMode(h, UT_ON);
		UTL_SetFcmAccessApLink(h, UT_FCM_APLINK_YXZ);
		UTL_SetFcmAccessAction            (h, 1);
		UTL_SetFcmAccessBitBlock          (h, 1);
		UTL_SetFcmAccessBitMode           (h, UT_FCM_BITMODE_BLOCK); 
		UTL_SetFcmAccessAllBit            (h, UT_ON);   
	}

	UTL_SetFcmAccessDut  (h, dut);
	UTL_SetFcmAccessXAddr(h, x_st, x_sp);
	UTL_SetFcmAccessYAddr(h, y_st, y_sp);
	UTL_ReadFcm(h, data, actual_size * sizeof(int));
}

void ulPresetFcm()
{
	FcmAccessHandle h = UTL_GetFcmAccessHandle();
	UTL_SetFcmAccessAutoAddrAssignMode(h, UT_ON);
	UTL_SetFcmAccessAllBit            (h, UT_ON);
	UTL_SetFcmAccessAllDut            (h, UT_ON);
	UTL_SetFcmAccessPresetAllMemory   (h, UT_OFF);
	UTL_PresetFcm                     (h, 0);
	UTL_DeleteHandle(h);
}

void ulPresetFcmAll()
{
	FcmAccessHandle h = UTL_GetFcmAccessHandle();
	UTL_SetFcmAccessAutoAddrAssignMode(h, UT_ON);
	UTL_SetFcmAccessAllBit            (h, UT_ON);
	UTL_SetFcmAccessAllDut            (h, UT_ON);
	UTL_SetFcmAccessPresetAllMemory   (h, UT_ON);
	UTL_PresetFcm                     (h, 0);
	UTL_DeleteHandle(h);
}

void ulConfigCfm(char *pinlist, int startxbit,int stopxbit,int startybit,int stopybit)
{
	int bit, ad, pin;
	bit =0;
	PinCursor pinc;
	FcmConfigHandle fcm_config = UTL_GetFcmConfigHandle();
	UTL_AddFcmConfigAction(fcm_config, 1, UT_FCM_DISABLE);
	UTL_SetFcmConfigPinAssignTarget(fcm_config,UT_FCM_FLEX_PINASSIGN);
	UTL_ClearFcmConfigPinAssign(fcm_config);
	UTL_SendFcmConfig(fcm_config);
	UTL_DeleteHandle(fcm_config);
	CfmConfigHandle h = UTL_GetCfmConfigHandle();
	pinc = UTL_GetPinCursor(pinlist);
	while((pin = UTL_NextPin(pinc)) != UT_NOMORE)
	{
		UTL_AddCfmConfigPinAssign(h, bit++, pin);
	}
	UTL_SetCfmConfigBitSize(h, bit);
	UTL_SetCfmConfigDutBitSize(h, bit);
	UTL_DeleteCursor(pinc);
	bit = 0;

	for (ad = startybit; ad < stopybit; ad++)
	{
		UTL_SetCfmConfigAddrAssign(h, bit++, UT_SIG_Y(ad));
	}
	for (ad = startxbit; ad < stopxbit; ad++)
	{
		UTL_SetCfmConfigAddrAssign(h, bit++, UT_SIG_X(ad));
	}
	


	UTL_ClearCfmConfigAction(h);
	UTL_AddCfmConfigAction(h, 1, UT_CFM_TOTALFAIL);
	UTL_SendCfmConfig(h);
	UTL_DeleteHandle(h);
}


void ulConfigCfm_MMA(char *pinlist, int startxbit,int stopxbit,int startybit,int stopybit)
{
        int bit, ad, pin;
        bit =0;
        PinCursor pinc;
        FcmConfigHandle fcm_config = UTL_GetFcmConfigHandle();
        UTL_AddFcmConfigAction(fcm_config, 1, UT_FCM_DISABLE);
        UTL_SetFcmConfigPinAssignTarget(fcm_config,UT_FCM_FLEX_PINASSIGN);
        UTL_ClearFcmConfigPinAssign(fcm_config);
        UTL_SendFcmConfig(fcm_config);
        UTL_DeleteHandle(fcm_config);
        CfmConfigHandle h = UTL_GetCfmConfigHandle();
        pinc = UTL_GetPinCursor(pinlist);
        while((pin = UTL_NextPin(pinc)) != UT_NOMORE)
        {
                UTL_AddCfmConfigPinAssign(h, bit++, pin);
        }
        UTL_SetCfmConfigBitSize(h, bit);
        UTL_SetCfmConfigDutBitSize(h, bit);
        UTL_DeleteCursor(pinc);
        bit = 0;

        for (ad = startybit; ad < stopybit; ad++)
        {
                UTL_SetCfmConfigAddrAssign(h, bit++, UT_SIG_Y(ad));
        }
        for (ad = startxbit; ad < stopxbit; ad++)
        {
                UTL_SetCfmConfigAddrAssign(h, bit++, UT_SIG_X(ad));
        }



        UTL_ClearCfmConfigAction(h);
//      UTL_AddCfmConfigAction(h, 1, UT_CFM_TOTALFAIL);
        UTL_AddCfmConfigAction(h, 1, UT_CFM_AF);
        UTL_SetCfmConfigStoreSignalByMMType(h, UT_CFM_AF, UT_SIG_MM);
        UTL_SendCfmConfig(h);
        UTL_DeleteHandle(h);
}

void ulReadCfm(unsigned int *data, int size, int x_st, int x_sp,int y_st, int y_sp, int dut)
{
	
	int bit, ad,xtemp=0,ytemp=0,x_sizebits=0,y_sizebits=0;
	bit = 0;
	assert ( data != NULL );
	assert ( x_sp>=x_st && x_st>=0 );
	assert ( y_sp>=y_st && y_st>=0 );
	int actual_size = ( x_sp - x_st +1 ) * ( y_sp - y_st +1 );
	assert ( size >= actual_size );
	
	xtemp=x_sp - x_st ;
	ytemp=y_sp - y_st ;
	while(xtemp!=0)
	{
		x_sizebits++;
		xtemp=xtemp>>1;
	}
	while(ytemp!=0)
	{
		y_sizebits++;
		ytemp=ytemp>>1;
	}
	
//--printf("x_sizebits=%d,y_sizebits=%d\n",x_sizebits,y_sizebits);

	 CfmAccessHandle h = UTL_GetCfmAccessHandle();
	for ( ad = 0; ad < y_sizebits; ad++)
	{
		UTL_SetCfmAccessAddrAssign(h, bit++, UT_SIG_Y(ad));
	}
	for ( ad = 0; ad < x_sizebits; ad++)	
	{
		UTL_SetCfmAccessAddrAssign(h, bit++, UT_SIG_X(ad));
	}
		UTL_SetCfmAccessApLink(h, UT_FCM_APLINK_YXZ);
		UTL_SetCfmAccessAction            (h, 1);
		UTL_SetCfmAccessBitBlock          (h, 1);
		UTL_SetCfmAccessBitMode           (h, UT_FCM_BITMODE_BLOCK); 
		UTL_SetCfmAccessAllBit            (h, UT_ON);  

	UTL_SetCfmAccessDut  (h, dut);
	UTL_SetCfmAccessXAddr(h, x_st, x_sp);
	UTL_SetCfmAccessYAddr(h, y_st, y_sp);
	UTL_ReadCfm(h, data, actual_size * sizeof(int));
	UTL_DeleteHandle(h);

}



void ulPresetCfm()
{
	CfmAccessHandle h = UTL_GetCfmAccessHandle();
	UTL_SetCfmAccessAllBit            (h, UT_ON);
	UTL_SetCfmAccessAllDut            (h, UT_ON);
	UTL_SetCfmAccessPresetAllMemory   (h, UT_ON);
	UTL_PresetCfm                     (h, 0);
	UTL_DeleteHandle(h);
}
