#ifndef _TB_DEBUG_FUNC_H
#define _TB_DEBUG_FUNC_H
#include "common.h"

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1: variable define
extern unsigned int temp1[DDUTCNT][FLASH_NUM];
extern unsigned int temp2[DDUTCNT][FLASH_NUM];
extern unsigned int temp3[DDUTCNT][FLASH_NUM];
extern int FCM_READ_DATA_BITSIZE;

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2: debug test block
void debug_ReadReg_Print();
void debug_ReadFlash(unsigned int AREA,unsigned int addr);
void debug_ReadFlash_SaveFile(char* identifier, int area, int startaddr, int endaddr, int stepaddr);
void debug_Ref_current();
void debug_cell_current();
void debug_cell_current_one_addr();
void debug_cell_current_one_addr_scan_1();
void debug_cell_current_one_addr_scan_2();
void debug_read_Margin0_ckbd_cg3p5_scan();
void debug_endurance_screen();
void debug_nvrx_program_debug2(); 
void debug_nvrx_program_debug(); 
void debug_program_one_row();
void debug_sector_erase_one();
void tb_nvrx_program_debug2(); 


#endif
