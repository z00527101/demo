#ifndef _FLASH_INFO_H
#define _FLASH_INFO_H


//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1: only for debug
#define  NVR_CFG_ADDR_For_Customer_CP1	 0x807fc0// low 32 bits
#define  NC_Addr_Num	64 //
#define ROW_01_ADDR		0x00200//-- tb_debug_func.c
#define ROW_ADDR		0x00180//-- tb_debug_func.c
#define ONE_ADDR		0xF03  //-- tb_debug_func.c

#define REF_trimcount			32
#define VREF10_trimcount		16	

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2: for flash

#define mode_flash 1
#define mode_logic 2
#define DDUTCNT    4 
#define VREF_trimcount			16
#define JTAG_SEQ   0   //--if USB JTAG sequency ,set 1.
#define FLASH_NUM  4   //-- 1~8  067T
#define DATA_NUM  3   //-- 1~8  067T
//#define FLASH40_64K_72 //-- FLASH40_16K_72, FLASH40_32K_72, FLASH40_64K_72, FLASH40_97K_36, FLASH40_128K_64, FLASH110_16K_64
#define FLASH40_16K_72 //-- FLASH40_16K_72, FLASH40_32K_72, FLASH40_64K_72, FLASH40_97K_36, FLASH40_128K_64, FLASH110_16K_64
#define UBM_ADDR_BITSIZE 24


extern int 	VREF_trim_tmp[DDUTCNT];
//extern double vref10_trim_result[REF_trimcount][FLASH_NUM][DDUTCNT];
extern double vref10_trim_result[REF_trimcount][DDUTCNT];
//extern unsigned int VcodeTmp[DDUTCNT][FLASH_NUM];
extern unsigned int VcodeTmp[DDUTCNT];
//extern unsigned int vref10_value_code[REF_trimcount][FLASH_NUM][DDUTCNT];
extern unsigned int vref10_value_code[REF_trimcount][DDUTCNT];
extern unsigned int VREF10_trim_value[16];//07JP
//-- soc ECC in 72bit high 8bit 
#define NC_ECC_FLAG   0
#define FOLLOW_CUSTOMER_RULE   1
#define IP0_NVR_CUSTOMER_SOC_ADDR  0x700E8
#define IP1_NVR_CUSTOMER_SOC_ADDR  0x700EC
//#define FOLLOW_DEFAULT_RULE   1
//#define NVR_Addr_Num_FLASH 255
#define NVR_Addr_Num_FLASH 261
#define NVR_Addr_Num_FLASH0 196
#define NVR_Addr_Num_FLASH1 131
#define NVR_Addr_Num_FLASH2 131
#define NVR_Addr_Num_FLASH3 128

//-- flash stress power level
#define VDD33_1P2MAX			4.356 V
#define VDD33_MAX				3.63 V
#define VDD33_NOM				3.3 V
#define VDD33_MIN				2.97 V

#define VDD11_1P2MAX			1.452 V
#define VDD11_MAX				1.21 V
#define VDD11_NOM				1.1 V
#define VDD11_MIN				0.99 V

#define VDD09_NOM				0.9 V
#define VDD09_MAX				1.0 V


//-- flash vs pin number
#define vsno_vdd33 1
#define vsno_vdd11 2
//#define vsno_vdd09 3
#define vsno_ef0_vpp  5
#define vsno_ef1_vpp  3
#define vsno_ef2_vpp  6
#define vsno_ef3_vpp  4


//-- flash IO dc group number
#define vi_io 1
#define vo_io 1  
#define vt_io 1


//-- how many frequency  
#define OP_RATE rate_20mhz 
//-- #define OP_RATE rate_15mhz      
//-- #define OP_RATE rate_10mhz 
//#define OP_RATE rate_5mhz 
#define VREF_Trim_Target                 1 V
//#define VREF_Trim_Target                 0.995 V
#define VREF_Trim_error                  0.03 V
#define HV_TRIM_SCAN		0x0
#define HV_INTERNAL		0x0
//define OHM factor    950k/20M OHM  factor
#define MVM_OHM_FACTOR 		1.043

