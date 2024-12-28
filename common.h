#ifndef _COMMON_H
#define _COMMON_H

#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <regex.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <test.h>
#include <UTHN.h>
#include <UTSC.h>
#include "search.h"
#include "fcm.h"
#include "ubm.h"
#include "dbm.h"
#include "dbmlod.h"
#include "timing.h"
#include "soc_timing.h"
#include "soc_level.h"
#include "soc_pinfmt.h"
#include "pdsmemory.h"
#include "soc_pinlist.h"
#include "flash_info.h"
#include "common_flash.h"
#include "tb_flash_func.h"
#include "tb_soc_func.h"
#include "tb_efa_func.h"
#include "tb_debug_func.h"
#include "flow_cp.h"
#include "flow_efa.h"
#include "flow_debug.h"
#include "libModeChange.h"
//#define FOLLOW_CUSTOMER_RULE
//#define PRIVATE_LOT_ID
#define PROBER_LOT_ID
extern int g_flash_no;
extern int in_flash_loop;          

typedef struct
{
	char pinname[100];
	int pinno;
	char vsname[100];
	int vsno;
} PIN_STRUCT;
//------------------------- variable ---------------------/
extern PIN_STRUCT g_pinlist[64];
extern char strNextSeq[DDUTCNT][100];
extern bool bFlowDutActive[DDUTCNT];
extern int g_die_X[DDUTCNT];
extern int g_die_Y[DDUTCNT];
extern int g_BinNo[DDUTCNT];
extern char *g_BinName[DDUTCNT];
extern int g_Rejected[DDUTCNT];
extern double g_tRjectTitemTime[DDUTCNT];
extern char *DeviceName;
extern char *LotNumber; 
extern char *ProberID;
extern char *WaferId;
extern unsigned int shotcounter;
extern char *WaferSlotNumber;
extern int FK[20];
extern PinHandle PIN_;
extern int FirstRun;
extern char mpa_name[50]; 

enum ePPS_SRNG { R2V, R3V, R4V, R16V };
enum ePPS_MRNG { M40uA, M400uA, M4mA, M40mA, M400mA,M1200mA, M8A };
enum eVPP_SRNG { R8V, R32V };
enum eVPP_MRNG { M8uA, M80uA, M800uA, M8mA, M128mA,M8V, Mm8V, M32V, Mm32V };
enum eDC_VRNG { R20V, R5V, Rm20V };
enum eDC_IRNG { R5uA, R20uA, R200uA, R2mA, R20mA, Rm20mA };

