#include "common.h"
void dbm_reset()
{
	DbmAccessHandle	DbmAccess_H;
	DbmAccess_H = UTL_GetDbmAccessHandle();
	UTL_ResetDbm                (DbmAccess_H);
	UTL_DeleteHandle            (DbmAccess_H);
}

void dbmap_set_condition(char* pinlst, long bitmode)
{
	DbmConfigHandle	DbmConfig_H;
	PinCursor	pcr;
	UT_PIN		pin=0;
	int		i;
	int		dbmbit;

	DbmConfig_H = UTL_GetDbmConfigHandle();
	UTL_SetDbmConfigBitSize    (DbmConfig_H, bitmode);
	UTL_SetDbmConfigAccessMode (DbmConfig_H, UT_DBM_ACCESS_AP_AUTOXFER);	

	pcr=UTL_GetPinCursor(pinlst);
	int pinAry[64] = {};
	int pinCnt=0;
	while((pin=UTL_NextPin(pcr))!=UT_NOMORE)
	{
		pinAry[pinCnt++] = pin;
	}
	for( i=0; i<pinCnt; i++ )
	{
		if( i>bitmode )
		{
			printf("\n\n ERROR!!!!!!!! \n Target pinlist count exceeds bitmode!!\n\n");
			UTL_Stop();
		}
	//	printf("i:%d pinCnt:%d pinAry[%d] is %d\n",i,pinCnt,i,pinAry[i]);
		UTL_AddDbmConfigPinAssign (DbmConfig_H, pinCnt-1-i, pinAry[i]);
	}

	UTL_SendDbmConfig          (DbmConfig_H);
	UTL_DeleteHandle           (DbmConfig_H);
}


int dbmap_write( char *patName, unsigned int *dbm_data, int depth, int start, int dbm_bitmode)
{
	if(FirstRun)
	{
		DbmAccessHandle	DbmAccess_H;
		DbmAccess_H = UTL_GetDbmAccessHandle();
		UTL_SetDbmAccessChildFlag   (DbmAccess_H, 0x7);	
		UTL_SetDbmAccessDataKind    (DbmAccess_H, UT_DBM_PPAT);
		UTL_SetDbmAccessBlockNumber (DbmAccess_H, 1);
		UTL_SetDbmAccessStartAddr   (DbmAccess_H, start);
		UTL_SetDbmAccessStopAddr    (DbmAccess_H, start+(depth/8)-1);
		UTL_WriteDbm                (DbmAccess_H, dbm_data, depth*sizeof(int));
		UTL_DeleteHandle            (DbmAccess_H);
	}

	int CycleNum=48/dbm_bitmode;  
	if(CycleNum==48){CycleNum=32;}

	unsigned int accumurate = start*CycleNum;
	if(FirstRun)
		printf("***patName=%s, dbm_bitmode=%d, depth=%d, start=%d, accu=%d\n", patName, dbm_bitmode, depth, start, accumurate);
	ENTRY	e;
	ENTRY	*ep;
	e.key  = patName;
	e.data = (void*) accumurate;
	ep = hsearch(e, ENTER);

	return start+(depth/8);
}

int Setup_Dbm( char *patName, int dbm_bitmode, unsigned int *dbm_data, int depth, int sta)
{
	dbmap_set_condition( "DIGPINS", dbm_bitmode );
	int currentSize=dbmap_write( patName, dbm_data, depth, sta,dbm_bitmode);
	return currentSize;
}
void setDbmPointer(char *patname)
{
	ENTRY e;
	ENTRY *ep;
	e.key=patname;
	ep=hsearch(e,FIND);                                                                                                            
	if(ep!=NULL)
	{
		if(FK[1]){printf("%s=%d\n",ep->key,ep->data);}
		setRegValue(UT_REG_DBMA,(USlider)ep->data);
	}

	else
	{
		printf("can't find pattern %s!!!\n", patname);
		UTL_Stop();

	}


}