#define VEG_TestMode		0x0
#define VCG_TestMode		0x1
#define VSL_TestMode		0x2
#define GOX_TestMode		0x3
#define ENDU_TestMode		0x4
#define COX_TestMode		0x5

//-- judge select ,if don't judge ,set num to 0x0.
#define GOX_Judge		0x1       
#define ENDU_Judge		0x1
#define COX_Judge		0x1

//--trim mid init position: appropriate num from wafer data  
#define VEG_mid_position	18
#define GOX_mid_position	20
#define ENDU_mid_position	10
#define VCG_mid_position	15
#define COX_mid_position	18
#define VSL_mid_position	12


//-- HV trim parameter
#define VEG_Trim_Target			4.96 V
#define VEG_Measure_Target		5 V
#define VEG_Trim_error			0.06 V
#define VEG_Meas_error			0.12 V
#define GOX_Trim_Target			5.2 V
#define GOX_Trim_error			0.12 V
#define ENDU_Trim_Target		4.2 V
#define ENDU_Trim_error			0.12 V

#define VCG_Trim_Target			4.68 V
#define VCG_Measure_Target	    4.72 V
#define VCG_Trim_error			0.06 V
#define VCG_Meas_error			0.12 V
#define COX_Trim_Target			4.92 V
#define COX_Trim_error			0.06 V

#define VSL_Trim_Target			4.6 V
#define VSL_Trim_error			0.1 V
#define VSL_Meas_error			0.2 V

#define HV_trimcount			24

//-- HV trim parameter
#define VEG_CBD_Addr		0x04
#define VCG_CBD_Addr		0x02
#define VSL_CBD_Addr		0x01

#define VEG_code_shift		1
#define VCG_code_shift		0
#define VSL_code_shift		0

#define VEG_Wait_Time	 25 MS
#define VCG_Wait_Time	 15 MS
#define VSL_Wait_Time	 15 MS


//-- deep program 
#define DPROG_ADDR_1_FLASH12      0x1fb3
#define DPROG_ADDR_2_FLASH12      0x1ff3
#define DPROG_ADDR_1_FLASH3       0x7b3
#define DPROG_ADDR_2_FLASH3       0x7f3
#define DPROG_DATA_1_H      0xff
#define DPROG_DATA_1_L      0xfbfffeff
#define DPROG_DATA_2_H      0xf7
#define DPROG_DATA_2_L      0xfffdffff
#define FLASH_NO_12		0x3
#define FLASH_NO_3		0x4


//--for Read ref trim
//-- #define READ_TRIM_ADDR			0x7B  //-- we can use DPROG_ADDR_1 to replace this
#define NORM_TRIM_ADDR			0x3
#define MARG1_TRIM_ADDR			0x6
#define MARG0_TRIM_ADDR			0x6
#define VREAD_TRIM_ADDR			0x6
#define NORM_TRIM_SHIFT			9
#define MARG1_TRIM_SHIFT		0
#define MARG0_TRIM_SHIFT		10
#define VREAD_TRIM_SHIFT		5


#define AREA_M			0x0
#define AREA_N			0x40
#define AREA_NC			0x80
#define AREA_R0			0x04
#define AREA_R1			0x08
#define AREA_M_R		0x0c 
#define AREA_M_R_N		0x4c 

//--for repair do not care odd/even different,NVR_CFG pattern will write odd/even rows.
#define REPAIR_FLAG_ADDR	0x80fff5
#define R0_ADDR			0x80fff6
#define R1_ADDR			0x80fff7
//#define REPAIR_MASK_L		0xfffffc00
#define REPAIR_MASK_L		0x55aafe00
#define REPAIR_MASK_H		0xffffffff