void VsOpenByList(char *pinlist);
//--part1-------------------------- Test Flow-----------------------------------
void InitialVariable();
void TestStartLog();
void TestEndLog();
void FlowPreProcess(char * label);
void RUN_TEST(char * label, void (*func)(), char *tname, void (*pre_exec)(), void (*post_exec)(), char *pass_branch, char * fail_branch , int bin);
void Dummy();
//--part2----------------------- Pin settings ---------------------
void ulCreatePinListWithNumber(char *plst, int type, int num);
void ulCreatePinListWithList(char *plst, int type, ...);
void setVi(double vihigh, double vilow, int vino);
void setVo(double vohigh, double volow, int vono);
void setVt(double vt, int vtno);
void setVload(double vload, int vload_no);
void SetAllPin2OneLevel(char *pinlist, float value);
void setPinLevelFixh(char *pinlist);
void setPinLevelFixh(char *pinlist,int dut);
void setPinLevelNone(char *pinlist);
void setPinLevelNone(char *pinlist,int dut);
void setPinPds( const char *pinlist, int datamux);
void setPdsMemoryPat( PdsMemoryHandle pds_memory_handle, int pds_adr, int mut_a, int mut_b, int mut_c, int mut_d);
//--part3----------------------- DC settings ---------------------
void setDcVSIM( double sval, enum eDC_VRNG srng, enum eDC_IRNG mrng, double pclamp, double mclamp, double hlimit, double llimit, int cnt );
void setDcISVM( double sval, enum eDC_IRNG srng, enum eDC_VRNG mrng, double pclamp, double mclamp, double hlimit, double llimit, int cnt );
//--part4----------------------- PPS&HV settings ---------------------
void setVs(int vsno, double val, enum ePPS_SRNG srng, enum ePPS_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt);
void setHvVSIM(char *pinlist, double val, enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt);
void setHvVSIM(int vsno, double val, enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double pclamp, double mclamp, double upperLimit, double lowerLimit, int cnt);
void setHvVSIM_ByFlashLoop(int vsno, double val[][FLASH_NUM], enum eVPP_SRNG srng, enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt,int flash_no);
void setHvMVM(char *pinlist,enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt);
void setHvMVM(int vsno,enum eVPP_MRNG mrng, double upperLimit, double lowerLimit, int cnt);
void setHvMask(int vsno);
void setHvMask(char *pinlist);
void setHvUnMask(char *pinlist);
void setHvUnMask(int vsno);
//--part5----------------------- DC Exec ---------------------
void ulSetSettlingTime(double t,double t1,double t2);
void DcExec(char* pinname);
//--part6----------------------- Pattern Exec--------------------
void setRegValue(RadioButton reg, USlider value);
void setRegValue2(RadioButton reg, USlider value);	
void AlpgPatExec( char *patfile, char *startlabel);
void AlpgPatStartNowait(char *patfile, char *startlabel);
void AlpgPatStart(char *patfile, char *startlabel);
//--part7-----------------------Test Result ---------------------/
int GetFinalResult(int dut);
int SetFinalResultOnly(int dut, int result);
void TransMeasData(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr, double *data,int dut);
void Read_Meas_Result(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr);	
void Read_Meas_Result2(char *pinlist, char *test_item, double hlimit, double llimit, double unit, char* unitstr);
int ReadMeasData_ByFlash(char *pinlist, double unit,double data[DDUTCNT][FLASH_NUM],int dut,int flash_no);
int ReadMeasData(char *pinlist, double unit,double *data,int dut);
int GetActiveDut(long long int *active_dut);
int ReturnActiveDut(long long int active_dut);
//--part8----------------------- Data process ---------------------/
void getMidValue(double * source, int cnt, double * destination, int dut); 
int compare(const void* a, const void* b);
int ConvertCharToInt(char *lotnumber, int index);
//--part9----------------------- Debug ---------------------/
int ReadFk(int *array);
void Pause2(char *fmt, ...);
void Pause(char *fmt, ...);
void DebugPrintf(char *fmt, ...);
//--part10----------------------- define ---------------------/
#define LOWBITinPATUP 0
#define PASS     0
#define FAIL     1
#define NOTTEST  -1

#define TRUE  1
#define FALSE 0

#define NA    *1e-9
#define UA    *1e-6
#define MA    *1e-3
#define  A    *1.0
#define UV    *1e-6
#define MV    *1e-3
#define  V    *1.0

#define NS    *1e-9
#define US    *1e-6
#define MS    *1e-3
#define  S    *1.0

#define FP_DISABLE	UT_ALPG_FPSEL_DISABLE
#define FP0	UT_ALPG_FPSEL_FP0
#define FP1	UT_ALPG_FPSEL_FP1
#define FP3	UT_ALPG_FPSEL_FP3
#define FP4	UT_ALPG_FPSEL_FP4
#define FP5	UT_ALPG_FPSEL_FP5
#define FP6	UT_ALPG_FPSEL_FP6
#define FP7	UT_ALPG_FPSEL_FP7
#define FP8	UT_ALPG_FPSEL_FP8
#define FP9	UT_ALPG_FPSEL_FP9
#define FP10	UT_ALPG_FPSEL_FP10

