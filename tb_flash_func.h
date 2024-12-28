#ifndef _TB_FLASH_FUNC_H
#define _TB_FLASH_FUNC_H

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 1:set up testblock
void tb_nvr_cfg_read_all_data();
extern int temp10[DDUTCNT][FLASH_NUM];
extern int temp11[DDUTCNT][FLASH_NUM];
extern int temp12[DDUTCNT][FLASH_NUM];
void  ReadNCFlash_repair(unsigned int addr);
void  ReadFlash_repair(unsigned int AREA,unsigned int addr);
void ReadReg_Print();
void tb_mbist_mux_setup();
void tb_write_soc_reg();
void tb_read_stb_reg();
void tb_flash_ip_reset();
void tb_chip_enable();
void tb_write_timing_reg();
void tb_read_timing_reg();
void tb_physical_address();
void tb_logical_address();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 2:trim &measure  testblock

void ReadFlash_SaveFile(char* identifier, int area, int startaddr, int endaddr, int stepaddr);
void tb_chip_erase_veg_trimming();
void tb_program_vcg_trimming();
void tb_program_vsl_trimming();
void tb_deep_program_sector_erase();
void tb_deep_program_sector_read();
void tb_deep_program_for_current_trim();
void tb_normal_read_triming();
void tb_m1_read_triming();
void tb_m0_read_triming();
void tb_vread_triming();
void tb_measure_veg_chip_erase();
void tb_measure_vcg_program();
void tb_measure_vsl_program();
void tb_measure_veg_chip_erase_ht();
void tb_measure_vcg_program_ht();
void tb_measure_vsl_program_ht();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 3:nvr_cfg program and read and reload 

void tb_nvr_cfg_sector_erase();
void tb_nvr_cfg_lock_en_sector_erase();
void tb_nvr_cfg_read_ff();
void tb_nvr_cfg_cycling_x10();
void tb_nvr_cfg_read_00();
void tb_nvr_cfg_program_trim_01();
void tb_nvr_cfg_program_trim();
void tb_nvr_cfg_read_trim_01();
void tb_nvr_cfg_read_trim();
void tb_reload_all_trim_code();
void tb_reload_all_trim_code_CP1();
void tb_nvr_cfg_program_cp1_id();
void tb_nvr_cfg_program_cp2_id();
void tb_nvr_cfg_program_cp3_id();
void tb_nvr_cfg_program_read_s2reteststamp();
void tb_nvr_cfg_program_read_s3reteststamp();
void tb_nvr_cfg_read_cp1_id();
void tb_nvr_cfg_read_cp2_id();
void tb_nvr_cfg_read_cp3_id();
void tb_nvr_cfg_read_cp2_retest_stamp();
void tb_nvr_cfg_read_cp3_retest_stamp();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 4:repair related

//void tb_clear_bist_fail_reg();
//void tb_main_check_error();
//void tb_nvr_cfg_program_repaired_rows();
//void tb_read_repair_addr();
//void tb_main_check_sort2_error();
//void tb_check_sort2_1column_error_main_array();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 5:stress &special

void tb_mrn_me_program_00();
void tb_mrn_mo_program_00();
void tb_mrn_gox_stress_hhi();
void tb_main_2x8rows_program_00();
void tb_main_2x8rows_read_m0_00();
void tb_mrn_me_1word_ff();
void tb_mrn_mo_1word_ff();
void tb_main_read_ckbd_lvctl();
void tb_main_read_ickbd_lvctl();
void tb_main_read_m0_ckbd_cg3p5v();
void tb_rdn_read_m0_ckbd_cg3p5v();
void tb_nvr_read_m0_ckbd_cg3p5v();
void tb_main_read_m0_ickbd_cg3p5v();
void tb_rdn_read_m0_ickbd_cg3p5v();
void tb_nvr_read_m0_ickbd_cg3p5v();
void tb_mrn_cox_stress();
void tb_cox2_stress();
void tb_mrn_sol_ox_stress();
void tb_mrn_rdist_stress();
void tb_mrn_endurance_screen();
void tb_mrn_chip_erase_x50();
void tb_main_1st_row_column_program_00();
void tb_main_1st_row_column_read_00();
void tb_main_1sec_pre_program_00();
void tb_main_1sec_read_00();
void tb_vread1_sector_erase_retry();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 6:normal function 