#define CP1_ID_ADDR                 0x807FC2
#define GOX_ADDR                    0x81FE01
#define COX_ADDR                    0x81FE02
#define ENDU_ADDR                   0x81FE03
#define CP2_ID_ADDR                 0x807FC3
#define CP2_RETEST_ID_ADDR          0x807FC4
#define CP3_ID_ADDR                 0x807FC5
#define CP3_RETEST_ID_ADDR          0x807FC6
//--waferID/xy location address from soc checklist
#define ADD_LOC_XY_L                0x807FC1
#define ADD_LOC_XY_H                0x407FC1
#define ADD_WAFERID_L               0x807FC0
#define ADD_WAFERID_H               0x407FC0
//nc location address for ITERATE_VCODE_L8 
#define ADD_LOC_F_VCODE             0x807FE0
//nc location address for ITERATE_VCODE_L8 
#define ADD_LOC_I_VCODE_L6          0x807FE1//H36:5_4_3 L36:2_1_0
#define ADD_LOC_I_VCODE_M6          0x807FE2//H36:11_10_9 L36:8_7_6
#define ADD_LOC_I_VCODE_H4          0x807FE3//H36:15 L36:14_13_12




//defined by customer 
#define WAFER_LOT_ID_ADDR 0x700E8
#define COORD_WAFER_ID_ADDR 0x700EC
#define CP1_ID_DATA                 0xFFFFC1C1
#define CP1_ID_DATA_REPAIR          0x0
#define CP2_ID_DATA                 0xFFFFC2C2
#define CP2_RETEST_ID_DATA          0xFFFFD2D2
#define CP3_ID_DATA                 0xFFFFC3C3
#define CP3_RETEST_ID_DATA          0xFFFFD3D3


 #ifdef FLASH110_16K_64
 	#define AREA_R_MIN				0x04
 	#define AREA_R_NUM				2
 
 	#define DATA_BITSIZE			64
 	#define PROG_WORD_SHIFT_ADDR	0x1
 	#define READ_WORD_SHIFT_ADDR	0x2
 	#define PROG_DATA_BITSIZE		32
 	#define READ_DATA_BITSIZE		64
 
 	#define ROW_SHIFT_ADDR			0x40
 	#define ROW_SHIFT_BIT 			6
 	#define SECT_SHIFT_ADDR 		0x80
 	#define SECT_SHIFT_BIT 			7
 
 	#define M_SECT_NUM				0x100
 	#define M_ROW_NUM				0x200
 	#define N_SECT_NUM				2
 	#define R_SECT_NUM				2
 	#define ADDR_HL_MASK_BIT			0x1
 
 #elif defined FLASH40_16K_72
 	#define AREA_R_MIN				0x04
 	#define AREA_R_NUM				2
 
 	#define DATA_BITSIZE			72
 	#define PROG_WORD_SHIFT_ADDR	0x1
 	#define READ_WORD_SHIFT_ADDR	0x1
 	#define PROG_DATA_BITSIZE		36
 	#define READ_DATA_BITSIZE		72
 
 	#define ROW_SHIFT_ADDR			0x40
 	#define ROW_SHIFT_BIT 			6
 	#define SECT_SHIFT_ADDR 		0x80
 	#define SECT_SHIFT_BIT 			7
	#define SECT_ADDR_BIT 			7    //    ;sect address bits, 64k:9 , 16k/32k :8
 
 	#define M_SECT_NUM				0x80
 	#define M_ROW_NUM				0x100
 	#define N_SECT_NUM				16
 	#define R_SECT_NUM				2
