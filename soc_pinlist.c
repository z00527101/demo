#include "common.h"

void DefinePinList(){

	ulCreatePinListWithNumber("VDD33",   UT_PINTYPE_VS, vsno_vdd33);       //OK
	ulCreatePinListWithNumber("VDD11",   UT_PINTYPE_VS, vsno_vdd11);       //OK
	ulCreatePinListWithNumber("EF0_VPP", UT_PINTYPE_VS, vsno_ef0_vpp);
	ulCreatePinListWithNumber("EF1_VPP", UT_PINTYPE_VS, vsno_ef1_vpp);
	ulCreatePinListWithNumber("EF2_VPP", UT_PINTYPE_VS, vsno_ef2_vpp);
	ulCreatePinListWithNumber("EF3_VPP", UT_PINTYPE_VS, vsno_ef3_vpp);
	ulCreatePinListWithNumber("VREF",       UT_PINTYPE_PIN, 6);              //OK
//	ulCreatePinListWithNumber("VREF_10",   UT_PINTYPE_PIN, 6);              //OK
//	ulCreatePinListWithNumber("TM0",        UT_PINTYPE_PIN, 1);              //OK
//	ulCreatePinListWithNumber("EF2_TM0",    UT_PINTYPE_PIN, 1);
//	ulCreatePinListWithNumber("EF1_TM0",    UT_PINTYPE_PIN, 1);
	ulCreatePinListWithNumber("EF0_TM0",    UT_PINTYPE_PIN, 1);
	ulCreatePinListWithNumber("SO",         UT_PINTYPE_PIN, 5);              //OK
	ulCreatePinListWithNumber("SCE",        UT_PINTYPE_PIN, 3);              //OK
	ulCreatePinListWithNumber("SCK",        UT_PINTYPE_PIN, 2);              //OK
	ulCreatePinListWithNumber("SI",         UT_PINTYPE_PIN, 4);              //OK
	ulCreatePinListWithNumber("Key",        UT_PINTYPE_PIN, 10);             //OK
	ulCreatePinListWithNumber("XRS_N",      UT_PINTYPE_PIN, 9); //OK
	ulCreatePinListWithNumber("PORSTn",     UT_PINTYPE_PIN, 8); //OK
	ulCreatePinListWithNumber("VREGENZ",   UT_PINTYPE_PIN, 7);
	ulCreatePinListWithNumber("NC51",   UT_PINTYPE_PIN, 11);


#if 1
	ulCreatePinListWithList("IO8", UT_PINTYPE_PIN,"PORSTn",NULL);
	ulCreatePinListWithList("IO16", UT_PINTYPE_PIN,"SCK",NULL);
	ulCreatePinListWithList("IO4", UT_PINTYPE_PIN,"Key",NULL);
	ulCreatePinListWithList("IO24", UT_PINTYPE_PIN,"SI",NULL);
	ulCreatePinListWithList("IO17", UT_PINTYPE_PIN,"SCE",NULL);
	ulCreatePinListWithList("IO32", UT_PINTYPE_PIN,"SO",NULL);

#endif
	ulCreatePinListWithList("VS_PINS", UT_PINTYPE_VS,"VDD33","VDD11",NULL);
	ulCreatePinListWithList("HV_PINS", UT_PINTYPE_VS,   "EF0_VPP","EF1_VPP","EF2_VPP","EF3_VPP",NULL);
	ulCreatePinListWithList("VPP0_PINS", UT_PINTYPE_VS, "EF0_VPP","EF1_VPP","EF2_VPP","EF3_VPP",NULL);
	ulCreatePinListWithList("VPP1_PINS", UT_PINTYPE_VS, "EF0_VPP","EF1_VPP","EF2_VPP","EF3_VPP",NULL);
//	ulCreatePinListWithList("TM0", UT_PINTYPE_PIN,"EF0_TM0","EF1_TM0","EF2_TM0",NULL);
	ulCreatePinListWithList("TM0", UT_PINTYPE_PIN,"EF0_TM0",NULL);
	ulCreatePinListWithList("EF1_TM0", UT_PINTYPE_PIN,"EF0_TM0",NULL);
	ulCreatePinListWithList("EF2_TM0", UT_PINTYPE_PIN,"EF0_TM0",NULL);
	ulCreatePinListWithList("VREF_10", UT_PINTYPE_PIN,"VREF",NULL);
	ulCreatePinListWithList("ALL_PINS", UT_PINTYPE_PIN, "VREF","EF0_TM0","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	ulCreatePinListWithList("ALL_PINS_1", UT_PINTYPE_PIN,"VREF","EF0_TM0","SCK", "SCE", "SI", "SO",NULL);
	ulCreatePinListWithList("SIO_PINS", UT_PINTYPE_PIN, "SI", "SO", NULL);
	ulCreatePinListWithList("SI_PINS", UT_PINTYPE_PIN, "SI", NULL);
	ulCreatePinListWithList("SO_PINS", UT_PINTYPE_PIN, "SO", NULL);

	//ulCreatePinListWithList("DIGPINS", UT_PINTYPE_PIN,NULL)
	//ulCreatePinListWithList("DIGPINS", UT_PINTYPE_PIN,"PORSTn","IO16","IO4","IO24","IO17","IO32",NULL);
	//ulCreatePinListWithList("DIGPINS", UT_PINTYPE_PIN,"IO8","IO16","IO4","IO24","IO17","IO32",NULL);
	ulCreatePinListWithList("DIGPINS", UT_PINTYPE_PIN,"IO16","IO4","IO24","IO17","IO32",NULL);

	ulCreatePinListWithList("MASK_PINS", UT_PINTYPE_PIN, "SCE", "SCK","SI", NULL);//--used for fixh when the dut is not active, need to check the pinlist for each project

	//ulCreatePinListWithList("OS_PINS_H", UT_PINTYPE_PIN, "VREF","TM0","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	ulCreatePinListWithList("OS_PINS_H", UT_PINTYPE_PIN, "EF0_TM0","VREF","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	//ulCreatePinListWithList("OS_PINS_H", UT_PINTYPE_PIN,"VREF","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	//ulCreatePinListWithList("OS_PINS_L", UT_PINTYPE_PIN, "VREF","TM0","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
        ulCreatePinListWithList("OS_PINS_L", UT_PINTYPE_PIN, "EF0_TM0","VREF","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	//ulCreatePinListWithList("OS_PINS_L", UT_PINTYPE_PIN, "VREF","SCK", "SCE", "SI", "SO","Key","XRS_N","PORSTn","VREGENZ","NC51",NULL);
	ulCreatePinListWithList("INPUT_PINS", UT_PINTYPE_PIN,"Key","SCK", "SCE", "SI","PORSTn", NULL);
	//ulCreatePinListWithList("OUTPUT_PINS", UT_PINTYPE_PIN, "SO", "VREF",NULL);  
	ulCreatePinListWithList("OUTPUT_PINS", UT_PINTYPE_PIN, "SO", NULL);  



///---	ulCreatePinListWithList("FLASH_PINS", UT_PINTYPE_PIN, "SCE", "SCK", "SI", "SO", NULL);
///---	ulCreatePinListWithList("ALL_PINS_OPEN", UT_PINTYPE_PIN, "S_AGPIO11", "SI", "POWERON", "SCE","SCK","SO","EF1_TM0","EF0_TM0","S_AGPIO10",NULL);
}
 