#define	X0	(UT_SIG_X(0 ))
#define	X1	(UT_SIG_X(1 ))
#define	X2	(UT_SIG_X(2 ))
#define	X3	(UT_SIG_X(3 ))
#define	X4	(UT_SIG_X(4 ))
#define	X5	(UT_SIG_X(5 ))
#define	X6	(UT_SIG_X(6 ))
#define	X7	(UT_SIG_X(7 ))
#define	X8	(UT_SIG_X(8 ))
#define	X9	(UT_SIG_X(9 ))
#define	X10	(UT_SIG_X(10))
#define	X11	(UT_SIG_X(11))
#define	X12	(UT_SIG_X(12))
#define	X13	(UT_SIG_X(13))
#define	X14	(UT_SIG_X(14))
#define	X15	(UT_SIG_X(15))
#define	X16	(UT_SIG_X(16))
#define	X17	(UT_SIG_X(17))
#define	X18	(UT_SIG_X(18))
#define	X19	(UT_SIG_X(19))
#define	X20	(UT_SIG_X(20))
#define	X21	(UT_SIG_X(21))
#define	X22	(UT_SIG_X(22))
#define	X23	(UT_SIG_X(23))
#define	X24	(UT_SIG_X(24))
#define	X25	(UT_SIG_X(25))
#define	X26	(UT_SIG_X(26))
#define	X27	(UT_SIG_X(27))
#define	X28	(UT_SIG_X(28))
#define	X29	(UT_SIG_X(29))
#define	X30	(UT_SIG_X(30))
#define	X31	(UT_SIG_X(31))
#define	Y0	(UT_SIG_Y(0 ))
#define	Y1	(UT_SIG_Y(1 ))
#define	Y2	(UT_SIG_Y(2 ))
#define	Y3	(UT_SIG_Y(3 ))
#define	Y4	(UT_SIG_Y(4 ))
#define	Y5	(UT_SIG_Y(5 ))
#define	Y6	(UT_SIG_Y(6 ))
#define	Y7	(UT_SIG_Y(7 ))
#define	Y8	(UT_SIG_Y(8 ))
#define	Y9	(UT_SIG_Y(9 ))
#define	Y10	(UT_SIG_Y(10))
#define	Y11	(UT_SIG_Y(11))
#define	Y12	(UT_SIG_Y(12))
#define	Y13	(UT_SIG_Y(13))
#define	Y14	(UT_SIG_Y(14))
#define	Y15	(UT_SIG_Y(15))
#define	Y16	(UT_SIG_Y(16))
#define	Y17	(UT_SIG_Y(17))
#define	Y18	(UT_SIG_Y(18))
#define	Y19	(UT_SIG_Y(19))
#define	Y20	(UT_SIG_Y(20))
#define	Y21	(UT_SIG_Y(21))
#define	Y22	(UT_SIG_Y(22))
#define	Y23	(UT_SIG_Y(23))
#define	Y24	(UT_SIG_Y(24))
#define	Y25	(UT_SIG_Y(25))
#define	Y26	(UT_SIG_Y(26))
#define	Y27	(UT_SIG_Y(27))
#define	Y28	(UT_SIG_Y(28))
#define	Y29	(UT_SIG_Y(29))
#define	Y30	(UT_SIG_Y(30))
#define	Y31	(UT_SIG_Y(31))
#define	C0	(UT_SIG_C(0 ))
#define	C1	(UT_SIG_C(1 ))
#define	C2	(UT_SIG_C(2 ))
#define	C3	(UT_SIG_C(3 ))
#define	C4	(UT_SIG_C(4 ))
#define	C5	(UT_SIG_C(5 ))
#define	C6	(UT_SIG_C(6 ))
#define	C7	(UT_SIG_C(7 ))
#define	C8	(UT_SIG_C(8 ))
#define	C9	(UT_SIG_C(9 ))
#define	C10	(UT_SIG_C(10))
#define	C11	(UT_SIG_C(11))
#define	C12	(UT_SIG_C(12))
#define	C13	(UT_SIG_C(13))
#define	C14	(UT_SIG_C(14))
#define	C15	(UT_SIG_C(15))
#define	C16	(UT_SIG_C(16))
#define	C17	(UT_SIG_C(17))
#define	C18	(UT_SIG_C(18))
#define	C19	(UT_SIG_C(19))
#define	C20	(UT_SIG_C(20))
#define	C21	(UT_SIG_C(21))
#define	C22	(UT_SIG_C(22))
#define	C23	(UT_SIG_C(23))
#define	C24	(UT_SIG_C(24))
#define	C25	(UT_SIG_C(25))
#define	C26	(UT_SIG_C(26))
#define	C27	(UT_SIG_C(27))
#define	C28	(UT_SIG_C(28))
#define	C29	(UT_SIG_C(29))
#define	C30	(UT_SIG_C(30))
#define	C31	(UT_SIG_C(31))
#define	D0	(UT_SIG_D(0 ))
#define	D1	(UT_SIG_D(1 ))
#define	D2	(UT_SIG_D(2 ))
#define	D3	(UT_SIG_D(3 ))
#define	D4	(UT_SIG_D(4 ))
#define	D5	(UT_SIG_D(5 ))
#define	D6	(UT_SIG_D(6 ))
#define	D7	(UT_SIG_D(7 ))
#define	D8	(UT_SIG_D(8 ))
#define	D9	(UT_SIG_D(9 ))
#define	D10	(UT_SIG_D(10))
#define	D11	(UT_SIG_D(11))
#define	D12	(UT_SIG_D(12))
#define	D13	(UT_SIG_D(13))
#define	D14	(UT_SIG_D(14))
#define	D15	(UT_SIG_D(15))
#define	D16	(UT_SIG_D(16))
#define	D17	(UT_SIG_D(17))
#define	D18	(UT_SIG_D(18))
#define	D19	(UT_SIG_D(19))
#define	D20	(UT_SIG_D(20))
#define	D21	(UT_SIG_D(21))
#define	D22	(UT_SIG_D(22))
#define	D23	(UT_SIG_D(23))
#define	D24	(UT_SIG_D(24))
#define	D25	(UT_SIG_D(25))
#define	D26	(UT_SIG_D(26))
#define	D27	(UT_SIG_D(27))
#define	D28	(UT_SIG_D(28))
#define	D29	(UT_SIG_D(29))
#define	D30	(UT_SIG_D(30))
#define	D31	(UT_SIG_D(31))
#define	SD0	(UT_SIG_SD(0 ))
#define	SD1	(UT_SIG_SD(1 ))
#define	SD2	(UT_SIG_SD(2 ))
#define	SD3	(UT_SIG_SD(3 ))
#define	SD4	(UT_SIG_SD(4 ))
#define	SD5	(UT_SIG_SD(5 ))
#define	SD6	(UT_SIG_SD(6 ))
#define	SD7	(UT_SIG_SD(7 ))
#define	SD8	(UT_SIG_SD(8 ))
#define	SD9	(UT_SIG_SD(9 ))
#define	SD10	(UT_SIG_SD(10))
#define	SD11	(UT_SIG_SD(11))
#define	SD12	(UT_SIG_SD(12))
#define	SD13	(UT_SIG_SD(13))
#define	SD14	(UT_SIG_SD(14))
#define	SD15	(UT_SIG_SD(15))
#define	SD16	(UT_SIG_SD(16))
#define	SD17	(UT_SIG_SD(17))
#define	SD18	(UT_SIG_SD(18))
#define	SD19	(UT_SIG_SD(19))
#define	SD20	(UT_SIG_SD(20))
#define	SD21	(UT_SIG_SD(21))
#define	SD22	(UT_SIG_SD(22))
#define	SD23	(UT_SIG_SD(23))
#define	SD24	(UT_SIG_SD(24))
#define	SD25	(UT_SIG_SD(25))
#define	SD26	(UT_SIG_SD(26))
#define	SD27	(UT_SIG_SD(27))
#define	SD28	(UT_SIG_SD(28))
#define	SD29	(UT_SIG_SD(29))
#define	SD30	(UT_SIG_SD(30))
#define	SD31	(UT_SIG_SD(31))
#define	FL	(UT_SIG_FL    )
#define	FH	(UT_SIG_FH    )

