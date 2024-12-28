#ifndef _COMMON_FLASH_H
#define _COMMON_FLASH_H
#include "tb_flash_func.h"
//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1: variable define
extern bool  isRepair[DDUTCNT];

enum rate_sel{rate_20mhz=0, rate_15mhz, rate_10mhz, rate_5mhz};
extern int fail_sect_addr1[DDUTCNT][FLASH_NUM];
extern int fail_sect_addr2[DDUTCNT][FLASH_NUM];
extern int fail_sect_count[DDUTCNT][FLASH_NUM];

extern int VEG_trim[DDUTCNT][FLASH_NUM];
extern int VCG_trim[DDUTCNT][FLASH_NUM];
extern int VSL_trim[DDUTCNT][FLASH_NUM];
extern int GOX_trim[DDUTCNT][FLASH_NUM];
extern int ENDU_trim[DDUTCNT][FLASH_NUM];
extern int COX_trim[DDUTCNT][FLASH_NUM];

extern int REF_trim[DDUTCNT][FLASH_NUM];
extern int MRG0_trim[DDUTCNT][FLASH_NUM];
extern int MRG1_trim[DDUTCNT][FLASH_NUM];
extern int VREAD1_trim[DDUTCNT][FLASH_NUM];

extern unsigned int XYh32[DDUTCNT];
extern unsigned int XYl32[DDUTCNT];
extern unsigned int idh32[DDUTCNT];
extern unsigned int idl32[DDUTCNT];
extern unsigned int LOTID_h32[DDUTCNT];
extern unsigned int LOTID_h32_nc_reload[DDUTCNT];
extern unsigned int LOTID_l32_nc_reload[DDUTCNT];
extern unsigned int WAFERID_h32_nc_reload[DDUTCNT];
extern unsigned int WAFERID_l32_nc_reload[DDUTCNT];
extern unsigned int LOTID_l32[DDUTCNT];
extern unsigned int WAFERID_h32[DDUTCNT];
extern unsigned int WAFERID_l32[DDUTCNT];
extern unsigned short  LOTID_ECC[DDUTCNT];
extern unsigned short  WAFERID_XY_ECC[DDUTCNT];

extern int ADD_LOC_ECC[DDUTCNT];
extern int WAFERID_ECC[DDUTCNT];
extern int WAFER_LOT_ID_ECC[DDUTCNT];
extern int COORD_WAFER_ID_ECC[DDUTCNT];



extern int CBD0[DDUTCNT][FLASH_NUM];
extern int CBD1[DDUTCNT][FLASH_NUM];
extern int CBD2[DDUTCNT][FLASH_NUM];
extern int CBD3[DDUTCNT][FLASH_NUM];
extern int CBD4[DDUTCNT][FLASH_NUM];
extern int CBD5[DDUTCNT][FLASH_NUM];
extern int CBD6[DDUTCNT][FLASH_NUM];
extern int CBD7[DDUTCNT][FLASH_NUM];

extern int CBD0_ECC[DDUTCNT][FLASH_NUM];
extern int CBD1_ECC[DDUTCNT][FLASH_NUM];
extern int CBD2_ECC[DDUTCNT][FLASH_NUM];
extern int CBD3_ECC[DDUTCNT][FLASH_NUM];
extern int CBD4_ECC[DDUTCNT][FLASH_NUM];
extern int CBD5_ECC[DDUTCNT][FLASH_NUM];
extern int CBD6_ECC[DDUTCNT][FLASH_NUM];
extern int CBD7_ECC[DDUTCNT][FLASH_NUM];
extern int CP1_ID_DATA_ECC;
extern int CP2_ID_DATA_ECC;
extern int CP3_ID_DATA_ECC;
extern int CP2_RETEST_ID_DATA_ECC;
extern int CP3_RETEST_ID_DATA_ECC;



extern int COX_CBD2[DDUTCNT][FLASH_NUM];
extern int COX_CBD3[DDUTCNT][FLASH_NUM];
extern int GOX_CBD4[DDUTCNT][FLASH_NUM];
extern int GOX_CBD5[DDUTCNT][FLASH_NUM];
extern int ENDU_CBD4[DDUTCNT][FLASH_NUM];
extern int ENDU_CBD5[DDUTCNT][FLASH_NUM];




