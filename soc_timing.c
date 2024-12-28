#include "common.h"

void SetDBMTiming()
{

	TgHandle h = UTL_GetTgHandle();
	int      ts, i;
	double
	RATE[]  = {	     50 NS,	 5 NS},
	BCLK1[] = {	      0 NS,	-1 NS},//SCE
	BCLK2[] = {	     25 NS,	-1 NS},//SCK
	CCLK2[] = {	     50 NS,	-1 NS},
	BCLK3[] = {	      0 NS,	-1 NS},//SIO
	STRB1[] = {	     40 NS,	 0 NS};//SIO
	for(ts=1, i=0; ts<=2;ts++,i++)
	{
		UTL_AddTgRate(h, ts, RATE[i]);
		if(BCLK1[i] < 0)    {UTL_AddTgBclkOff(h, ts, 1);}
		else                {UTL_AddTgBclk   (h, ts, 1, BCLK1[i]);}
		if(BCLK2[i] < 0)    {UTL_AddTgBclkOff(h, ts, 2);}
		else                {UTL_AddTgBclk   (h, ts, 2, BCLK2[i]);}
		if(CCLK2[i] < 0)    {UTL_AddTgCclkOff(h, ts, 2);}
		else                {UTL_AddTgCclk   (h, ts, 2, CCLK2[i]);}
		if(BCLK3[i] < 0)    {UTL_AddTgBclkOff(h, ts, 3);}
		else                {UTL_AddTgBclk   (h, ts, 3, BCLK3[i]);}
		if(STRB1[i] < 0)    {UTL_AddTgStrbOff(h, ts, 1);}
		else                {UTL_AddTgStrb   (h, ts, 1, STRB1[i]);}
	}

	UTL_SendTg(h);
	UTL_DeleteHandle(h);
}