#define P(pin,...)                                              \
( {                                                             \
  PIN_ = UTL_GetPinHandle();                                    \
  UTL_InitializePinHandle( PIN_);                               \
  __VA_ARGS__;                                                  \
  UTL_SendPin( PIN_, pin);                                      \
  UTL_DeleteHandle( PIN_);                    \
} )
#define P_PART				UTL_SetPinSendMode	    ( PIN_, UT_PIN_PART   )
#define P_PPAT                          UTL_SetPinDbmPinPattern     ( PIN_, UT_ON         )
#define P_PPAT_IOCTRL(n)                UTL_SetPinDbmPinDrePattern  ( PIN_, UT_ON         ), UTL_AddPinStrbCpeMode(PIN_, n,UT_PIN_PCPE)
#define P_FIXL                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_FIXL   )
#define P_FIXH                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_FIXH	  )
#define P_NRZB                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_NRZB	  )
#define P_INRZB                         UTL_SetPinDrWaveform        ( PIN_, UT_WAV_INRZB  )
#define P_NRZC                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_NRZC	  )
#define P_INRZC                         UTL_SetPinDrWaveform        ( PIN_, UT_WAV_INRZC  )
#define P_RZO                           UTL_SetPinDrWaveform        ( PIN_, UT_WAV_RZO	  )
#define P_IRZO                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IRZO	  )
#define P_RZZ                           UTL_SetPinDrWaveform        ( PIN_, UT_WAV_RZZ	  )
#define P_IRZZ                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IRZZ	  )
#define P_RZX                           UTL_SetPinDrWaveform        ( PIN_, UT_WAV_RZX	  )
#define P_IRZX                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IRZX	  )
#define P_XOR                           UTL_SetPinDrWaveform        ( PIN_, UT_WAV_XOR	  )
#define P_IXOR                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IXOR	  )
#define P_XORBC                         UTL_SetPinDrWaveform        ( PIN_, UT_WAV_XORBC  )
#define P_IXORBC                        UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IXORBC )
#define P_DNRZ                          UTL_SetPinDrWaveform        ( PIN_, UT_WAV_DNRZ	  )
#define P_IDNRZ                         UTL_SetPinDrWaveform        ( PIN_, UT_WAV_IDNRZ  )
#define P_OPEN                          UTL_SetPinOpen              ( PIN_, UT_ON        )
#define P_VSOPEN                        UTL_SetPinVsDisconnect      ( PIN_, UT_ON        )
#define P_HINIT                         UTL_SetPinInit              ( PIN_, UT_PIN_HINIT  )
#define P_LINIT                         UTL_SetPinInit              ( PIN_, UT_PIN_HINIT  )
#define P_BCLK(n)                       UTL_SetPinDrClock           ( PIN_, UT_PIN_BCLK, n)
#define P_CCLK(n)                       UTL_SetPinDrClock           ( PIN_, UT_PIN_CCLK, n)
#define P_PSM                           UTL_SetPinPsm               ( PIN_, UT_ON)
#define P_RDSM                          UTL_SetPinRdsm              ( PIN_, UT_ON)
#define P_EINV                          UTL_SetPinEinv              ( PIN_, UT_ON)
#define P_EINVSTRB(n)			UTL_AddPinEinvStrbNumber	( PIN_, n)
#define P_MATCH                         UTL_SetPinMatchMode         ( PIN_, UT_SIG_MATCH)
#define P_MATCHGROUP                    UTL_SetPinMatchMode         ( PIN_, UT_SIG_MATCHGROUP)
#define P_MPG_SEL1 			UTL_SetPinMatchGroup	    ( PIN_, UT_MPG_SEL1, UT_ON)
#define P_MPG_SEL2			UTL_SetPinMatchGroup	    ( PIN_, UT_MPG_SEL2, UT_ON)
#define P_INH(n)                        UTL_SetPinViNumber          ( PIN_, n), UTL_SetPinHvDr              ( PIN_, UT_ON)
#define P_IOC                           UTL_SetPinIoCtrl            ( PIN_, UT_ON)
#define P_IN(n)                         UTL_SetPinViNumber          ( PIN_, n)
#define P_OUT(n)                        UTL_SetPinVoNumber          ( PIN_, n), UTL_SetPinTerm( PIN_, UT_OFF)
#define P_OUTL(n)                       UTL_SetPinVoNumber          ( PIN_, n), UTL_SetPinTerm( PIN_, UT_ON)
#define P_VT(n)                         UTL_SetPinVtNumber          ( PIN_, n)
#define P_IL(n)                         UTL_SetPinIlNumber          ( PIN_, n), UTL_SetPinPl( PIN_,UT_PIN_PL)
#define P_STRB(s,c)                     UTL_AddPinStrbCpeNumber     ( PIN_, s, c)
#define P_EXPA(s)                       UTL_AddPinStrbChannel       ( PIN_, s, 1)
#define P_EXPB(s)                       UTL_AddPinStrbChannel       ( PIN_, s, 2)
#define P_EXP_CH1_FL(s)                 UTL_AddPinStrbChannel       ( PIN_, s, 1)
#define P_EXP_CH1_FH(s)                 UTL_AddPinStrbChannel       ( PIN_, s, 1)
#define P_EXP_CH2_FL(s)                 UTL_AddPinStrbChannel       ( PIN_, s, 2)
#define P_EXP_CH2_FH(s)                 UTL_AddPinStrbChannel       ( PIN_, s, 2)
#define P_DRDLY(n)                      UTL_SetPinDrDelay           ( PIN_, n)
#define P_DREDLY(n)                     UTL_SetPinDreDelay          ( PIN_, n)
#define P_CPEDLY(n)                     UTL_SetPinCpeDelay          ( PIN_, n)
#define P_EXPDLY(n)                     UTL_SetPinExpDelay          ( PIN_, n)
#define P_DRECLK(n)                     UTL_SetPinDreClock          ( PIN_, n)
#define P_DRE(n)                        UTL_SetPinDreNumber         ( PIN_, n)
#define P_DRENRZ                        UTL_SetPinDreWaveform       ( PIN_, UT_DREWAV_NRZ)
#define P_DRERZ                         UTL_SetPinDreWaveform       ( PIN_, UT_DREWAV_RZ)
#define P_DREDNRZ                       UTL_SetPinDreWaveform       ( PIN_, UT_DREWAV_DNRZ)
#define PDSA(cyp,...)                                           \
( {                                                             \
  void (*PDS)(PinHandle,RadioButton,RadioButton)                \
    = UTL_AddPinPdsA;                                           \
  int    CYP = cyp;                                             \
  __VA_ARGS__;                                                  \
} )
#define PDSB(cyp,...)                                           \
( {                                                             \
  void (*PDS)(PinHandle,RadioButton,RadioButton)                \
    = UTL_AddPinPdsB;                                           \
  int    CYP = cyp;                                             \
  __VA_ARGS__;                                                  \
} )
#define PDSD(cyp,...)                                           \
( {                                                             \
  void (*PDS)(PinHandle,RadioButton,RadioButton)                \
    = UTL_AddPinPdsD;                                           \
  int    CYP = cyp;                                             \
  __VA_ARGS__;                                                  \
} )
#define P_C(n)                          PDS( PIN_, UT_SIG_C   (n), CYP)
#define P_I_C(n)                        PDS( PIN_, UT_SIG_I_C (n), CYP)
#define P_D(n)                          PDS( PIN_, UT_SIG_D   (n), CYP)
#define P_SD(n)                         PDS( PIN_, UT_SIG_SD  (n), CYP)
#define P_I_D(n)                        PDS( PIN_, UT_SIG_I_D (n), CYP)
#define P_I_SD(n)                       PDS( PIN_, UT_SIG_I_SD(n), CYP)
#define P_X(n)                          PDS( PIN_, UT_SIG_X   (n), CYP)
#define P_Y(n)                          PDS( PIN_, UT_SIG_Y   (n), CYP)
#define P_FH                            PDS( PIN_, UT_SIG_FH, CYP)
#define P_FL                            PDS( PIN_, UT_SIG_FL, CYP)
#define P_WT                            PDS( PIN_, UT_SIG_WT, CYP)
#define P_RD                            PDS( PIN_, UT_SIG_RD, CYP)