extern int Column_fail_sec_reg[DDUTCNT][FLASH_NUM][3];
extern int Column_fail_sec_num[DDUTCNT][FLASH_NUM];
extern int fail_sect_num[DDUTCNT][FLASH_NUM];
extern int fail_sect_addr[DDUTCNT][FLASH_NUM][4];
extern int retry_num;




//--*****************************************************************************************************
//--*****************************************************************************************************
///--- void Caltrimvalue(char *trimlabe,double trim_result[][DDUTCNT],int trim[],unsigned int *trim_value, double target,int trimcount);
/*part1*********************** Register value*********************/
void tb_read_fail_addr();
void setRegFlashNo(int flash_no);
void setRegRDNAreaLoop(unsigned area_start, unsigned int area_num, unsigned int addr);
void setRegMultiAddr(unsigned addr_area, unsigned int addr_start, unsigned int addr_stop, unsigned int addr_step);
void setRegAddrNUM(unsigned addr_area,int addnum);
void setRegAddr(unsigned addr_area);
void setRegAddr(unsigned addr_area, unsigned int addr);
void setRegAddr_D1D(unsigned addr_area, unsigned int addr);
void setRegDataIn(unsigned int data3, unsigned int data2, unsigned int data1);
void setRegDataIn_TPHB(unsigned int data3, unsigned int data2, unsigned int data1);
void setRegDataOut(unsigned int data3, unsigned int data2, unsigned int data1);
void setRegDataOut_TPHB(unsigned int data3, unsigned int data2, unsigned int data1);
/*part2*********************** Pattern Exec*********************/
void FlashPatExec(char* patfile, char* startlabel);
void FlashPatStart(char* patfile, char* startlabel);
void FlashPatExec_NoWait(char* patfile, char* startlabel);
void LogicPatExec(char* patfile);
/*part3***********************Trim related*********************/
void Read_triming(int addr, int shift, int trim[][FLASH_NUM], char *patname);
//void Read_triming(int addr, unsigned int *data, int trim[][FLASH_NUM], char *patname);
void Meas_read_trim(int flag[][DDUTCNT][FLASH_NUM],int trim[][FLASH_NUM]);
void Caltrimvalue(char *trimlabel,double trim_result[][DDUTCNT],int trim[],unsigned int* trim_value,double target,int trimcount);
void HV_VSIM_Triming_Binary(int testmode);
void HV_MVM_Triming_Binary(int testmode);
void measure_HV_VSIM(int testmode);
void measure_HV_MVM(int testmode);
void measure_VSL_program(double hlimit, double lolimit);
void HVVSIM_Measure_Voltage_ByFlash(char* item, char*pin,int VSNO, double target,double value[][FLASH_NUM],double vpp_leakage_offset[][FLASH_NUM],int flash_no);
void HV_internal_set(int testmode); 

/*part4*********************** Initial id *********************/
void InitialCpDieId(unsigned  int *XYh32, unsigned  int *XYl32,unsigned int *idh32,unsigned int *idl32);
void InitialCpDieId_jx2(unsigned  int *LOTID_h32, unsigned  int *LOTID_l32,unsigned  int *WaferID_h32,unsigned int *WaferID_l32);
/*part5***********************repair & 1R1C*********************/
void ConfigCFM_Sector();
void ReadCFM_Sector();

void tb_clear_bist_fail_reg();
void tb_main_check_error();
void tb_nvr_cfg_program_repaired_rows();
void tb_read_repair_addr();
void tb_main_check_sort2_error();
void tb_check_sort2_1column_error_main_array();
/*part6*********************cell current***********************/
void tb_cell_current();


/*part7*********************** Data process*********************/
void ReadReg_STB_BFAIL_REG(int select_STB_or_BFAIL);
void ReadFlash(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname="FN_NORL_READ_FCM");
int check_new_sect_addr(int fail_num,int fail1_sect_addr ,int fail2_sect_addr,int dut);




///--- void measure_VEG_VSIM(double hlimit,double lolimit);

#endif
