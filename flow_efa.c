#include "common.h"

void Flow_EFA(){
    printf("########################################START_EFA_diagnose_TEST########################################\n");

//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******set active dut       *************************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
	RUN_TEST("4-1",  read_efa_file,   "read_efa_file",    Dummy,    Dummy,    "END",    "NEXT",    800    );

//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******DC       *******Need to be replaced with the specific content of this project******************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
    RUN_TEST("1-1"    , tb_opens_shorts                              , "opens_shorts"                               , Dummy       , Dummy       , "NEXT"  , "STOP"  , 4 ); 
    RUN_TEST("1-2"    , tb_power_leakage                             , "Power_leakage"                              , Dummy       , Dummy       , "NEXT"  , "STOP"  , 5 ); 
    RUN_TEST("1-3"    , tb_input_leakage                             , "input_leakage"                              , Dummy       , Dummy       , "NEXT"  , "STOP"  , 6 ); 
//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******SOC start and BIST start *******Need to be replaced with the specific content of this project******************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
    RUN_TEST("1-9"    , tb_soc_power_reset                           , "soc_power_reset"                            , Dummy       , Dummy       , "NEXT"  , "STOP"  , 701); 
    RUN_TEST("1-13"   , tb_mbist_mux_setup                           , "mbist_mux_setup"                            , Dummy       , Dummy       , "NEXT"  , "STOP"  , 800); 
    RUN_TEST("1-14"   , tb_write_soc_reg                             , "write_soc_reg"                              , Dummy       , Dummy       , "NEXT"  , "STOP"  , 800);
    RUN_TEST("1-15"   , tb_read_stb_reg                              , "read_stb_reg"                               , Dummy       , Dummy       , "NEXT"  , "STOP"  , 13 ); 
    RUN_TEST("7-2"    , tb_SOC_VREF_Monitor                          , "tb_SOC_VREF_Monitor"                        , Dummy       , Dummy       , "NEXT"  , "STOP"  , 702); 
    RUN_TEST("1-17"   , tb_flash_ip_reset                            , "flash_ip_reset"                             , Dummy       , Dummy       , "NEXT"  , "NEXT"  , 800); 
    RUN_TEST("1-18"   , tb_chip_enable                               , "chip_enable"                                , Dummy       , Dummy       , "NEXT"  , "STOP"  , 800); 
    RUN_TEST("1-19"   , tb_write_timing_reg                          , "write_timing_reg"                           , Dummy       , Dummy       , "NEXT"  , "STOP"  , 800); 
    RUN_TEST("1-20"   , tb_read_timing_reg                           , "read_timing_reg"                            , Dummy       , Dummy       , "NEXT"  , "STOP"  , 14 ); 
//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******reload trim code   ******************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
    RUN_TEST("2-17"   , tb_flash_ip_reset                            , "flash_ip_reset"                             , Dummy       , Dummy       , "NEXT"  , "NEXT"  , 800); 
    RUN_TEST("1-241"  , tb_reload_all_trim_code                      , "reload_all_trim_code"                       , Dummy       , Dummy       , "NEXT"  , "STOP"  , 800); 
//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******force pass read ID   ******************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
    RUN_TEST("3-20"   , tb_nvr_cfg_read_cp3_id                       , "nvr_cfg_read_cp3_id_nom"                    , Dummy       , Dummy       , "NEXT"   ,"NEXT"  , 800); 
    RUN_TEST("3-21"   , tb_nvr_cfg_read_cp2_id                       , "nvr_cfg_read_cp2_id_nom"                    , Dummy       , Dummy       , "NEXT"  , "NEXT"  , 315); 
    RUN_TEST("2-24"   , tb_nvr_cfg_read_cp1_id                       , "nvr_cfg_read_cp1_id"                        , Dummy       , Dummy       , "NEXT"  , "NEXT"  , 215); 
//-- /////////////////////////////////////////////////////////////////////////////////////////////////
//-- //////    *******EFA cell current measure   ******************
//-- //////////////////////////////////////////////////////////////////////////////////////////////////
//--	RUN_TEST("0-2", debug_set_active_dut,          "debug_set_active_dut",        Dummy,    Dummy,    "NEXT",    "STOP",    800    );
	RUN_TEST("4-7", tb_efa_cell_current_40_128Kx64,           "efa_cell_current",            Dummy,    Dummy,    "NEXT",    "NEXT",    800    );

    printf("########################################END_EFA_diagnose_TEST########################################\n");
}