#define START_FLASH_LOOP \
{ \
        if(in_flash_loop==1) {printf("is in flash loop. please check the code\n");UTL_Stop();}\
        in_flash_loop=1;\
        for(g_flash_no=1; g_flash_no<=FLASH_NUM;g_flash_no++) \
        {\

#define END_FLASH_LOOP \
        } \
        in_flash_loop=0;\
}
#define START_FLASH_LOOP_BY_PINLIST(pinlist) \
{ \
        if(in_flash_loop==1) {printf("is in flash loop. please check the code\n");UTL_Stop();}\
        in_flash_loop=1;\
        PinCursor pincur;\
        UT_PIN vspinno;\
        pincur = UTL_GetPinCursor(pinlist);\
        g_flash_no=1;\
        while((vspinno=UTL_NextPin(pincur))!=UT_NOMORE)\
        {\
                if(g_flash_no>FLASH_NUM){printf("The pin number of % is over FLASH_NUM, please check the code\n", pinlist);UTL_Stop();}\

#define END_FLASH_LOOP_BY_PINLIST \
                g_flash_no++;\
        } \
        UTL_DeleteCursor(pincur);\
        in_flash_loop=0;\
}
#define START_PINLIST_LOOP(pinlist) \
{ \
        PinCursor pincur;\
        UT_PIN pinno;\
        pincur = UTL_GetPinCursor(pinlist);\
        while((pinno=UTL_NextPin(pincur))!=UT_NOMORE)\
        {

#define END_PINLIST_LOOP \
        } \
        UTL_DeleteCursor(pincur);\
}

