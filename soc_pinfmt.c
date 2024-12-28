#include "common.h"
void SetPinOpen(){
	P("ALL_PINS", P_OPEN);
	//P("ALL_PINS_1", P_OPEN);
}

void SetPinfmt_Open(){
	P("SCK",	P_OPEN);
	P("SCE",	P_OPEN);
	P("SI",		P_OPEN);
	P("SO",		P_OPEN);
	P("VREF",	P_OPEN);
	P("EF0_TM0",	P_OPEN);
	P("EF1_TM0",	P_OPEN);
	P("EF2_TM0",	P_OPEN);
//	P("PTA5_TEST",	P_OPEN);
}

void SetDBMPinfmt(){
//	P("IO8",	P_IN(vi_io),	P_NRZB,		P_BCLK(1),	PDSA(1, P_C(6)),P_PPAT);//PORSTn
	P("IO16",	P_IN(vi_io),	P_RZO,		P_BCLK(2),P_CCLK(2),PDSA(1, P_C(7)),P_PPAT);//SCLK
	//P("IO16",	P_IN(vi_io),	P_IRZO,		P_BCLK(2),P_CCLK(2),PDSA(1, P_C(7)),P_PPAT);//SCLK
	P("IO4",	P_IN(vi_io),	P_NRZB,		P_BCLK(1),	PDSA(1, P_C(8)),P_PPAT);//Key
	P("IO24",	P_IN(vi_io),	P_NRZB,		P_BCLK(1),	PDSA(1, P_C(9)),P_PPAT);//SI
	P("IO17",	P_IN(vi_io),	P_NRZB,		P_BCLK(1),	PDSA(1, P_C(10)),P_PPAT);//SCE
	P("IO32",	P_OUT(vo_io),	P_VT(1),	P_STRB(1,1),	PDSA(1, P_C(11)),P_PPAT,P_PPAT_IOCTRL(1));//SO
}
#if 0
void SetPinfmt(){
	P("SCK",	P_IN(vi_io),	P_IRZO,			P_BCLK(2),	P_CCLK(2),	PDSA(1, P_C(1)));
	P("SCE",	P_IN(vi_io),	P_INRZB,		P_BCLK(1),			PDSA(1, P_C(0)));
	P("SI",		P_IN(vi_io),	P_NRZB,			P_BCLK(3),			PDSA(1, P_C(2)));
	P("SO",		P_OUT(vo_io),	P_VT(1),		P_STRB(1,1),			PDSA(1, P_C(3)));
}
#endif  
void SetPinfmt(){
	P("SCE",	P_IN(vi_io),	P_INRZB,		        P_BCLK(1),				PDSA(1, P_C(0)));
	//P("SCE",	P_IN(vi_io),	P_NRZB,		        P_BCLK(1),				PDSA(1, P_C(0)));
	P("SCK",	P_IN(vi_io),	P_IRZO,		            	P_BCLK(2),	 P_CCLK(2),	PDSA(1, P_C(1)));
	//P("SCK",	P_IN(vi_io),	P_RZO,		            	P_BCLK(2),	 P_CCLK(2),	PDSA(1, P_C(1)));
	P("SI",		P_IN(vi_io),	P_NRZB,		             	P_BCLK(3),				PDSA(1, P_C(2)));
	P("SO",		P_OUT(vo_io),	P_VT(1),			P_STRB(1,1),			            PDSA(1, P_C(3)));
//	P("TM0", 	P_IN(vi_io),    P_OPEN 	);
	P("Key",	P_IN(vi_io),	P_FIXL,  	P_NRZB,		P_BCLK(1),				PDSA(1, P_C(4)));	
//	P("PORSTn",	P_IN(vi_io),	P_INRZB,	             	P_BCLK(1),				PDSA(1, P_C(5)));
	//P("PORSTn",	P_IN(vi_io),	P_FIXH,     P_INRZB,	             	P_BCLK(1),				PDSA(1, P_C(5)));
	P("PORSTn",	P_IN(vi_io),	P_FIXH   );
	//P("PORSTn",	P_IN(vi_io),	P_NRZB,	             	P_BCLK(1),				PDSA(1, P_C(5)));
//	P("VREF_0P8", 	P_OPEN);
}  
