#include "common.h"

void SetPowerSeq()
{
	PowerSeqHandle PowerSeq_H = UTL_GetPowerSeqHandle();
//	UTL_AddPowerSeqVs		(PowerSeq_H,    1 , vsno_vdd09);
	UTL_AddPowerSeqVs		(PowerSeq_H,    1 , vsno_vdd11);
	UTL_AddPowerSeqVs		(PowerSeq_H,    1 , vsno_vdd33);
	UTL_AddPowerSeqOthers   (PowerSeq_H,    2 );               
	UTL_SetPowerSeqWaitTime (PowerSeq_H,    1, 3.00e-3);
	//UTL_SetPowerSeqWaitTime (PowerSeq_H,    1, 0.2e-3);
	UTL_SetPowerSeqWaitTime (PowerSeq_H,    2, 3.00e-3);
	//UTL_SetPowerSeqWaitTime (PowerSeq_H,    2, 0.6e-3);
	UTL_SendPowerSeq        (PowerSeq_H);
	UTL_DeleteHandle        (PowerSeq_H);
}

void PowerOn(){
	UTL_SetWet();
	UTL_OnPowerSeq();
}

void PowerOff(){
	UTL_ResetWet();
	UTL_OffPowerSeq();
}

void SetLevel0V(){
	setVs(vsno_vdd11,  0 V, R16V, M400mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33,  0 V, R16V, M400mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);
	setVi(VDD33_NOM,  0, vi_io);
	setVo(VDD33_NOM/2,0, vo_io);
	setVt(VDD33_NOM/2,   vt_io);

//-- why not set as below
//-- 	setVi(0 V,  0, vi_io);
//-- 	setVo(0 V,  0, vo_io);
//-- 	setVt(0 V,     vt_io);

}

void SetLevelLow_SOC(){
	//setVs(vsno_vdd11, VDD11_MIN, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd11, VDD11_MIN, R16V, M400mA, 200 MA, -100 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd33, VDD33_MIN, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33, VDD33_MIN, R16V, M400mA, 200 MA, -100 MA, 400 MA, -200 MA, 5);
	setVi(VDD33_NOM,  0, vi_io);
	setVo(VDD33_NOM/2,0, vo_io);
	setVt(VDD33_NOM/2,   vt_io);
}

void SetLevelNorm_SOC(){
	//setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd11, VDD11_NOM, R16V, M400mA, 400 MA, -200 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 200 MA, -200 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33, VDD33_NOM, R16V, M400mA, 400 MA, -200 MA, 400 MA, -200 MA, 5);
	setVi(VDD33_NOM,  0, vi_io);
	setVo(VDD33_NOM/2,0, vo_io);
	setVt(VDD33_NOM/2,   vt_io);
}

void SetLevelHigh_SOC(){
	//setVs(vsno_vdd11, VDD11_MAX, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd11, VDD11_MAX, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd33, VDD33_MAX, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33, VDD33_MAX, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
	setVi(VDD33_NOM,  0, vi_io);
	setVo(VDD33_NOM/2,0, vo_io);
	setVt(VDD33_NOM/2,   vt_io);
}

void SetLevelNorm(){
	SetLevelNorm_SOC();
	//--add some soc set for CP test ,etc. LDO set code.
	UTL_WaitTime(10 MS);
}

void SetLevelLow(){
	SetLevelLow_SOC();
	//--add some soc set for CP test ,etc. LDO set code.
}

void SetLevelHigh(){
	SetLevelHigh_SOC();
	//--add some soc set for CP test ,etc. LDO set code.
}

void SetLevelHigh_1P2MAX(){
	//setVs(vsno_vdd11, VDD11_1P2MAX, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd11, VDD11_1P2MAX, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
	//setVs(vsno_vdd33, VDD33_1P2MAX, R16V, M400mA, 400 MA, -400 MA, 200 MA, -200 MA, 5);
	setVs(vsno_vdd33, VDD33_1P2MAX, R16V, M400mA, 400 MA, -100 MA, 400 MA, -200 MA, 5);
	setVi(VDD33_NOM,  0, vi_io);
	setVo(VDD33_NOM/2,0, vo_io);
	setVt(VDD33_NOM/2,   vt_io);
}