#define WAIT_SITE_SYNC(sync_name)\
{\
	static int sync_no;\
	int sync_status,sync_code;\
	if((sync_code = UTSC_Flow_TestSiteSync_Wait(sync_name, &sync_status)) !=0){\
		UTSC_Flow_ExitOnError(sync_code);\
	}\
	if(sync_status == UTSC_FLOW_SYNC_INCOMPLETE){\
		fprintf(stderr,"test end site exist.\n");\
		UT_ErrorInfo error_info =UTSC_ErrorInfo_Construct();\
		UTL_RuntimeError(error_info);\
		}\
}


//--T5830 to prevent kelvin alarm w/o dsa
#define DISABLE_KELVIN_ALARM                      	\
{												\
	RteMaskHandle rmh=UTL_GetRteMaskHandle();	\
	UTL_SetRteMaskPpsKelvinAlarm(rmh, UT_ON);	\
	UTL_SendRteMask(rmh);						\

#define ENABLE_KELVIN_ALARM                    \
	UTL_SetRteMaskPpsKelvinAlarm(rmh, UT_OFF);	\
	UTL_SendRteMask(rmh);						\
 	UTL_DeleteHandle(rmh);						\
}

#define DISABLE_DcGuard_ALARM                      	\
{												\
	RteMaskHandle rmh1=UTL_GetRteMaskHandle();	\
	UTL_SetRteMaskDcGuardAlarm(rmh1, UT_ON);	\
	UTL_SendRteMask(rmh1);						\

#define ENABLE_DcGuard_ALARM                    \
	UTL_SetRteMaskDcGuardAlarm(rmh1, UT_OFF);	\
	UTL_SendRteMask(rmh1);						\
 	UTL_DeleteHandle(rmh1);						\
}

