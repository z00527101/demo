#include "common.h"

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1:variable define


//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2:DC
void tb_opens_shorts(){
	UTL_ResetWet();
	SetLevel0V();
	SetAllPin2OneLevel((char *)"ALL_PINS", 0 V);

	ulSetSettlingTime(5 MS,8 MS,5 MS);
	printf((char *)"[DUT]\tTest Name\tPIN\tCHN\t\t\t\tHlimit\tMeasure\tLolimit\t\tSITE\tSDUT\tFinal Result\n");	


//-- for IO pins
	setDcISVM(-100 UA, R200uA, Rm20V, 0.6 V, -1.2 V, -0.15 V, -0.85 V, 5);
	DcExec("OS_PINS_L");
	Read_Meas_Result("OS_PINS_L", "CONTACT_LOW", -0.15 V, -0.85 V, 1 V, "V");

	setDcISVM(100 UA, R200uA, R20V, 1.2 V, -0.6 V, 0.85 V, 0.15 V, 5);
	DcExec("OS_PINS_H");
	Read_Meas_Result("OS_PINS_H", "CONTACT_HIGH", 0.85 V, 0.15 V, 1 V, "V");

#if 1
//-- for HV pins
#if 1
        setHvUnMask("HV_PINS");
        setHvVSIM("HV_PINS", -0.15 V,R32V ,M8uA ,1 MA, -1 MA, 8 UA, -8 UA, 10);
        START_GROUP_TEST_BY_VPP(vsno_ef0_vpp)
                DcExec("EF0_VPP");
                Read_Meas_Result("EF0_VPP", "EF0_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
        END_GROUP_TEST_BY_VPP(vsno_ef0_vpp)
        START_GROUP_TEST_BY_VPP(vsno_ef1_vpp)
                DcExec("EF1_VPP");
                Read_Meas_Result("EF1_VPP", "EF1_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
        END_GROUP_TEST_BY_VPP(vsno_ef1_vpp)
        START_GROUP_TEST_BY_VPP(vsno_ef2_vpp)
                DcExec("EF2_VPP");
                Read_Meas_Result("EF2_VPP", "EF2_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
        END_GROUP_TEST_BY_VPP(vsno_ef2_vpp)
        START_GROUP_TEST_BY_VPP(vsno_ef3_vpp)
                DcExec("EF3_VPP");
                Read_Meas_Result("EF3_VPP", "EF3_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
        END_GROUP_TEST_BY_VPP(vsno_ef3_vpp)
        setHvVSIM("HV_PINS", 0 V,R32V ,M8uA ,1 MA, -1 MA, 8 UA, -8 UA, 10);
        setHvMask("HV_PINS");
#endif
#if 0
//-- for DPS pins
        setDcVSIM(0.1 V, R20V, R200uA, 200 UA, -200 UA, 40 UA, -40 UA, 5);
        //setDcVSIM(0.1 V, R20V, R2mA, 2 MA, -2 MA, 1 MA, -1 MA, 5);
	START_GROUP_TEST_BY_VS(vsno_vdd11)
		DcExec("VDD11");             
		//Read_Meas_Result("VDD11", "VDD11_Current", 2 MA, -2 MA, 1 MA, "MA");
		Read_Meas_Result("VDD11", "VDD11_Current", 200 UA, -200 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VS
	setDcVSIM(3 V, R20V, R200uA, 200 UA, -200 UA, 40 UA, -40 UA, 5);
        //setDcVSIM(0.1 V, R20V, R2mA, 2 MA, -2 MA, 1 MA, -1 MA, 5);
	START_GROUP_TEST_BY_VS(vsno_vdd33)
		DcExec("VDD33");             
		//Read_Meas_Result("VDD11", "VDD11_Current", 2 MA, -2 MA, 1 MA, "MA");
		Read_Meas_Result("VDD33", "VDD33_Current", 200 UA, -200 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VS

#endif
#if 0
	SetLevelNorm();
//	SetLevelHigh();
#if 0
	START_GROUP_TEST_BY_VS(vsno_vdd09)
		DcExec("VDD09");  
		Read_Meas_Result("VDD09", "VDD09_Current", 200 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS
#endif
#if 1
	START_GROUP_TEST_BY_VS(vsno_vdd11)
		DcExec("VDD11");             
		Read_Meas_Result("VDD11", "VDD11_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS
#endif
	START_GROUP_TEST_BY_VS(vsno_vdd33)
		DcExec("VDD33"); 
		Read_Meas_Result("VDD33", "VDD33_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS

#endif
#endif
}
void tb_power_leakage(){
	SetPinfmt_Open();
	SetLevel0V();
	PowerOn();
	setVs(vsno_vdd11, VDD11_MAX, R16V, M400mA, 200 MA, -200 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd11, VDD11_MAX, R16V, M200mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33, VDD33_MAX, R16V, M400mA, 200 MA, -200 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd33, VDD33_MAX, R16V, M200mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);


//-- for DPS pins
	START_GROUP_TEST_BY_VS(vsno_vdd33)
		//setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 200 MA, -100 MA, 400 MA, -200 MA, 5);
		setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
		//setVs(vsno_vdd33, VDD33_NOM, R16V, M200mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		DcExec("VDD33");                   
		Read_Meas_Result("VDD33", "VDD33_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS
	START_GROUP_TEST_BY_VS(vsno_vdd11)
		//setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 400 MA, -100 MA, 200 MA, -200 MA, 5);
		//setVs(vsno_vdd11, VDD11_NOM, R16V, M200mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		DcExec("VDD11");                                                                                                      
		Read_Meas_Result("VDD11", "VDD11_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS

//-- for HV pins
#if 0
	setHvUnMask(vsno_ef0_vpp);
	setHvVSIM(vsno_ef0_vpp,5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	START_GROUP_TEST_BY_VPP(vsno_ef0_vpp)
		DcExec("EF0_VPP");              
		Read_Meas_Result("EF0_VPP", "EF0_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VPP(vsno_ef0_vpp)
	setHvVSIM(vsno_ef0_vpp,0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	setHvMask(vsno_ef0_vpp);
	setHvUnMask(vsno_ef1_vpp);
	setHvVSIM(vsno_ef1_vpp,5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	START_GROUP_TEST_BY_VPP(vsno_ef1_vpp)
		DcExec("EF1_VPP");              
		Read_Meas_Result("EF1_VPP", "EF1_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VPP(vsno_ef1_vpp)
	setHvVSIM(vsno_ef1_vpp,0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	setHvMask(vsno_ef1_vpp);

	setHvUnMask(vsno_ef2_vpp);
	setHvVSIM(vsno_ef2_vpp,5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	START_GROUP_TEST_BY_VPP(vsno_ef2_vpp)
		DcExec("EF2_VPP");              
		Read_Meas_Result("EF2_VPP", "EF2_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VPP(vsno_ef2_vpp)
	setHvVSIM(vsno_ef2_vpp,0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	setHvMask(vsno_ef2_vpp);

	setHvUnMask(vsno_ef3_vpp);
	setHvVSIM(vsno_ef3_vpp,5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	START_GROUP_TEST_BY_VPP(vsno_ef3_vpp)
		DcExec("EF3_VPP");              
		Read_Meas_Result("EF3_VPP", "EF3_VPP_Current", 8 UA, -8 UA, 1 UA, "UA");
	END_GROUP_TEST_BY_VPP(vsno_ef3_vpp)
	setHvVSIM(vsno_ef3_vpp,0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
	setHvMask(vsno_ef3_vpp);

#endif
}
void tb_input_leakage(){

	SetAllPin2OneLevel((char *)"INPUT_PINS", 0.0 V);
	//setDcVSIM(VDD33_NOM, R20V, R200uA, 400 UA, -400 UA, 200 UA, -200 UA, 5);
	setDcVSIM(VDD33_NOM, R20V, R200uA, 200 UA, -200 UA, 200 UA, -200 UA, 5);
	DcExec("INPUT_PINS");
	Read_Meas_Result("INPUT_PINS", "INPUT_LEAKAGE_HIGH", 200 UA, -200 UA, 1 UA, "UA");

	SetAllPin2OneLevel((char *)"INPUT_PINS", VDD33_NOM);
	//setDcVSIM(0.0 V, R20V, R200uA, 400 UA, -400 UA, 200 UA, -200 UA, 5);
	setDcVSIM(0.0 V, R20V, R200uA, 200 UA, -200 UA, 200 UA, -200 UA, 5);
	DcExec("INPUT_PINS");
	Read_Meas_Result("INPUT_PINS", "INPUT_LEAKAGE_LOW", 200 UA, -200 UA, 1 UA, "UA");

	UTL_OffPowerSeq();
}

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 3:customer  power on

void tb_soc_power_set_0(){
	SetLevel0V();
}
void tb_soc_power_reset(){
	SetLevel0V();
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.2 V, 0.9 V, 5);
	VsOpenByList("VS_PINS");
	VsOpenByList("HV_PINS");
	SetPinOpen();
	PowerOff();

	UTL_WaitTime(5 MS);
	SetLevel0V();
	PowerOn();
	//SetPinOpen();
        SetLevelNorm();
	P("PORSTn",    P_IN(vi_io),   P_FIXH );
	P("XRS_N",     P_IN(vi_io),   P_FIXL ); //step1 XRS_N down, power on
	//Pause2("wait1");
        P("VREGENZ",    P_IN(vi_io),   P_FIXH );//disable internal LDO&DCDC function
        P("SCE",    P_IN(vi_io),   P_FIXH );//disable internal LDO&DCDC function
        //P("VREGENZ",    P_IN(vi_io),   P_FIXL );//disable internal LDO&DCDC function
        P("NC51",    P_IN(vi_io),   P_FIXH );//disable PORBOR function

	UTL_WaitTime(5 MS);
	UTL_WaitTime(5 MS);
	//P("PORSTn",    P_IN(vi_io),   P_FIXL );
	P("PORSTn",    P_IN(vi_io),   P_FIXH );

	UTL_WaitTime(10 MS);
        //step2 wait 200us, XRS_N high	
        P("XRS_N",       P_IN(vi_io),   P_FIXH );
	//Pause2("wait2");
        //step3 wait 5us, XRS_N down,GPIO4 input low(GPIO4<---->Key),wait 500ns,input sclk through GPIO16 pin	
        UTL_WaitTime(10 MS);
        P("XRS_N",       P_IN(vi_io),   P_FIXL );
	P("Key",    P_IN(vi_io),   P_FIXL );
	UTL_WaitTime(50 MS);
	setDcVSIM(VDD33_NOM,R20V,R20mA,20 MA, -20 MA, 20 MA, -20 MA, 5);
	DcExec("OS_PINS_H");
    Read_Meas_Result("OS_PINS_H","IO_LEAKAGE_HIGH", 20 MA, -20 MA, 1 MA, "MA");
    //Read_Meas_Result("OS_PINS_H","IO_LEAKAGE_HIGH", 20 UA, -20 UA, 1 UA, "UA");

	setDcVSIM(0.0 V,R20V,R20mA,20 MA, -20 MA, 20 MA, -20 MA, 5);
	//setDcVSIM(0.0 V,R20V,R20uA,20 UA, -20 UA, 20 UA, -20 UA, 5);
	DcExec("OS_PINS_L");
    Read_Meas_Result("OS_PINS_L","IO_LEAKAGE_LOW", 20 MA, -20 MA, 1 MA, "MA");
    //Read_Meas_Result("OS_PINS_L","IO_LEAKAGE_LOW", 20 UA, -20 UA, 1 UA, "UA");
#if 0
	START_GROUP_TEST_BY_VS(vsno_vdd33)
		//setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 200 MA, -100 MA, 400 MA, -200 MA, 5);
		setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
		//setVs(vsno_vdd33, VDD33_NOM, R16V, M200mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		DcExec("VDD33");                   
		Read_Meas_Result("VDD33", "VDD33_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS
	START_GROUP_TEST_BY_VS(vsno_vdd11)
		//setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 400 MA, -100 MA, 200 MA, -200 MA, 5);
		//setVs(vsno_vdd11, VDD11_NOM, R16V, M200mA, 200 MA, -100 MA, 200 MA, -200 MA, 5);
		DcExec("VDD11");                                                                                                      
		Read_Meas_Result("VDD11", "VDD11_Current", 400 MA, -200 MA, 1 MA, "MA");
	END_GROUP_TEST_BY_VS
#endif
        P("XRS_N",       P_IN(vi_io),   P_FIXL );
	//Pause2("wait3");
//	Pause2("wait4");
	UTL_WaitTime(50 MS);

	LogicPatExec("STB_READ_REG.mpa");
	//FlashPatExec("patFlash.mpa", "SOC_POWER_RESET");
	
	P("PORSTn",    P_IN(vi_io),   P_FIXH );
	// (1)Afer PowerOn, Vref has a 1V voltage
	// (2)Modify 4bits of Vref in STB Reg
#if 0
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	DcExec("VREF");
	//Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.96 V, 1 V, "V");
	Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.3 V, 1 V, "V");
#endif
}
#if 0
void tb_soc_power_reset(){
	SetLevel0V();
	//setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.2 V, 0.9 V, 5);
	//VsOpenByList("VS_PINS");
	//VsOpenByList("HV_PINS");
	SetPinOpen();
	PowerOff();
	UTL_WaitTime(1000 MS);
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	DcExec("VREF");
	//Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.96 V, 1 V, "V");
	Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.3 V, 1 V, "V");

        SetLevelNorm();
	PowerOn();
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	DcExec("VREF");
	//Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.96 V, 1 V, "V");
	Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.3 V, 1 V, "V");
	//SetPinOpen();
	P("XRS_N",     P_IN(vi_io),   P_FIXL ); //step1 XRS_N down, power on
//Pause2("wait");    
	P("PORSTn",    P_IN(vi_io),   P_FIXH );
        P("VREGENZ",    P_IN(vi_io),   P_FIXH );//disable internal LDO&DCDC function
        P("NC51",    P_IN(vi_io),   P_FIXH );//disable PORBOR function

	UTL_WaitTime(10 MS);
	;UTL_WaitTime(1 MS);
        //step2 wait 200us, XRS_N high	
        P("XRS_N",       P_IN(vi_io),   P_FIXH );
//Pause2("wait2");    

        //step3 wait 5us, XRS_N down,GPIO4 input low(GPIO4<---->Key),wait 500ns,input sclk through GPIO16 pin	
        UTL_WaitTime(10 MS);
        P("XRS_N",       P_IN(vi_io),   P_FIXL );
	P("Key",    P_IN(vi_io),   P_FIXL );
//Pause2("wait3");    
	UTL_WaitTime(5 MS);
	//Read_Meas_Result("VDD33", "VS_Current", 400 MA, -200 MA, 1 MA, "MA");
	//Read_Meas_Result("VDD11", "VS_Current", 200 MA, -200 MA, 1 MA, "MA");
	
        UTL_WaitTime(10 MS);
	FlashPatExec("patFlash.mpa", "SOC_POWER_RESET");
//Pause2("wait4");    
//	UTL_WaitTime(1000 MS);
	
	// (1)Afer PowerOn, Vref has a 1V voltage
	// (2)Modify 4bits of Vref in STB Reg
#if 1
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	DcExec("VREF");
	//Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.96 V, 1 V, "V");
	Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.3 V, 1 V, "V");
#endif
}
#endif
void Caltrimvalue2(char *trimlabel,double trim_result[][DDUTCNT],int trim[], double target,int trimcount)
{
	int trimcode = 0, i = 0;

	double meas_best[DDUTCNT]={0};

	START_DUT_LOOP(UT_CDUT)
	meas_best[dut-1]=99;

		printf("\nDUT%2d:  %s  ", dut,trimlabel);
		for(i = 0; i<trimcount; i++){
	        if(trim_result[i][dut-1]>-30)printf(" %.3lf", trim_result[i][dut-1]);else printf(" _  ");
			if(fabs(trim_result[i][dut-1]-target)<fabs(meas_best[dut-1]-target))
			{
				meas_best[dut-1] = trim_result[i][dut-1];
				trim[dut-1] = i;
			}
		}

		printf("BTV:%2.3lf , Posi:%d\n",meas_best[dut-1],trim[dut-1]);
	END_DUT_LOOP

}

void tb_SOC_VREF_Monitor(){

	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	DcExec("VREF");
	Read_Meas_Result("VREF", "VREF_1V", 1.04 V, 0.96 V, 1 V, "V");

}
void tb_Write_Soc_Reg_Vref_trim(){
 tb_Write_Soc_Reg_Vref_trim_01();//iterate all possible trim code
 tb_Write_Soc_Reg_Vref_trim_02();//binary search for target trim code
}

#if 1
void tb_Write_Soc_Reg_Vref_trim_02(){
    UTL_WaitTime(5 MS);
	double target=VREF_Trim_Target;
	printf("vref 1v trim target is %f\n",target);
	double hlimit=VREF_Trim_Target+VREF_Trim_error;
	double lolimit=VREF_Trim_Target-VREF_Trim_error;                                                                                                                                                                                   
	int data=1;
	double voltage[DDUTCNT]={0};
	double datain[VREF_trimcount][DDUTCNT]={0};
	long long int active_dut=0;
	int start[DDUTCNT]={0},stop[DDUTCNT]={0},mid[DDUTCNT]={0};
	double delta[DDUTCNT]={1};
	int searchflag[DDUTCNT]={0};

	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.2 V, 0.9 V, 5);
        GetActiveDut(&active_dut);                                                                              
        START_DUT_LOOP(UT_CDUT)//init data 
            start[dut-1]=0;
            stop[dut-1]=VREF_trimcount-1;    
          //  mid[dut-1]=(start[dut-1]+stop[dut-1])/2;   
            mid[dut-1]=8;   
        END_DUT_LOOP
	ulConfigUbm("SI_PINS", C28, C27);
	ExclusionHandle hex = UTL_GetExclusionHandle();
	UTL_SetExclusionIgnoreWet(hex, UT_OFF);
	UTL_SetExclusionMask(hex, UT_OFF);
	if(1){
			while(UTL_GetDutCount(UT_CDUT)) 
			{
					START_DUT_LOOP(UT_CDUT)
						data=(VREF10_trim_value[mid[dut-1]]&0xf)|0xffffbe70;//VREF default,without trim work
						ulWriteUbm("SIO_PINS",0,31, 0x0, 0x0,data, dut);                                 
					END_DUT_LOOP
					FlashPatExec("patFlash.mpa", "WRITE_SOC_REG_UBM");
					UTL_WaitTime(10 MS);
					DcExec("VREF");                                                                              
					START_DUT_LOOP(UT_CDUT)
						ReadMeasData("VREF", 1 V, datain[mid[dut-1]], dut);
					END_DUT_LOOP
					START_DUT_LOOP(UT_CDUT) 
							if(start[dut-1]>=stop[dut-1]||fabs(datain[mid[dut-1]][dut-1]-target)<0.001)      
							{
									UTL_AddExclusionDut(hex,dut);                   
							}
							else
							{
									delta[dut-1]=datain[mid[dut-1]][dut-1]-target;
									//set search mode: step mode or binary sort mode
									if( fabs(delta[dut-1])<=0.03) searchflag[dut-1]=1; 
									else searchflag[dut-1]=0;                   
									if( searchflag[dut-1]==1)                   
									{   // step mode for near point
											if(delta[dut-1]<0)              
											{
													stop[dut-1]=mid[dut-1]-1;       
													mid[dut-1]=mid[dut-1]-1;
											}
											else 
											{
													start[dut-1]=mid[dut-1]+1;  
													mid[dut-1]=mid[dut-1]+1;
											}
									}
									else 
									{
											if (delta[dut-1] <0)               
											{ 
													stop[dut-1]=mid[dut-1]-1;    
													mid[dut-1]=(int)((start[dut-1]+stop[dut-1])/2);
											}
											else    
											{
													start[dut-1]=mid[dut-1]+1;
													mid[dut-1]=(int)((start[dut-1]+stop[dut-1])/2);
											}
									}

							}
					END_DUT_LOOP          
					UTL_SetExclusionSetOrReset (hex,UT_ON);           
					UTL_SendExclusion(hex);                                             
			}
//	}
	                UTL_DeleteHandle(hex);
           		ReturnActiveDut(active_dut);                        
			Caltrimvalue2("VREF_TRIM",datain,VREF_trim_tmp,target,VREF_trimcount);		
			//Caltrimvalue2("VREF_TRIM",datain,VcodeTmp,target,VREF_trimcount);		
			START_DUT_LOOP(UT_CDUT)
					//printf("DUT:%d TRIM_VALUE:%d\n",dut,VREF10_trim_value[VREF_trim[dut-1]]);
					//printf("DUT:%d TRIM_VALUE:%d\n",dut,VREF10_trim_value[VcodeTmp[dut-1]]);
			END_DUT_LOOP
	}
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.6 V, hlimit, lolimit, 5);
	START_DUT_LOOP(UT_CDUT)
		data=(VREF10_trim_value[VREF_trim_tmp[dut-1]]&0xf)|0xffffbe70;//VREF default,without trim work
					printf("DUT:%d TRIM_CODE_VALUE_INDEX:%d\n",dut,VREF_trim_tmp[dut-1]);
		VcodeTmp[dut-1]=VREF10_trim_value[VREF_trim_tmp[dut-1]];
					printf("DUT:%d TRIM_VCODE_VALUE:%d\n",dut,VcodeTmp[dut-1]);
                ulWriteUbm("SIO_PINS",0,31, 0x0, 0x0,data, dut);                                 
	END_DUT_LOOP
		 FlashPatExec("patFlash.mpa", "WRITE_SOC_REG_UBM");
	UTL_WaitTime(5 MS);

	DcExec("VREF");
    Read_Meas_Result("VREF","VREF",hlimit V,lolimit V,1 V,"V");



}
#endif
//implement the AR to loop all the available trim code to get the corresponding vref value for late save to NVR area
void tb_Write_Soc_Reg_Vref_trim_01(){
	ulConfigUbm("SI_PINS", C28, C27);
	setDcISVM(0 UA, R20uA, R20V, 1.5 V, -0.5 V, 1.04 V, 0.96 V, 10);
	int dut_flag[DDUTCNT]={0};
	int i;
	printf("Get the VREF_10 voltage:\n");
//	START_FLASH_LOOP
		for(i = 0; i < VREF10_trimcount; i++){
		//for(i = 15; i >= 0; i--){
			START_DUT_LOOP(UT_CDUT)
			//ulWriteUbm("SI", 0 * PROG_DATA_BITSIZE, 1 * PROG_DATA_BITSIZE - 1, 0x0, 0x0, (VREF10_trim_value[i]&0xf)|0xffffbe70,dut);
			//ulWriteUbm("SI", 0 , 31, 0x0, 0x0, (VREF10_trim_value[i]&0xf)|0xffffbe70,dut);
			ulWriteUbm("SI", 0 , 31, 0x0, 0x0, (VREF10_trim_value[i]&0xf)|0xffff3e70,dut);
          		END_DUT_LOOP
			FlashPatExec("patFlash.mpa", "WRITE_SOC_REG_UBM");
			
			DcExec("VREF_10");
                        	START_DUT_LOOP(UT_CDUT)
					//ReadMeasData("VREF_10", 1 V,  vref10_trim_result[15-i],dut);
					ReadMeasData("VREF_10", 1 V,  vref10_trim_result[i],dut);
					//vref10_value_code[VREF10_trim_value[i]][g_flash_no-1][dut-1] = int(vref10_trim_result[i][g_flash_no-1][dut-1] * 1000);
					//vref10_value_code[VREF10_trim_value[15-i]][dut-1] = int(vref10_trim_result[15-i][dut-1] * 1000);
					vref10_value_code[VREF10_trim_value[i]][dut-1] = int(vref10_trim_result[i][dut-1] * 1000);
					printf("Dut %d, Vcode=%d , VREF10 ==%f,VREF10_value[%d]==%d\n",dut, VREF10_trim_value[i], vref10_trim_result[i][dut-1],i,vref10_value_code[VREF10_trim_value[i]][dut-1]);
					//printf("Dut %d, Vcode=%d , VREF10 ==%f,VREF10_value[%d]==%d\n",dut, VREF10_trim_value[i], vref10_trim_result[i][dut-1],i,vref10_value_code[i][dut-1]);
					//printf("Binary = %x \n",int(vref10_trim_result[i][g_flash_no-1][dut-1] * 1000));
                             	END_DUT_LOOP

		}
//	END_FLASH_LOOP
	printf("\n");

}


//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 4:for customer
void Int2ByteArray(int IntValue, unsigned char *bytes)
{
	int n;
	n = IntValue;
	bytes[0] = (n >> 24) &0xff;
	bytes[1] = (n >> 16) &0xff;
	bytes[2] = (n >> 8) &0xff;
	bytes[3] = n &0xff;
//#if 0
	for(int i=0;i<4;i++)
	{
		printf("0x%x ",bytes[i]);
	}
	printf("\n");
}

int getParity(int n)
{
    int parity = 0;
    while (n)
   {
	parity = !parity;
        n = n & (n-1);
   } 
   return parity;
}
//校验函数
//查表法计算crc
unsigned short do_crc_table(unsigned char *ptr,int len)
{
    unsigned short crc = 0x0000;

    while(len--)
    {
        crc = (crc << 8) ^ crc_table[(crc >> 8 ^ *ptr++) & 0xff];
    }

    return(crc);
}

//直接计算法计算crc
unsigned short do_crc(unsigned char *ptr, int len)
{
    unsigned int i;
    unsigned short crc = 0x0000;

    while(len--)
    {
        crc ^= (unsigned short)(*ptr++) << 8;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }

    return crc;
}

unsigned short CalcEcc(unsigned int address, unsigned int data_H, unsigned int data_L)			
{			
	const unsigned int addrSyndrome[8] = {0x554ea, 0x0bad1, 0x2a9b5, 0x6a78d,0x19f83, 0x07f80, 0x7ff80, 0x0007f};			
        const unsigned int data_HSyndrome[8] = {0xb4d1b4d1, 0x15571557,	0xa699a699, 0x38e338e3,0xc0fcc0fc, 0xff00ff00,0xff0000ff, 0x00ffff00};			
	const unsigned int data_LSyndrome[8] = {0x4b2e4b2e, 0x15571557,	0xa699a699, 0x38e338e3,0xc0fcc0fc, 0xff00ff00,0xff0000ff, 0xff0000ff};			
	const unsigned short parity = 0xfc;			
	unsigned int xorData_H;			
	unsigned int xorData_L;			
	unsigned int xorData;			
	unsigned int xorAddr;			
	unsigned short bit, eccBit, eccVal;			
			
			//Extract bits "20:2" of the address			
			//			
			address = (address >> 2) & 0x7ffff;			
			//			
			//Compute the ECC one bit at a time.			
			//			
			eccVal = 0;			
			for (bit = 0; bit < 8; bit++)			
			{			
			//			
			//Apply the encoding masks to the address and data			
			//			
			xorAddr = address & addrSyndrome[bit];			
			xorData_H = data_H & data_HSyndrome[bit];			
			xorData_L = data_L & data_LSyndrome[bit];			
			//			
			//Fold the masked address into a single bit for parity calculation.			
			//The result will be in the LSB.			
			//			
			xorAddr = xorAddr ^ (xorAddr >> 16);			
			xorAddr = xorAddr ^ (xorAddr >> 8);			
			xorAddr = xorAddr ^ (xorAddr >> 4);			
			xorAddr = xorAddr ^ (xorAddr >> 2);			
			xorAddr = xorAddr ^ (xorAddr >> 1);			
			//			
			//Fold the masked data into a single bit for parity calculation.			
			//The result will be in the LSB.			
			xorData = xorData_H ^ xorData_L ;			
			xorData = xorData ^ (xorData >> 16);			
			xorData = xorData ^ (xorData >> 8);			
			xorData = xorData ^ (xorData >> 4);			
			xorData = xorData ^ (xorData >> 2);			
			xorData = xorData ^ (xorData >> 1);			
			//			
			//Merge the address and data, extract the ECC bit, and add it in			
			//			
			//eccBit = ((unsigned int)xorData ^ (unsigned int)xorAddr) & 0x0001;			
			eccBit = ((unsigned short)xorData ^ (unsigned short)xorAddr) & 0x0001;			
			eccVal |= eccBit << bit;			
			}
										
			//Handle the bit parity. For odd parity, XOR the bit with 1									
			//									
			eccVal ^= parity;									
			return eccVal;
}			



/************************************************************************************************/
/* Function     : CalculateECC()	     	   	 					*/
/* Description  : Calculate ECC code according to bit[63:0]					*/
/************************************************************************************************/

int CalculateECC(unsigned int hb32, unsigned int lb32)
{
//-- special for 067T, please check and rewrite

    int parity_wr_bit0;
    int parity_wr_bit1;
    int parity_wr_bit2;
    int parity_wr_bit3;
    int parity_wr_bit4;
    int parity_wr_bit5;
    int parity_wr_bit6;
    int parity_wr_bit7;
    int parity_wr;
    int i,j;

    parity_wr_bit0 = ((lb32 >> 0) & 0x1) ^ ((lb32 >> 1) & 0x1) ^ ((lb32 >> 3) & 0x1) ^ ((lb32 >> 4) & 0x1) ^ ((lb32 >> 6) & 0x1) ^ ((lb32 >> 8) & 0x1) ^ ((lb32 >> 10) & 0x1) ^ ((lb32 >> 11) & 0x1) ^ ((lb32 >> 13) & 0x1) ^ ((lb32 >> 15) & 0x1) ^ ((lb32 >> 17) & 0x1) ^ ((lb32 >> 19) & 0x1) ^ ((lb32 >> 21) & 0x1) ^ ((lb32 >> 23) & 0x1) ^ ((lb32 >> 25) & 0x1) ^ ((lb32 >> 26) & 0x1) ^ ((lb32 >> 28) & 0x1) ^ ((lb32 >> 30) & 0x1) ^ ((hb32 >> (32-32)) & 0x1) ^ ((hb32 >> (34-32)) & 0x1) ^ ((hb32 >> (36-32)) & 0x1) ^ ((hb32 >> (38-32)) & 0x1) ^ ((hb32 >> (40-32)) & 0x1) ^ ((hb32 >> (42-32)) & 0x1) ^ ((hb32 >> (44-32)) & 0x1) ^ ((hb32 >> (46-32)) & 0x1) ^ ((hb32 >> (48-32)) & 0x1) ^ ((hb32 >> (50-32)) & 0x1) ^ ((hb32 >> (52-32)) & 0x1) ^ ((hb32 >> (54-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1) ^ ((hb32 >> (57-32)) & 0x1) ^ ((hb32 >> (59-32)) & 0x1) ^ ((hb32 >> (61-32)) & 0x1) ^ ((hb32 >> (63-32)) & 0x1);
    parity_wr_bit1 = ((lb32 >> 0) & 0x1) ^ ((lb32 >> 2) & 0x1) ^ ((lb32 >> 3) & 0x1) ^ ((lb32 >> 5) & 0x1) ^ ((lb32 >> 6) & 0x1) ^ ((lb32 >> 9) & 0x1) ^ ((lb32 >> 10) & 0x1) ^ ((lb32 >> 12) & 0x1) ^ ((lb32 >> 13) & 0x1) ^ ((lb32 >> 16) & 0x1) ^ ((lb32 >> 17) & 0x1) ^ ((lb32 >> 20) & 0x1) ^ ((lb32 >> 21) & 0x1) ^ ((lb32 >> 24) & 0x1) ^ ((lb32 >> 25) & 0x1) ^ ((lb32 >> 27) & 0x1) ^ ((lb32 >> 28) & 0x1) ^ ((lb32 >> 31) & 0x1) ^ ((hb32 >> (32-32)) & 0x1) ^ ((hb32 >> (35-32)) & 0x1) ^ ((hb32 >> (36-32)) & 0x1) ^ ((hb32 >> (39-32)) & 0x1) ^ ((hb32 >> (40-32)) & 0x1) ^ ((hb32 >> (43-32)) & 0x1) ^ ((hb32 >> (44-32)) & 0x1) ^ ((hb32 >> (47-32)) & 0x1) ^ ((hb32 >> (48-32)) & 0x1) ^ ((hb32 >> (51-32)) & 0x1) ^ ((hb32 >> (52-32)) & 0x1) ^ ((hb32 >> (55-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1) ^ ((hb32 >> (58-32)) & 0x1) ^ ((hb32 >> (59-32)) & 0x1) ^ ((hb32 >> (62-32)) & 0x1) ^ ((hb32 >> (63-32)) & 0x1);
    parity_wr_bit2 = ((lb32 >> 1) & 0x1) ^ ((lb32 >> 2) & 0x1) ^ ((lb32 >> 3) & 0x1) ^ ((lb32 >> 7) & 0x1) ^ ((lb32 >> 8) & 0x1) ^ ((lb32 >> 9) & 0x1) ^ ((lb32 >> 10) & 0x1) ^ ((lb32 >> 14) & 0x1) ^ ((lb32 >> 15) & 0x1) ^ ((lb32 >> 16) & 0x1) ^ ((lb32 >> 17) & 0x1) ^ ((lb32 >> 22) & 0x1) ^ ((lb32 >> 23) & 0x1) ^ ((lb32 >> 24) & 0x1) ^ ((lb32 >> 25) & 0x1) ^ ((lb32 >> 29) & 0x1) ^ ((lb32 >> 30) & 0x1) ^ ((lb32 >> 31) & 0x1) ^ ((hb32 >> (32-32)) & 0x1) ^ ((hb32 >> (37-32)) & 0x1) ^ ((hb32 >> (38-32)) & 0x1) ^ ((hb32 >> (39-32)) & 0x1) ^ ((hb32 >> (40-32)) & 0x1) ^ ((hb32 >> (45-32)) & 0x1) ^ ((hb32 >> (46-32)) & 0x1) ^ ((hb32 >> (47-32)) & 0x1) ^ ((hb32 >> (48-32)) & 0x1) ^ ((hb32 >> (53-32)) & 0x1) ^ ((hb32 >> (54-32)) & 0x1) ^ ((hb32 >> (55-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1) ^ ((hb32 >> (60-32)) & 0x1) ^ ((hb32 >> (61-32)) & 0x1) ^ ((hb32 >> (62-32)) & 0x1) ^ ((hb32 >> (63-32)) & 0x1);
    parity_wr_bit3 = ((lb32 >> 4) & 0x1) ^ ((lb32 >> 5) & 0x1) ^ ((lb32 >> 6) & 0x1) ^ ((lb32 >> 7) & 0x1) ^ ((lb32 >> 8) & 0x1) ^ ((lb32 >> 9) & 0x1) ^ ((lb32 >> 10) & 0x1) ^ ((lb32 >> 18) & 0x1) ^ ((lb32 >> 19) & 0x1) ^ ((lb32 >> 20) & 0x1) ^ ((lb32 >> 21) & 0x1) ^ ((lb32 >> 22) & 0x1) ^ ((lb32 >> 23) & 0x1) ^ ((lb32 >> 24) & 0x1) ^ ((lb32 >> 25) & 0x1) ^ ((hb32 >> (33-32)) & 0x1) ^ ((hb32 >> (34-32)) & 0x1) ^ ((hb32 >> (35-32)) & 0x1) ^ ((hb32 >> (36-32)) & 0x1) ^ ((hb32 >> (37-32)) & 0x1) ^ ((hb32 >> (38-32)) & 0x1) ^ ((hb32 >> (39-32)) & 0x1) ^ ((hb32 >> (40-32)) & 0x1) ^ ((hb32 >> (49-32)) & 0x1) ^ ((hb32 >> (50-32)) & 0x1) ^ ((hb32 >> (51-32)) & 0x1) ^ ((hb32 >> (52-32)) & 0x1) ^ ((hb32 >> (53-32)) & 0x1) ^ ((hb32 >> (54-32)) & 0x1) ^ ((hb32 >> (55-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1);
    parity_wr_bit4 = ((lb32 >> 11) & 0x1) ^ ((lb32 >> 12) & 0x1) ^ ((lb32 >> 13) & 0x1) ^ ((lb32 >> 14) & 0x1) ^ ((lb32 >> 15) & 0x1) ^ ((lb32 >> 16) & 0x1) ^ ((lb32 >> 17) & 0x1) ^ ((lb32 >> 18) & 0x1) ^ ((lb32 >> 19) & 0x1) ^ ((lb32 >> 20) & 0x1) ^ ((lb32 >> 21) & 0x1) ^ ((lb32 >> 22) & 0x1) ^ ((lb32 >> 23) & 0x1) ^ ((lb32 >> 24) & 0x1) ^ ((lb32 >> 25) & 0x1) ^ ((hb32 >> (41-32)) & 0x1) ^ ((hb32 >> (42-32)) & 0x1) ^ ((hb32 >> (43-32)) & 0x1) ^ ((hb32 >> (44-32)) & 0x1) ^ ((hb32 >> (45-32)) & 0x1) ^ ((hb32 >> (46-32)) & 0x1) ^ ((hb32 >> (47-32)) & 0x1) ^ ((hb32 >> (48-32)) & 0x1) ^ ((hb32 >> (49-32)) & 0x1) ^ ((hb32 >> (50-32)) & 0x1) ^ ((hb32 >> (51-32)) & 0x1) ^ ((hb32 >> (52-32)) & 0x1) ^ ((hb32 >> (53-32)) & 0x1) ^ ((hb32 >> (54-32)) & 0x1) ^ ((hb32 >> (55-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1);
    parity_wr_bit5 = ((lb32 >> 26) & 0x1) ^ ((lb32 >> 27) & 0x1) ^ ((lb32 >> 28) & 0x1) ^ ((lb32 >> 29) & 0x1) ^ ((lb32 >> 30) & 0x1) ^ ((lb32 >> 31) & 0x1) ^ ((hb32 >> (32-32)) & 0x1) ^ ((hb32 >> (33-32)) & 0x1) ^ ((hb32 >> (34-32)) & 0x1) ^ ((hb32 >> (35-32)) & 0x1) ^ ((hb32 >> (36-32)) & 0x1) ^ ((hb32 >> (37-32)) & 0x1) ^ ((hb32 >> (38-32)) & 0x1) ^ ((hb32 >> (39-32)) & 0x1) ^ ((hb32 >> (40-32)) & 0x1) ^ ((hb32 >> (41-32)) & 0x1) ^ ((hb32 >> (42-32)) & 0x1) ^ ((hb32 >> (43-32)) & 0x1) ^ ((hb32 >> (44-32)) & 0x1) ^ ((hb32 >> (45-32)) & 0x1) ^ ((hb32 >> (46-32)) & 0x1) ^ ((hb32 >> (47-32)) & 0x1) ^ ((hb32 >> (48-32)) & 0x1) ^ ((hb32 >> (49-32)) & 0x1) ^ ((hb32 >> (50-32)) & 0x1) ^ ((hb32 >> (51-32)) & 0x1) ^ ((hb32 >> (52-32)) & 0x1) ^ ((hb32 >> (53-32)) & 0x1) ^ ((hb32 >> (54-32)) & 0x1) ^ ((hb32 >> (55-32)) & 0x1) ^ ((hb32 >> (56-32)) & 0x1);
    parity_wr_bit6 = ((hb32 >> (57-32)) & 0x1) ^ ((hb32 >> (58-32)) & 0x1) ^ ((hb32 >> (59-32)) & 0x1) ^ ((hb32 >> (60-32)) & 0x1) ^ ((hb32 >> (61-32)) & 0x1) ^ ((hb32 >> (62-32)) & 0x1) ^ ((hb32 >> (63-32)) & 0x1);
    parity_wr_bit7 = parity_wr_bit0 ^ parity_wr_bit1 ^ parity_wr_bit2 ^ parity_wr_bit3 ^ parity_wr_bit4 ^ parity_wr_bit5 ^ parity_wr_bit6;

    i = 0;
    while(i <32) {
        parity_wr_bit7 = parity_wr_bit7 ^ ((lb32 >> i) & 0x1);
        i++;
    }
    j = 0;
    while(j <32) {
    parity_wr_bit7 = parity_wr_bit7 ^ ((hb32 >> j) & 0x1);
    j++;
    }
    
    parity_wr = (parity_wr_bit7 << 7) + (parity_wr_bit6 << 6) + (parity_wr_bit5 << 5) + (parity_wr_bit4 << 4) + (parity_wr_bit3 << 3) + (parity_wr_bit2 << 2) + (parity_wr_bit1 << 1) + (parity_wr_bit0 << 0); 

    return parity_wr;

}
void tb_nvr_program_customer_data_01()
{
	int i =0;
	int j =0;
	int k =0;
	k = i + j*29;
	printf("tb_nvr_program_customer_data_01\n");	
	ulConfigUbm("SI", C28, C27);
START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
		
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
		//for(int i=0;i<29;i++)
		for(i=0;i<29;i++)
                {
			printf("flash no is %d\tindex is %d\t1:0x%x\td2:0x%x\td3:0x%x\n",g_flash_no,i,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2]);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x80000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x800000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP
}
void tb_nvr_program_customer_data_05()
{
	printf("tb_nvr_program_customer_data_05\n");

START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
			//0x101 flash0 Vcode=8-11 
			nvr_data_iterate_vcode_H8[0][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_H8[0][1]=0x0|(vref10_value_code[11][dut-1]<<16)|(vref10_value_code[10][dut-1]);   //bit[63:32]=  bit[63:48](<<16)|bit[47:32]	
			nvr_data_iterate_vcode_H8[0][2]=0x0|(vref10_value_code[9][dut-1]<<16)|(vref10_value_code[8][dut-1]);   //bit[31:0]=  bit[31:16](<<16)|bit[15:0]	

			//0x101 flash1 Vcode=12-15 
			nvr_data_iterate_vcode_H8[1][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_H8[1][1]=0x0|(vref10_value_code[15][dut-1]<<16)|(vref10_value_code[14][dut-1]);   //bit[63:32]=  bit[63:48](<<16)|bit[47:32]	
			nvr_data_iterate_vcode_H8[1][2]=0x0|(vref10_value_code[13][dut-1]<<16)|(vref10_value_code[12][dut-1]);   //bit[31:0]=  bit[31:16](<<16)|bit[15:0]	

	
			//0x101 flash2 all 0xff 
			nvr_data_iterate_vcode_H8[2][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_H8[2][1]=0xffffffff;	
			nvr_data_iterate_vcode_H8[2][2]=0xffffffff;	
 		
			//0x101 flash3 all 0xff 
			nvr_data_iterate_vcode_H8[3][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_H8[3][1]=0xffffffff;	
			nvr_data_iterate_vcode_H8[3][2]=0xffffffff;	
	
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
                {
			printf("flash no is %d\td1_hh8:0x%x\td2_h32:0x%x\td3_l32:0x%x\n",g_flash_no,nvr_data_iterate_vcode_H8[g_flash_no-1][0],nvr_data_iterate_vcode_H8[g_flash_no-1][1],nvr_data_iterate_vcode_H8[g_flash_no-1][2]);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR_ITERATE_VCODE_H8[g_flash_no-1]+0x800000,1,nvr_data_iterate_vcode_H8[g_flash_no-1][0],nvr_data_iterate_vcode_H8[g_flash_no-1][1],nvr_data_iterate_vcode_H8[g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP


}

void tb_nvr_program_customer_data_04()
{
	printf("tb_nvr_program_customer_data_04\n");

START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
			//0x101 flash0 Vcode=0-3 
			nvr_data_iterate_vcode_L8[0][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_L8[0][1]=0x0|(vref10_value_code[3][dut-1]<<16)|(vref10_value_code[2][dut-1]);   //bit[63:32]=  bit[63:48](<<16)|bit[47:32]	
			nvr_data_iterate_vcode_L8[0][2]=0x0|(vref10_value_code[1][dut-1]<<16)|(vref10_value_code[0][dut-1]);   //bit[31:0]=  bit[31:16](<<16)|bit[15:0]	

			//0x101 flash1 Vcode=4-7 
			nvr_data_iterate_vcode_L8[1][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_L8[1][1]=0x0|(vref10_value_code[7][dut-1]<<16)|(vref10_value_code[6][dut-1]);   //bit[63:32]=  bit[63:48](<<16)|bit[47:32]	
			nvr_data_iterate_vcode_L8[1][2]=0x0|(vref10_value_code[5][dut-1]<<16)|(vref10_value_code[4][dut-1]);   //bit[31:0]=  bit[31:16](<<16)|bit[15:0]	

	
			//0x101 flash2 all 0xff 
			nvr_data_iterate_vcode_L8[2][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_L8[2][1]=0xffffffff;	
			nvr_data_iterate_vcode_L8[2][2]=0xffffffff;	
 		
			//0x101 flash3 all 0xff 
			nvr_data_iterate_vcode_L8[3][0]=0xff;   //bit[71:64]=8'hFF	
			nvr_data_iterate_vcode_L8[3][1]=0xffffffff;	
			nvr_data_iterate_vcode_L8[3][2]=0xffffffff;	
	
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
                {
			printf("flash no is %d\td1_hh8:0x%x\td2_h32:0x%x\td3_l32:0x%x\n",g_flash_no,nvr_data_iterate_vcode_L8[g_flash_no-1][0],nvr_data_iterate_vcode_L8[g_flash_no-1][1],nvr_data_iterate_vcode_L8[g_flash_no-1][2]);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR_ITERATE_VCODE_L8[g_flash_no-1]+0x800000,1,nvr_data_iterate_vcode_L8[g_flash_no-1][0],nvr_data_iterate_vcode_L8[g_flash_no-1][1],nvr_data_iterate_vcode_L8[g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP

}


void tb_nvr_program_customer_data_03()
{
/*
Vcode bit[3:0];
bit[16] 0
others is 1
0xff_0xffffffff_0xfffefff0
*/
	printf("tb_nvr_program_customer_data_03\n");

START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
			nvr_data_final_vcode[0][0]=0xff;	
			nvr_data_final_vcode[0][1]=0xffffffff;	
			//nvr_data_final_vcode[0][2]=0xfffefff0;
			nvr_data_final_vcode[0][2]=0xfffefff0|VcodeTmp[dut-1];

			nvr_data_final_vcode[1][0]=0xff;	
			nvr_data_final_vcode[1][1]=0xffffffff;	
			nvr_data_final_vcode[1][2]=0xffffffff;
	
 		
			nvr_data_final_vcode[2][0]=0xff;	
			nvr_data_final_vcode[2][1]=0xffffffff;	
			nvr_data_final_vcode[2][2]=0xffffffff;
		
	
			nvr_data_final_vcode[3][0]=0xff;	
			nvr_data_final_vcode[3][1]=0xffffffff;	
			nvr_data_final_vcode[3][2]=0xffffffff;
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
                {
			printf("flash no is %d\td1__hh8:0x%x\td2_h32:0x%x\td3_l32:0x%x\n",g_flash_no,nvr_data_final_vcode[g_flash_no-1][0],nvr_data_final_vcode[g_flash_no-1][1],nvr_data_final_vcode[g_flash_no-1][2]);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x80000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x800000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR_FINAL_VCODE[g_flash_no-1]+0x800000,1,nvr_data_final_vcode[g_flash_no-1][0],nvr_data_final_vcode[g_flash_no-1][1],nvr_data_final_vcode[g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP

}





void tb_nvr_program_customer_data_02()
{
/*
#define WAFER_LOT_ID_ADDR 0x700E8
#define COORD_WAFER_ID_ADDR 0x700EC

Ubm_Write_Push("SI",ADD_LOC_XY_L  ,1, ADD_LOC_ECC[dut-1], WAFERID_h32[dut-1],WAFERID_l32[dut-1],  dut);//--2 group data
Ubm_Write_Push("SI",ADD_WAFERID_L ,1, WAFERID_ECC[dut-1], LOTID_h32[dut-1],LOTID_l32[dut-1],  dut);//--2 group data
unsigned short CalcEcc(unsigned int address, unsigned int data_H, unsigned int data_L)			
extern int WAFER_LOT_ID_ECC[DDUTCNT];
extern int COORD_WAFER_ID_ECC[DDUTCNT];
extern int NVR_FLASH_ADDR_LOT_COORD[FLASH_NUM];
extern int nvr_data_lot_coord[FLASH_NUM][DATA_NUM];
*/
	printf("tb_nvr_program_customer_data_02\n");
        	
//	START_DUT_LOOP(UT_CDUT)
//		//WAFER_LOT_ID_ECC[dut-1] = CalcEcc(WAFER_LOT_ID_ADDR,0x5555AAAA,0xAAAA5555);
//		WAFER_LOT_ID_ECC[dut-1] = CalcEcc(WAFER_LOT_ID_ADDR,LOTID_h32[dut-1],LOTID_l32[dut-1]);
//		printf("dut[%d] 's wafer_lot_id ecc is 0x%x\n",dut,WAFER_LOT_ID_ECC[dut-1]);
//		
//		COORD_WAFER_ID_ECC[dut-1] = CalcEcc(COORD_WAFER_ID_ADDR,WAFERID_h32[dut-1],WAFERID_l32[dut-1]);
//		printf("dut[%d] 's coord_wafer_id ecc is 0x%x\n",dut,COORD_WAFER_ID_ECC[dut-1]);
//        END_DUT_LOOP

START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
		//WAFER_LOT_ID_ECC[dut-1] = CalcEcc(WAFER_LOT_ID_ADDR,LOTID_h32[dut-1],LOTID_l32[dut-1]);
		WAFER_LOT_ID_ECC[dut-1] = CalcEcc(WAFER_LOT_ID_ADDR,LOTID_h32_nc_reload[dut-1],LOTID_l32_nc_reload[dut-1]);
		printf("dut[%d] 's wafer_lot_id ecc is 0x%x\n",dut,WAFER_LOT_ID_ECC[dut-1]);
	        nvr_data_lot_coord[0][0]=WAFER_LOT_ID_ECC[dut-1];	
	        
		//nvr_data_lot_coord[0][1]=LOTID_h32[dut-1];	
	        nvr_data_lot_coord[0][1]=LOTID_h32_nc_reload[dut-1];	
	        
		//nvr_data_lot_coord[0][2]=LOTID_l32[dut-1];
		nvr_data_lot_coord[0][2]=LOTID_l32_nc_reload[dut-1];

		//COORD_WAFER_ID_ECC[dut-1] = CalcEcc(COORD_WAFER_ID_ADDR,WAFERID_h32[dut-1],WAFERID_l32[dut-1]);
		COORD_WAFER_ID_ECC[dut-1] = CalcEcc(COORD_WAFER_ID_ADDR,WAFERID_h32_nc_reload[dut-1],WAFERID_l32_nc_reload[dut-1]);
		printf("dut[%d] 's coord_wafer_id ecc is 0x%x\n",dut,COORD_WAFER_ID_ECC[dut-1]);
	        nvr_data_lot_coord[1][0]=COORD_WAFER_ID_ECC[dut-1];	
	        
		//nvr_data_lot_coord[1][1]=WAFERID_h32[dut-1];	
		nvr_data_lot_coord[1][1]=WAFERID_h32_nc_reload[dut-1];	
	        
		//nvr_data_lot_coord[1][2]=WAFERID_l32[dut-1];	
		nvr_data_lot_coord[1][2]=WAFERID_l32_nc_reload[dut-1];	
	
	        nvr_data_lot_coord[2][0]=0xF5;	
	        nvr_data_lot_coord[2][1]=0xffffffff;	
	        nvr_data_lot_coord[2][2]=0xffffffff;	
 		
		
	        nvr_data_lot_coord[3][0]=0x6B;	
	        nvr_data_lot_coord[3][1]=0xffffffff;	
	        nvr_data_lot_coord[3][2]=0xffffffff;
	
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
		//for(int i=0;i<29;i++)
		//for(i=(0+(j*29));i<((j+1)*29);i++)
                {
			printf("flash no is %d\td1_hh8:0x%x\td2_h32:0x%x\td3_l32:0x%x\n",g_flash_no,nvr_data_lot_coord[g_flash_no-1][0],nvr_data_lot_coord[g_flash_no-1][1],nvr_data_lot_coord[g_flash_no-1][2]);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x80000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x800000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR_LOT_COORD[g_flash_no-1]+0x800000,1,nvr_data_lot_coord[g_flash_no-1][0],nvr_data_lot_coord[g_flash_no-1][1],nvr_data_lot_coord[g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP

}


void tb_nvr_program_customer_data()
{
	printf("tb_nvr_program_customer_data\n");
	for(int j=0;j<9;j++)
{
//	tb_nvr_program_customer_data_01();	
	int i =0;
	printf("tb_nvr_program_customer_data_01\n");	
	ulConfigUbm("SI", C28, C27);
START_FLASH_LOOP
	setRegAddr(AREA_N);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW

	START_DUT_LOOP(UT_CDUT)
		
 		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
		//for(int i=0;i<29;i++)
		for(i=(0+(j*29));i<((j+1)*29);i++)
                {
	//		printf("flash no is %d\tindex is %d\t1:0x%x\td2:0x%x\td3:0x%x\n",g_flash_no,i,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2]);
	        	//Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x80000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
	        	Ubm_Write_Push("SI",NVR_FLASH_ADDR[i][g_flash_no-1]+0x800000,1,nvr_data[i][g_flash_no-1][0],nvr_data[i][g_flash_no-1][1],nvr_data[i][g_flash_no-1][2],dut);
                }
        END_DUT_LOOP
	//	FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM");
		FlashPatExec("patFlash.mpa", "FN_NORL_PROG_UBM_UBM");
END_FLASH_LOOP

}
	tb_nvr_program_customer_data_02();	
	tb_nvr_program_customer_data_03();	
	tb_nvr_program_customer_data_04();	
	tb_nvr_program_customer_data_05();	
}
void tb_nvr_cfg_program_soc_trim()
{

		ulConfigUbm("SI", C28, C27);
#ifdef FOLLOW_CUSTOMER_RULE
		
		InitialCpDieId_jx2(LOTID_h32,LOTID_l32 ,WAFERID_h32,WAFERID_l32);
		//LOTID_h32,LOTID_l32,WAFERID_h32,WAFERID_l32 for nc save
START_FLASH_LOOP
	setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW  note:must to be added 
	START_DUT_LOOP(UT_CDUT)
		ADD_LOC_ECC[dut-1]=0xff;
                WAFERID_ECC[dut-1]=0xff;
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",ADD_LOC_XY_L  ,1, ADD_LOC_ECC[dut-1], WAFERID_h32[dut-1],WAFERID_l32[dut-1],  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_WAFERID_L ,1, WAFERID_ECC[dut-1], LOTID_h32[dut-1],LOTID_l32[dut-1],  dut);//--2 group data
	END_DUT_LOOP
		FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");
END_FLASH_LOOP
#endif

}
void tb_nvr_cfg_read_Vcode(){
		ulConfigUbm("SO_PINS", C28, C27);
		START_FLASH_LOOP
		setRegAddr(AREA_NC);//--set  add/IDX1 loop num  for  NVR_CFG_READ_UBM_UBM
		if(g_flash_no == 1)
{
			START_DUT_LOOP(UT_CDUT)
#if 1
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",ADD_LOC_F_VCODE  ,1, 0xff, 0xffffffff,0xfffffff0 | VcodeTmp[dut-1],  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_L6  ,1, (vref10_value_code[VREF10_trim_value[11]][dut-1]>>4)&0xff,(vref10_value_code[VREF10_trim_value[11]][dut-1]&0xf)<<28|(vref10_value_code[VREF10_trim_value[12]][dut-1]<<16)|(vref10_value_code[VREF10_trim_value[13]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[14]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[14]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[15]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[8]][dut-1]),  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_M6  ,1, (vref10_value_code[VREF10_trim_value[4]][dut-1]>>4)&0xff,(vref10_value_code[VREF10_trim_value[4]][dut-1]&0xf)<<28|(vref10_value_code[VREF10_trim_value[5]][dut-1]<<16)|(vref10_value_code[VREF10_trim_value[6]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[7]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[7]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[9]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[10]][dut-1]),  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_H4  ,1, 0xff,0xffff0000|(vref10_value_code[VREF10_trim_value[0]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[1]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[1]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[2]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[3]][dut-1]),  dut);//--2 group data
#endif


#if 0
			ulWriteUbm("SI_PINS",0*PROG_DATA_BITSIZE,1*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff,  0xfffffff0 | VcodeTmp[dut-1], dut);//Vcode'
			ulWriteUbm("SI_PINS",1*PROG_DATA_BITSIZE,2*PROG_DATA_BITSIZE-1, 0xff, 0xfffffff0 | (vref10_value_code[2][dut-1]>>8), (vref10_value_code[2][dut-1]&0xff)<<24|vref10_value_code[1][dut-1]<<12|vref10_value_code[0][dut-1], dut);//36bit: V2 Value  V1 Value  V0 Value
			ulWriteUbm("SI_PINS",2*PROG_DATA_BITSIZE,3*PROG_DATA_BITSIZE-1, 0xff, 0xfffffff0 | (vref10_value_code[5][dut-1]>>8), (vref10_value_code[5][dut-1]&0xff)<<24|vref10_value_code[4][dut-1]<<12|vref10_value_code[3][dut-1], dut);//36bit: V5  Value  V4 Value  V3 Value
			ulWriteUbm("SI_PINS",3*PROG_DATA_BITSIZE,4*PROG_DATA_BITSIZE-1, 0xff, 0xfffffff0 | (vref10_value_code[8][dut-1]>>8), (vref10_value_code[8][dut-1]&0xff)<<24|vref10_value_code[7][dut-1]<<12|vref10_value_code[6][dut-1], dut);//36bit: V8 Value  V7 Value  V6 Value
			ulWriteUbm("SI_PINS",4*PROG_DATA_BITSIZE,5*PROG_DATA_BITSIZE-1, 0xff, 0xfffffff0 | (vref10_value_code[11][dut-1]>>8),(vref10_value_code[11][dut-1]&0xff)<<24|vref10_value_code[10][dut-1]<<12|vref10_value_code[9][dut-1], dut);//36bit: V11 Value  V10 Value  V9 Value
			ulWriteUbm("SI_PINS",5*PROG_DATA_BITSIZE,6*PROG_DATA_BITSIZE-1, 0xff, 0xfffffff0 | (vref10_value_code[14][dut-1]>>8),(vref10_value_code[14][dut-1]&0xff)<<24|vref10_value_code[13][dut-1]<<12|vref10_value_code[12][dut-1], dut);//36bit: V14 Value  V13 Value  V12 Value
			ulWriteUbm("SI_PINS",6*PROG_DATA_BITSIZE,7*PROG_DATA_BITSIZE-1, 0xff, 0xffffffff, 0xfffff000|vref10_value_code[15][dut-1], dut);//36bit: V15 Value
#endif
			END_DUT_LOOP
					FlashPatExec("patFlash.mpa", "NVR_CFG_READ_UBM_UBM");
			//FlashPatExec("patFlash.mpa", "NVR_CFG_READ_VCODE_TCODE");
}

		END_FLASH_LOOP
}
void tb_nvr_cfg_program_Vcode(){
		ulConfigUbm("SI_PINS", C28, C27);
		START_FLASH_LOOP
	setRegAddr(AREA_NC);//--set  area and IDX4/5  for NVR_CFG_PROG_UBM_UBM_2ROW  note:must to be added 
		if(g_flash_no == 1)
{
			START_DUT_LOOP(UT_CDUT)
		for(int i = 0; i < VREF10_trimcount; i++){
			//printf("Dut %d, Vcode=%d , VREF10 ==%f,VREF10_value[%d]==%d\n",dut, VREF10_trim_value[i], vref10_trim_result[i][dut-1],i,vref10_value_code[i][dut-1]);
			//printf("\n-----------------------------------------\n");
			printf("Dut %d, Vcode=%d , VREF10 ==%f,VREF10_value[%d]==%d\n",dut, VREF10_trim_value[i], vref10_trim_result[i][dut-1],i,vref10_value_code[VREF10_trim_value[i]][dut-1]);


		}
#if 1
		Ubm_Write_Push("SI",0 , -1,0 , 0,0,  dut);//--init push func
 		Ubm_Write_Push("SI",ADD_LOC_F_VCODE  ,1, 0xff, 0xffffffff,0xfffffff0 | VcodeTmp[dut-1],  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_L6  ,1, (vref10_value_code[VREF10_trim_value[11]][dut-1]>>4)&0xff,(vref10_value_code[VREF10_trim_value[11]][dut-1]&0xf)<<28|(vref10_value_code[VREF10_trim_value[12]][dut-1]<<16)|(vref10_value_code[VREF10_trim_value[13]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[14]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[14]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[15]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[8]][dut-1]),  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_M6  ,1, (vref10_value_code[VREF10_trim_value[4]][dut-1]>>4)&0xff,(vref10_value_code[VREF10_trim_value[4]][dut-1]&0xf)<<28|(vref10_value_code[VREF10_trim_value[5]][dut-1]<<16)|(vref10_value_code[VREF10_trim_value[6]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[7]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[7]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[9]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[10]][dut-1]),  dut);//--2 group data
 		Ubm_Write_Push("SI",ADD_LOC_I_VCODE_H4  ,1, 0xff,0xffff0000|(vref10_value_code[VREF10_trim_value[0]][dut-1]<<4)|(vref10_value_code[VREF10_trim_value[1]][dut-1]>>8), ((vref10_value_code[VREF10_trim_value[1]][dut-1]&0xff)<<24)|(vref10_value_code[VREF10_trim_value[2]][dut-1]<<12)|(vref10_value_code[VREF10_trim_value[3]][dut-1]),  dut);//--2 group data
#endif
	END_DUT_LOOP
	        FlashPatExec("patFlash.mpa", "NVR_CFG_PROG_UBM_UBM_2ROW");
}
		END_FLASH_LOOP
}


//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 5:others

void  ReadFlash(unsigned int AREA,unsigned int addr)
{
	unsigned int DATA3[1][DDUTCNT][FLASH_NUM],DATA2[1][DDUTCNT][FLASH_NUM],DATA1[1][DDUTCNT][FLASH_NUM];
		ReadFlashNC(AREA, 0x0,0x0,0x1,addr,DATA3,DATA2,DATA1,"FN_SLOW_READ_FCM_1");
			START_FLASH_LOOP
			START_DUT_LOOP(UT_CDUT)
				temp3[dut-1][g_flash_no-1]=DATA3[0][dut-1][g_flash_no-1];
				temp2[dut-1][g_flash_no-1]=DATA2[0][dut-1][g_flash_no-1];
				temp1[dut-1][g_flash_no-1]=DATA1[0][dut-1][g_flash_no-1];
			END_DUT_LOOP
			END_FLASH_LOOP

}
void ReadFlashNC(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname)
{
	int ybits = 0, xbits=0;
	int transfer_bits = 13;//13??
	int dataBitSize = FCM_READ_DATA_BITSIZE;	//7
	int tempyaddr=FCM_READ_DATA_BITSIZE-1; 
	int round_startaddr=(startaddr/(1<<transfer_bits))*(1<<transfer_bits);
//	printf("start_addr:%d end_addr:%d round_addr:%d\n",startaddr, endaddr, round_startaddr);
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
	printf("temp_xaddr:%d temp_yaddr:%d\n", tempxaddr, tempyaddr);
	printf("ybits:%d xbits:%d\n",ybits, xbits);
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
	unsigned int *d1=(unsigned int*)malloc(sizeof(unsigned int)*(72));//originally xbits = 0 and inside of the unsigned int *()1 >> xbit
	unsigned int *d2=(unsigned int*)malloc(sizeof(unsigned int)*(72));
	unsigned int *d3=(unsigned int*)malloc(sizeof(unsigned int)*(72));

 START_FLASH_LOOP
	setRegValue(UT_REG_D2C, area<<8);//set for Y15-8
	setRegValue(UT_REG_D1C, round_startaddr|addr_mask);//set for X23-0
	//printf("REG_D1C:%d\n",round_startaddr|addr_mask);
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

	FlashPatStart("patFlash.mpa", patname);


	if(g_flash_no == 1)
{
	START_DUT_LOOP(UT_CDUT)
		UTL_ConvertDutNumber(UT_SITEDUT,dut,UT_SYSDUT,NULL,&sysdut);
			memset(d1, 0, sizeof(unsigned int)*(72));//bit31:0
			memset(d2, 0, sizeof(unsigned int)*(72));//bit63:32
			memset(d3, 0, sizeof(unsigned int)*(72));//bit71:64
		ulReadFcm(fcm_data, (1<<xbits) * (1<<ybits), 0, endaddr-round_startaddr, 0, (1<<ybits)-1, dut);
			for(i=startaddr-round_startaddr;i<=endaddr-round_startaddr;i++)
			{
				for(j=0;j<(dataBitSize<32?dataBitSize:32);j++)
				{
					d1[i] |= (((fcm_data[i*(1<<ybits)+dataBitSize-1-j]))&1)<<j;
				//	printf("d1[%d]:%x fcm_data[%d]:%x\n", i, d1[i], i*(1<<ybits)+dataBitSize-1-j, fcm_data[i*(1<<ybits)+dataBitSize-1-j]);
				}
				for(j=32;j<(dataBitSize<64?dataBitSize:64);j++)
				{
					d2[i] |= (((fcm_data[i*(1<<ybits)+dataBitSize-1-j]))&1)<<(j-32);
				//	printf("d2[%d]:%x fcm_data[%d]:%x\n", i, d2[i], i*(1<<ybits)+dataBitSize-1-j, fcm_data[i*(1<<ybits)+dataBitSize-1-j]);
				}
				for(j=64;j<(dataBitSize<72?dataBitSize:72);j++)
				{
					d3[i] |= (((fcm_data[i*(1<<ybits)+dataBitSize-1-j]))&1)<<((j-64));
				//	printf("d3[%d]:%x fcm_data[%d]:%x\n", i, d3[i], i*(1<<ybits)+dataBitSize-1-j, fcm_data[i*(1<<ybits)+dataBitSize-1-j]);
				}
				DATA3[i][dut-1][g_flash_no-1]=d3[i];
				DATA2[i][dut-1][g_flash_no-1]=d2[i];
				DATA1[i][dut-1][g_flash_no-1]=d1[i];
				//printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, 0, d3[i], d2[i], d1[i]);fflush(stdout);
				printf("DUT[%i] read address[0x%05X], data = 0x%02X_%08X_%08X\n", dut, addr_mask, d3[i], d2[i], d1[i]);fflush(stdout);
				if(addr_mask == 0x807FC0){		//Vcode'
					LOTID_l32_nc_reload[dut-1]= d1[i];
					LOTID_h32_nc_reload[dut-1]= d2[i];
					printf("Debug:  LOTID_l32_nc_reload[%d]== 0x%x\n", dut,LOTID_l32_nc_reload[dut-1]);
					printf("Debug:  LOTID_h32_nc_reload[%d]== 0x%x\n", dut,LOTID_h32_nc_reload[dut-1]);
				}
                                if(addr_mask == 0x807FC1){		//Vcode'
					WAFERID_l32_nc_reload[dut-1]= d1[i];
					WAFERID_h32_nc_reload[dut-1]= d2[i];
					printf("Debug:  WAFERID_l32_nc_reload[%d]== 0x%x\n", dut,WAFERID_l32_nc_reload[dut-1]);
					printf("Debug:  WAFERID_h32_nc_reload[%d]== 0x%x\n", dut,WAFERID_h32_nc_reload[dut-1]);
				}

				if(addr_mask == 0x807FE0){		//Vcode'
					VcodeTmp[dut-1] = d1[i] & 0xf;
					//printf("Debug: Vcode = %x\n", VcodeTmp[dut-1]);
				}else if(addr_mask == 0x807FE1){	//Vcode=0000,0001,0010,0011,0100,0101 Tcode=8
					vref10_value_code[0][dut-1]=d1[i] & 0xfff;
					vref10_value_code[1][dut-1]=d1[i]>>12 & 0xfff;
					vref10_value_code[2][dut-1]=(d1[i]>>24) & 0xfff | (d2[i]&0xf)<<8;
					vref10_value_code[3][dut-1]=(d2[i]>>4) & 0xfff;
					vref10_value_code[4][dut-1]=(d2[i]>>16) & 0xfff;
					vref10_value_code[5][dut-1]=(d2[i]>>28) & 0xfff | (d3[i]&0xff)<<4;
					printf("Debug: vref10_value_code[5]:0x%x, vref10_value_code[4]:0x%x, vref10_value_code[3]:0x%x, vref10_value_code[2]:0x%x, vref10_value_code[1]:0x%x, vref10_value_code[0]:0x%x\n", vref10_value_code[5][dut-1],vref10_value_code[4][dut-1], vref10_value_code[3][dut-1],vref10_value_code[2][dut-1],vref10_value_code[1][dut-1],vref10_value_code[0][dut-1]);
				}else if(addr_mask == 0x807FE2){	//Vcode=0110,0111,1000,1001,1010,1011 Tcode=8
					vref10_value_code[6][dut-1]=d1[i] & 0xfff;
					vref10_value_code[7][dut-1]=d1[i]>>12 & 0xfff;
					vref10_value_code[8][dut-1]=(d1[i]>>24) & 0xfff | (d2[i]&0xf)<<8;
					vref10_value_code[9][dut-1]=(d2[i]>>4) & 0xfff;
					vref10_value_code[10][dut-1]=(d2[i]>>16) & 0xfff;
					vref10_value_code[11][dut-1]=(d2[i]>>28) & 0xfff | (d3[i]&0xff)<<4;
					printf("Debug: vref10_value_code[11]:0x%x, vref10_value_code[10]:0x%x, vref10_value_code[9]:0x%x, vref10_value_code[8]:0x%x, vref10_value_code[7]:0x%x, vref10_value_code[6]:0x%x\n", vref10_value_code[11][dut-1],vref10_value_code[10][dut-1], vref10_value_code[9][dut-1],vref10_value_code[8][dut-1],vref10_value_code[7][dut-1],vref10_value_code[6][dut-1]);
					//printf("Debug: 0x22 ,%x, %x, %x, %x, %x, %x\n", vref10_value_code[11][dut-1],vref10_value_code[10][dut-1], vref10_value_code[9][dut-1],vref10_value_code[8][dut-1],vref10_value_code[7][dut-1],vref10_value_code[6][dut-1]);
				}else if(addr_mask == 0x807FE3){	//Vcode=1100,1101,1110,1111 Tcode=8; Tcode=0000,000 Vcode=Vcode'
					vref10_value_code[12][dut-1]=d1[i] & 0xfff;
					vref10_value_code[13][dut-1]=d1[i]>>12 & 0xfff;
					vref10_value_code[14][dut-1]=(d1[i]>>24) & 0xfff | (d2[i]&0xf)<<8;
					vref10_value_code[15][dut-1]=(d2[i]>>4) & 0xfff;
					//printf("Debug: 0x22 ,%x, %x, %x, %x\n", vref10_value_code[15][dut-1],vref10_value_code[14][dut-1], vref10_value_code[13][dut-1],vref10_value_code[12][dut-1]);
					printf("Debug:  vref10_value_code[15]:0x%x, vref10_value_code[14]:0x%x, vref10_value_code[13]:0x%x, vref10_value_code[12]:0x%x\n", vref10_value_code[15][dut-1],vref10_value_code[14][dut-1], vref10_value_code[13][dut-1],vref10_value_code[12][dut-1]);
				}
			}
	END_DUT_LOOP


}
END_FLASH_LOOP
	free(fcm_data);
		free(d1);
		free(d2);
		free(d3);
}


