#include "common.h"


int main(int argc, char **argv)
{
	UTL_InitTest(argc, argv);
	UTL_StartTimer(UT_TIMER_R10MS);
	InitialVariable();
#if 0
	if(UTL_GetDutCount(UT_SDUT) != 0)
	{	
		TestStartLog();
	}
#endif
#if 1
	if(UTL_GetDutCount(UT_SDUT) == 0)
	{
		printf("ut_sdut:%d\n",UTL_GetDutCount(UT_SDUT));	
	}else{

		TestStartLog();
}
#endif

    hcreate(500);
	DefinePinList();
	SetPowerSeq();
	DbmLoad();

#ifdef CP1
	Flow_CP1();  
#elif (defined CP2)
	Flow_CP2();  
#elif (defined CP3)
	Flow_CP3();  
#elif (defined OS)
	Flow_OS();
#elif (defined EFA)
	Flow_EFA();  
#elif (defined DEBUG)
	Flow_DEBUG();  
#endif

	hdestroy();
	
	if(UTL_GetDutCount(UT_SDUT) != 0)
{

	TestEndLog();
}
	return 0;
}
