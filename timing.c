#include "common.h"

void SetTiming()
{
	TgHandle h = UTL_GetTgHandle();
	int      ts, i;

	if(OP_RATE==rate_20mhz){ 
		double
		RATE[]  = {	    5 NS, 	 50 NS,		 1 US, 	 50 NS},
		BCLK1[] = {	   -1 ,	  	  0 NS,		 0 NS,	 0 NS},//-- SCE
		BCLK2[] = {	   -1 ,  	  0 NS,		 0 NS,   0 NS},//-- SCLK
		//BCLK2[] = {	   -1 ,  	  25 NS,		 0 NS,   0 NS},//-- SCLK
		CCLK2[] = {	   -1 ,    	 25 NS,		25 NS,   25 NS},
		//CCLK2[] = {	   -1 ,    	 50 NS,		25 NS,   25 NS},
		BCLK3[] = {	   -1 ,	  	 0 NS,		 0 NS,	 0 NS},//-- SI
		//STRB1[] = {	  2.5 NS,	 48 NS,		48 NS,	 48 NS},//-- SO
		STRB1[] = {	  2.5 NS,	 40 NS,		40 NS,	 40 NS},//-- SO
		STRB2[] = {	  2.5 NS,	 48 NS,		48 NS,	 48 NS};//-- SO
		for(ts=1, i=0; ts<=4;ts++,i++)
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
			if(STRB2[i] < 0)    {UTL_AddTgStrbOff(h, ts, 2);}
			else                {UTL_AddTgStrb   (h, ts, 2, STRB2[i]);}
		}
	}else if(OP_RATE==rate_15mhz){
		double
		RATE[]  = {		5 NS,  66.7 NS,		 1 US,  66.7 NS},
		BCLK1[] = {	   -1 ,	  0 NS,		 0 NS,	0 NS},//-- SCE
		BCLK2[] = {	   -1 ,   0 NS,		 0 NS,  0 NS},//-- SCLK
		CCLK2[] = {	   -1 NS, 33.35 NS,  33.35 NS, 	33.35 NS},
		BCLK3[] = {	   -1 ,	  0 NS,		 0 NS,	0 NS},//-- SI
		STRB1[] = {	  2.5 NS,	 60 NS,		60 NS,	60 NS},//-- SO
		STRB2[] = {	  2.5 NS,	 25 NS,		60 NS,	60 NS};//-- SO
		for(ts=1, i=0; ts<=4;ts++,i++)
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
			if(STRB2[i] < 0)    {UTL_AddTgStrbOff(h, ts, 2);}
			else                {UTL_AddTgStrb   (h, ts, 2, STRB2[i]);}
		}
	}else if(OP_RATE==rate_10mhz){
		double
		RATE[]  = {		5 NS,   100 NS,		 1 US,   100 NS},
		BCLK1[] = {	   -1 NS,	  0 NS,		 0 NS,	 0 NS},//-- SCE
		BCLK2[] = {	   -1 NS,     0 NS,		 0 NS,   0 NS},//-- SCLK
		CCLK2[] = {	   -1 NS,    50 NS,     50 NS,   50 NS},
		BCLK3[] = {	   -1 NS,	  0 NS,		 0 NS,	 0 NS},//-- SI
		STRB1[] = {	  2.5 NS,	 90 NS,		90 NS,	 90 NS},//-- SO
		STRB2[] = {	  2.5 NS,	 40 NS,		90 NS,	 90 NS};//-- SO
		for(ts=1, i=0; ts<=4;ts++,i++)
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
			if(STRB2[i] < 0)    {UTL_AddTgStrbOff(h, ts, 2);}
			else                {UTL_AddTgStrb   (h, ts, 2, STRB2[i]);}
		}
	}else if(OP_RATE==rate_5mhz){
		double
		RATE[]  = {	    5 NS,   200 NS,		 1 US,   200 NS},
		BCLK1[] = {	   -1 ,	      0 NS,		 0 NS,	 0 NS},//-- SCE
		BCLK2[] = {	   -1 ,       0 NS,		 0 NS,   0 NS},//-- SCLK
		CCLK2[] = {	   -1 ,     100 NS,    100 NS,   100 NS},
		BCLK3[] = {	   -1 ,	      0 NS,		 0 NS,	 0 NS},//-- SI
		STRB1[] = {	  2.5 NS,	180 NS,		120 NS,	120 NS},//-- SO
		STRB2[] = {	  2.5 NS,	40 NS,		120 NS,	120 NS};//-- SO
		for(ts=1, i=0; ts<=4;ts++,i++)
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
			if(STRB2[i] < 0)    {UTL_AddTgStrbOff(h, ts, 2);}
			else                {UTL_AddTgStrb   (h, ts, 2, STRB2[i]);}
		}
	}
	else{printf("the rate is fault, can't set timing!!!\n"); UTL_Stop();}

	UTL_SendTg(h);
	UTL_DeleteHandle(h);
}