void tb_mrn_chip_erase();
void tb_main_chip_erase();
void tb_main_sector_erase();
void tb_main_program_ff();
void tb_main_program_ckbd();
void tb_main_program_ickbd();
void tb_main_program_diagonal();
void tb_main_read_ff();
void tb_main_read_ckbd();
void tb_main_read_ickbd();
void tb_main_read_diagonal();
void tb_main_read_m0_ckbd();
void tb_main_read_m0_ickbd();
void tb_main_read_m1_ff();
void tb_main_read_m1_ckbd();
void tb_main_read_m1_ickbd();
void tb_main_read_m1_ht_ff();
void tb_main_read_m1_ht_ckbd();
void tb_main_read_m1_ht_ickbd();
void tb_nvr_sector_erase();
void tb_nvr_program_ff();
void tb_nvr_program_ckbd();
void tb_nvr_program_ickbd();
void tb_nvr_read_ff();
void tb_nvr_read_ckbd();
void tb_nvr_read_ickbd();
void tb_nvr_read_m0_ckbd();
void tb_nvr_read_m0_ickbd();
void tb_nvr_read_m1_ff();
void tb_nvr_read_m1_ckbd();
void tb_nvr_read_m1_ickbd();
void tb_nvr_read_m1_ht_ff();
void tb_nvr_read_m1_ht_ckbd();
void tb_nvr_read_m1_ht_ickbd();
void tb_rdn_sector_erase();
void tb_rdn_program_ff();
void tb_rdn_program_ckbd();
void tb_rdn_program_ickbd();
void tb_rdn_read_ff();
void tb_rdn_read_ckbd();
void tb_rdn_read_ickbd();
void tb_rdn_read_m0_ckbd();
void tb_rdn_read_m0_ickbd();
void tb_rdn_read_m1_ff();
void tb_rdn_read_m1_ckbd();
void tb_rdn_read_m1_ickbd();
void tb_rdn_read_m1_ht_ff();
void tb_rdn_read_m1_ht_ckbd();
void tb_rdn_read_m1_ht_ickbd();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 7:about idd 
void tb_idd_auto_low_power();
void tb_idd_auto_power();
void tb_idd_deep_power_down();
void tb_idd_erase();
void tb_idd_erase_low();
void tb_idd_program();
void tb_idd_program_low();
void tb_idd_read_high();
void tb_idd_standby();

//--*****************************************************************************************************
//--*****************************************************************************************************
//--part 8:others testblock 

#define  NVR_CFG_ADDR_For_Customer_CP1	 0x807fc0// low 32 bits
#define  NC_Addr_Num	64 //
void Read_NVRCFG(int area, int startaddr, int endaddr, int stepaddr,int addr_mask,unsigned int DATA3[][DDUTCNT][FLASH_NUM],unsigned int DATA2[][DDUTCNT][FLASH_NUM],unsigned int DATA1[][DDUTCNT][FLASH_NUM],char *patname="FN_NORL_READ_FCM");
extern int NVR_CFG_BYFLASH_CP1[DDUTCNT][NC_Addr_Num][FLASH_NUM];
void tb_nvr_cfg_dump_NcData();
void tb_nvrx_read_Vcode_ECID_CP3();
void tb_nvrx_program_Vcode_ECID_CP3();
///---void tb_Pre_program_2rows_00();
///---void tb_chip_erase_high_M_R();
///---void tb_program_VCG_trimming_Binary();
///---void tb_set_VSL_trim_Reg();
///---void tb_meo_program00_M_R_N();
///---void tb_nvr_cfg_program_read_CP2_id_high();
///---void tb_nvr_cfg_program_read_CP3_id_high();
///---void tb_nvr_cfg_read_CP3_id_high();
///---void tb_main_FN_read_ff();
///---void tb_nvr_cfg_FN_read_ff();
///---void tb_nvr_cfg_read_s1id();
///---void tb_nvrx_read_Margin0_00();
///---void tb_nvrx_read_00();
///---void tb_program_ickbd_nvrx();
///---void tb_read_Margin0_array_00();
///---void tb_read_Margin1_erows_ff();
///---void tb_read_Margin1_ht_ff();
///---void tb_read_Margin1_orows_ff();
///---void tb_read_Margin1_rdn();
///---void tb_read_array_00();
///---void tb_sect_read_Margin1_ht_array_ickbd();
///---void tb_sect_read_Margin1_ht_array_ckbd();
///---void tb_sect_read_Margin0_array_00();
///---void tb_sect_read_Margin1_ht_array_ff();
///---void tb_sect_read_array_00();
///---void tb_sect_read_array_FF();
///---void tb_sect_read_array_ickbd();
///---void tb_sect_read_array_ckbd();
///---void tb_redundancy_sector_erase_low();
///---void tb_rtst_stress();
///---void tb_main_2x8rows_check_error();
///---void tb_vread1_sector_erase_retry_p2();
///---void tb_vread1_sector_erase_retry_p3();
///---void tb_vread1_sector_erase_retry_p4();



/*int check_new_sect_addr(int fail_num, int fail1_sect_addr , int fail2_sect_addr,int dut);
void Read_triming(int addr, unsigned int *data, int trim[][FLASH_NUM], char *patname);
void tb_chip_erase_VEG_trimming();
void tb_chip_erase_VEG_trimming_VSIM();
void HV_VSIM_Triming_Binary(int testmode);
void tb_chip_erase_VEG_trimming_Binary();
void tb_chip_erase_M_R_N();
void tb_endurance_screen_M_R();
void tb_mrn_chip_erase();
void measure_HV_VSIM(int testmode);
void tb_nvr_cfg_read_CP1_Repaired_id();
void tb_nvr_read_m1_ff();
void tb_nvrx_read_ff_high();
void tb_nvrx_read_ff_low();
void tb_leakage();
void tb_output_leakage();
void tb_program_VCG_trimming();
void tb_program_VCG_trimming_VSIM();
void tb_program_VSL_trimming();
void tb_program_VSL_trimming_VSIM();
void tb_nvr_program_ckbd();
void tb_program_ckbd_nvrx();
void tb_nvr_program_ickbd();
void CalMultitrimvalue(char *trimlabel,double trim_result[][DDUTCNT*FLASH_NUM],int trim[],double target,double error,int trimcount,int dut);
void measure_VSL_program(double hlimit, double lolimit);*/
#endif