// 	#define ADDR_HL_MASK_BIT			0x300000
 	#define ADDR_HL_MASK_BIT			0xc00000
	#define N_SECT_HIGH_ADDR_16K			0x2000
	#define N_SECT_HIGH_ADDR_64K			0x8000
	//--- 16k+64k 0x2000 |0x8000
	#define N_SECT_HIGH_ADDR_ALL			0xA000

 #elif defined FLASH40_32K_72
 	#define AREA_R_MIN				0x04
 	#define AREA_R_NUM				2
 
 	#define DATA_BITSIZE			72
 	#define PROG_WORD_SHIFT_ADDR	0x1
 	#define READ_WORD_SHIFT_ADDR	0x1
 	#define PROG_DATA_BITSIZE		36
 	#define READ_DATA_BITSIZE		72
 
 	#define ROW_SHIFT_ADDR			0x40
 	#define ROW_SHIFT_BIT 			6
 	#define SECT_SHIFT_ADDR 		0x80
 	#define SECT_SHIFT_BIT 			7
 
 	#define M_SECT_NUM				0x100
 	#define M_ROW_NUM				0x200
 	#define N_SECT_NUM				16
 	#define R_SECT_NUM				2
 	#define ADDR_HL_MASK_BIT			0x300000
 
 #elif defined FLASH40_64K_72
	#define AREA_R_MIN				0x04
	#define AREA_R_NUM				2
	#define DATA_BITSIZE			72
	#define PROG_WORD_SHIFT_ADDR	0x1
	#define READ_WORD_SHIFT_ADDR	0x1
	#define PROG_DATA_BITSIZE		36
	#define READ_DATA_BITSIZE		72

	#define ROW_SHIFT_ADDR			0x40
	#define ROW_SHIFT_BIT 			6
	#define SECT_SHIFT_ADDR 		0x80
	#define SECT_SHIFT_BIT 			7

	#define M_SECT_NUM				0x200
	#define M_ROW_NUM				0x400
	#define N_SECT_NUM				16
	#define R_SECT_NUM				2
	#define ADDR_HL_MASK_BIT			0xc00000
	#define N_SECT_HIGH_ADDR_16K			0x2000
	#define N_SECT_HIGH_ADDR_64K			0x8000
	//--- 16k+64k 0x2000 |0x8000
	#define N_SECT_HIGH_ADDR_ALL			0xA000

// #elif defined FLASH40_97K_36
// 
// #elif defined FLASH40_128K_64
// 	#define AREA_R_MIN				0x04
// 	#define AREA_R_NUM				2
// 
// 	#define DATA_BITSIZE			64
// 	#define PROG_WORD_SHIFT_ADDR	0x1
// ///---	#define READ_WORD_SHIFT_ADDR	0x1
// 	#define PROG_DATA_BITSIZE		32
// 	#define READ_DATA_BITSIZE		64
// 
// 	#define ROW_SHIFT_ADDR			0x80
// 	#define ROW_SHIFT_BIT 			7
// 	#define SECT_SHIFT_ADDR 		0x200
// ///---	#define SECT_SHIFT_BIT 			9
// 
// 	#define M_SECT_NUM				0x100
// 	#define M_ROW_NUM				0x400
// 	#define N_SECT_NUM				16
// ///---	#define R_SECT_NUM				2
// 
// 	//--for Read ref trim
// 	#define READ_TRIM_ADDR			0x7B
// 	#define NORM_TRIM_ADDR			0x3
// 	#define MARG1_TRIM_ADDR			0x6
// 	#define MARG0_TRIM_ADDR			0x6
// 	#define VREAD_TRIM_ADDR			0x6
// 	#define NORM_TRIM_SHIFT			9
// 	#define MARG1_TRIM_SHIFT		0
// 	#define MARG0_TRIM_SHIFT		10
// 	#define VREAD_TRIM_SHIFT		5
// 	#define ADDR_HL_MASK_BIT			0x300000
// 
 #endif


extern unsigned int VEG_trim_value[24];
extern unsigned int VCG_trim_value[24];
extern unsigned int VSL_trim_value[24];
extern unsigned int REF_trim_value[32];

extern int NVR_FLASH_ADDR[NVR_Addr_Num_FLASH][FLASH_NUM]; 
extern int nvr_data[NVR_Addr_Num_FLASH][FLASH_NUM][DATA_NUM];
extern int NVR_FLASH_ADDR_LOT_COORD[FLASH_NUM];
extern int NVR_FLASH_ADDR_FINAL_VCODE[4];
extern int NVR_FLASH_ADDR_ITERATE_VCODE_H8[4];
extern int NVR_FLASH_ADDR_ITERATE_VCODE_L8[4];
extern int nvr_data_lot_coord[FLASH_NUM][DATA_NUM];
extern int nvr_data_final_vcode[4][3];
extern int nvr_data_iterate_vcode_L8[4][3];
extern int nvr_data_iterate_vcode_H8[4][3];

#endif
