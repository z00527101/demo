#ifndef _TB_EFA_FUNC_H
#define _TB_EFA_FUNC_H
#include "common.h"

extern char *tm_pinlist[FLASH_NUM];

void debug_set_active_dut(); 
void read_efa_file(); 

void tb_efa_cell_current_110_16Kx64();
void tb_efa_cell_current_40_128Kx64();
void tb_efa_cell_current_40_97Kx36();
void tb_efa_cell_current_40_16K32K64Kx72();
void cellcurrent_measure_modeonce_40_128Kx64(int dut,int start_addr,int end_addr);
void measure_cell_current_modeonce_40_128Kx64(int dut);

void measure_cell_current_110_16Kx64(int dut);
void measure_cell_current_40_128Kx64(int dut);
void measure_cell_current_40_97Kx36(int dut);
void measure_cell_current_40_16K32K64Kx72(int dut);

void cellcurrent_measure_110_16Kx64(int dut,int start_addr_a0,int end_addr_a0);
void cellcurrent_measure_40_128Kx64(int dut,int start_addr_a0,int end_addr_a0);
void cellcurrent_measure_40_97Kx36(int dut,int start_addr_a0,int end_addr_a0);
void cellcurrent_measure_40_16K32K64Kx72(int dut,int start_addr_a0,int end_addr_a0);

int query(int *bin_lab,int ext_bin_no);
void find_2_word_of_main_array(int dut);
void find_1_word_each_sector(int dut);
void find_1_word_each_row(int dut);
void find_1_word_each_word(int dut);
void find_1_bit_each_word();



#define START_DUT_LOOP_EFA(dutgroup)			\
{												\
	DutCursor dutcsr;							\
	UT_DUT    dut; 								\
	dutcsr = UTL_GetDutCursor(dutgroup);		\
	long long int efa_active_dut;							\
	GetActiveDut(&efa_active_dut);				\
	setPinLevelFixh("MASK_PINS");							\
	ExclusionHandle hex = UTL_GetExclusionHandle();		\
	UTL_SetExclusionIgnoreWet  (hex,UT_OFF);		\
	UTL_SetExclusionMask       (hex,UT_OFF);			\
	START_DUT_LOOP(UT_CDUT)							\
	UTL_AddExclusionDut(hex,dut);					\
	END_DUT_LOOP									\
	UTL_SetExclusionSetOrReset (hex,UT_ON);			\
	UTL_SendExclusion(hex);							\
	while ( (dut=UTL_NextDut(dutcsr)) != UT_NOMORE )	\
	{												\
		if(1<<(dut-1)&efa_active_dut){				\
			UTL_ClearExclusionDut(hex);				\
			UTL_AddExclusionDut(hex,dut);			\
			UTL_SetExclusionSetOrReset (hex,UT_OFF);\
			UTL_SendExclusion(hex); 				\
			setPinLevelNone("MASK_PINS",dut);				\
		}else{continue;}

#define END_DUT_LOOP_EFA				\
			setPinLevelFixh("MASK_PINS",dut);							\
			UTL_SetExclusionSetOrReset (hex,UT_ON);\
			UTL_SendExclusion(hex);\
	}										\
	UTL_ClearExclusionDut(hex);				\
	START_DUT_LOOP(UT_MDUT)					\
		if((1<<(dut-1))&efa_active_dut){	\
			setPinLevelNone("MASK_PINS",dut);				\
			UTL_AddExclusionDut(hex,dut);\
	}					\
	END_DUT_LOOP  							\
	UTL_SetExclusionSetOrReset (hex,UT_OFF); \
	UTL_SendExclusion(hex);\
	UTL_DeleteCursor(dutcsr); 				\
	UTL_DeleteHandle(hex);					\
}


#endif
