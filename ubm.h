#ifndef _UBM_H
#define _UBM_H

void ulWriteUbm(char *pinlist, int startaddr, int stopaddr, unsigned int data3, unsigned int data2, unsigned int data1, int dut);
void ulPresetUbm(USlider data);
void ulConfigUbm(char *pinlist, int func_select_cbit, int jump_select_cbit) ;
void ulDisableUbm(void);
//-- push func for flash read/write 
void Ubm_Write_Push(char *pinlist,unsigned int flash_addr, int reset_and_datanum, unsigned int data3, unsigned int data2,unsigned int data1, int dut);
void Ubm_Read_Push(char *pinlist,unsigned int flash_addr, int reset_and_datanum, unsigned int data3, unsigned int data2,unsigned int data1, int dut);
#endif