#define DISABLE_PpsGuard_ALARM                      	\
{												\
	RteMaskHandle rmh2=UTL_GetRteMaskHandle();	\
	UTL_SetRteMaskPpsGuardAlarm(rmh2, UT_ON);	\
	UTL_SendRteMask(rmh2);						\

#define ENABLE_PpsGuard_ALARM                    \
	UTL_SetRteMaskPpsGuardAlarm(rmh2, UT_OFF);	\
	UTL_SendRteMask(rmh2);						\
 	UTL_DeleteHandle(rmh2);						\
}
#define START_DUT_LOOP(dutgroup)                            \
{                                                           \
	DutCursor dutcsr;                                       \
	UT_DUT    dut;                                          \
	dutcsr = UTL_GetDutCursor(dutgroup);                    \
	while ( (dut=UTL_NextDut(dutcsr)) != UT_NOMORE )        \
	{

#define END_DUT_LOOP                                        \
	}                                                       \
	UTL_DeleteCursor(dutcsr);                               \
}

//-- Duts were divided to serveral group by specified pin based on the socket file define
#define START_GROUP_TEST_BY_VS(vspinno)									\
{																		\
	DutCursor dutcsr;                                                   \
	UT_DUT    dut;                                                      \
	int maxdg,dg;                                                       \
	long long int active_dut_temp;\
	GetActiveDut(&active_dut_temp);\
	maxdg=UTL_GetDctVsDutGroupMaxNumber(vspinno);                       \
	ExclusionHandle hex = UTL_GetExclusionHandle();                     \
	UTL_SetExclusionIgnoreWet  (hex,UT_OFF);                            \
	UTL_SetExclusionMask       (hex,UT_ON);                         \
	START_DUT_LOOP(UT_MDUT)                                             \
		UTL_AddExclusionDut(hex,dut);                                   \
	END_DUT_LOOP                                                        \
	UTL_SetExclusionSetOrReset (hex,UT_ON);                             \
	UTL_SendExclusion(hex);                                             \
	for(dg=0;dg<=maxdg;dg++)                                            \
	{                                                                   \
		int dutcnt=0;\
		UTL_ClearExclusionDut(hex);                                     \
		dutcsr=UTL_GetDctVsDutGroupCursor(vspinno,dg);                  \
		while ( (dut=UTL_NextDut(dutcsr)) != UT_NOMORE )                \
		{                                                               \
			if((1<<(dut-1))&active_dut_temp){\
				dutcnt++;\
				UTL_AddExclusionDut(hex,dut);                               \
			}else{continue;}\
		}                                                               \
		UTL_DeleteCursor(dutcsr);\
		if(dutcnt>0){\
			UTL_SetExclusionSetOrReset (hex,UT_OFF);                        \
			UTL_SendExclusion(hex);                                         \
		}else{continue;}\


#define END_GROUP_TEST_BY_VS											\
		if(dg!=maxdg)                                                   \
		{                                                               \
			UTL_SetExclusionSetOrReset (hex,UT_ON);                     \
			UTL_SendExclusion(hex);                                     \
		}                                                               \
	}                                                                   \
	UTL_ClearExclusionDut(hex);                                     \
	START_DUT_LOOP(UT_MDUT)                                             \
		if((1<<(dut-1))&active_dut_temp){\
			UTL_AddExclusionDut(hex,dut);                                   \
		}else{continue;}\
	END_DUT_LOOP                                                        \
	UTL_SetExclusionSetOrReset (hex,UT_OFF);                            \
	UTL_SendExclusion(hex);                                             \
	UTL_DeleteHandle(hex);                                              \
}
//--1. start goup test by HV-ABCDEF
//--2. mask other group HV channel
//--3. set other group exclusion
#define START_GROUP_TEST_BY_VPP(vspinno)\
{\
	DutCursor dutcsr;                                                   \
	UT_DUT    dut;                                                      \
	int maxdg,dg;                                               \
	long long int active_dut_temp;\
	GetActiveDut(&active_dut_temp);\
	ExclusionHandle hex = UTL_GetExclusionHandle();                     \
	VsMaskHandle hvsmask= UTL_GetVsMaskHandle();                        \
	maxdg=UTL_GetDctVsDutGroupMaxNumber(vspinno);                       \
	UTL_SetExclusionIgnoreWet  (hex,UT_OFF);                            \
	UTL_SetExclusionMask       (hex,UT_OFF);		                    \
	dutcsr=UTL_GetDutCursor(UT_CDUT);					\
	UTL_ClearVsMaskDutVsMask(hvsmask);							\
	while((dut=UTL_NextDut(dutcsr)) != UT_NOMORE) {                  \
		UTL_AddVsMaskDutVsMask(hvsmask,dut,vspinno,UT_ON);			\
		UTL_AddExclusionDut(hex,dut);                               \
	}                                                                \
	UTL_SendVsMask(hvsmask);                                        \
	UTL_SetExclusionSetOrReset (hex,UT_ON);                        \
	UTL_SendExclusion(hex);											\
	UTL_DeleteCursor(dutcsr);										\
	for(dg=0;dg<=maxdg;dg++)                                            \
	{                                                                   \
		int dutcnt=0;\
		UTL_ClearExclusionDut(hex);									\
		UTL_ClearVsMaskDutVsMask(hvsmask);							\
		dutcsr=UTL_GetDctVsDutGroupCursor(vspinno,dg);                  \
		while ( (dut=UTL_NextDut(dutcsr)) != UT_NOMORE )  {				\
			if((1<<(dut-1))&active_dut_temp){\
				dutcnt++;\
				UTL_AddVsMaskDutVsMask(hvsmask,dut,vspinno,UT_OFF);			\
				UTL_AddExclusionDut(hex,dut);								\
			}else{continue;}\
		}\
		UTL_DeleteCursor(dutcsr);\
		if(dutcnt>0){\
			UTL_SendVsMask(hvsmask);                                        \
	    	UTL_SetExclusionSetOrReset (hex,UT_OFF);                        \
	    	UTL_SendExclusion(hex);											\
		}else{continue;}

#define END_GROUP_TEST_BY_VPP(vspinno)								\
		if(dg<maxdg) {													\
			UTL_ClearVsMaskDutVsMask(hvsmask);							\
			UTL_ClearExclusionDut(hex);									\
			dutcsr=UTL_GetDctVsDutGroupCursor(vspinno,dg);              \
			while((dut=UTL_NextDut(dutcsr)) != UT_NOMORE) {             \
				if((1<<(dut-1))&active_dut_temp){\
					dutcnt++;\
					UTL_AddVsMaskDutVsMask(hvsmask,dut,vspinno,UT_ON);		\
					UTL_AddExclusionDut(hex,dut);							\
				}else{continue;}\
			}                                                           \
			UTL_DeleteCursor(dutcsr);                                   \
			UTL_SetExclusionSetOrReset (hex,UT_ON);                     \
			UTL_SendExclusion(hex);										\
			UTL_SendVsMask(hvsmask);									\
		}                                                               \
	}\
	UTL_ClearVsMaskDutVsMask(hvsmask);							\
	UTL_ClearExclusionDut(hex);									\
	dutcsr=UTL_GetDutCursor(UT_MDUT);					\
	while((dut=UTL_NextDut(dutcsr)) != UT_NOMORE) {                  \
		if((1<<(dut-1))&active_dut_temp){\
			UTL_AddVsMaskDutVsMask(hvsmask,dut,vspinno,UT_ON);	\
			UTL_AddExclusionDut(hex,dut);						\
		}\
	}                                                       \
	UTL_DeleteCursor(dutcsr);\
	UTL_SendVsMask(hvsmask);									\
	UTL_SetExclusionSetOrReset (hex,UT_OFF);					\
	UTL_SendExclusion(hex);										\
	UTL_DeleteHandle(hvsmask);											\
	UTL_DeleteHandle(hex);												\
}




#endif
