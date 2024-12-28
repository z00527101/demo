#include "common.h"
void SetPdsMemory()	
{
	static PdsMemoryHandle pds_memory_handle = -1;
	if ( pds_memory_handle < 0 )
	{
		pds_memory_handle= UTL_GetPdsMemoryHandle();
		UTL_SetPdsMemoryClearPat  ( pds_memory_handle, UT_SIG_C(31));
		UTL_SetPdsMemoryIncPat    ( pds_memory_handle, UT_SIG_C(30));
		UTL_SetPdsMemoryJumpPat   ( pds_memory_handle, UT_SIG_C(29));
		UTL_SetPdsMemoryJumpAddr  ( pds_memory_handle, 48);
		setPdsMemoryPat( pds_memory_handle,  0, X7	, FL	, FH , FH ); //-- CMD 7
		setPdsMemoryPat( pds_memory_handle,  1, X6	, FL	, FH , FH ); //-- CMD 6
		setPdsMemoryPat( pds_memory_handle,  2, X5	, FL	, FH , FH ); //-- CMD 5
		setPdsMemoryPat( pds_memory_handle,  3, X4	, FL	, FH , FH ); //-- CMD 4
		setPdsMemoryPat( pds_memory_handle,  4, X3	, FL	, FH , FH ); //-- CMD 3
		setPdsMemoryPat( pds_memory_handle,  5, X2	, FL	, FH , FH ); //-- CMD 2
		setPdsMemoryPat( pds_memory_handle,  6, X1	, FL	, FH , FH ); //-- CMD 1
		setPdsMemoryPat( pds_memory_handle,  7, X0	, FL	, FH , FH ); //-- CMD 0
		setPdsMemoryPat( pds_memory_handle,  8, X7	, FL	, FH , FH ); //-- FLASH_NO 7
		setPdsMemoryPat( pds_memory_handle,  9, X6	, FL	, FH , FH ); //-- FLASH_NO 6
		setPdsMemoryPat( pds_memory_handle, 10, X5	, FL	, FH , FH ); //-- FLASH_NO 5
		setPdsMemoryPat( pds_memory_handle, 11, X4	, FL	, FH , FH ); //-- FLASH_NO 4
		setPdsMemoryPat( pds_memory_handle, 12, X3	, FL	, FH , FH ); //-- FLASH_NO 3
		setPdsMemoryPat( pds_memory_handle, 13, X2	, FL	, FH , FH ); //-- FLASH_NO 2
		setPdsMemoryPat( pds_memory_handle, 14, X1	, FL	, FH , FH ); //-- FLASH_NO 1
		setPdsMemoryPat( pds_memory_handle, 15, X0	, FL	, FH , FH ); //-- FLASH_NO 0
		setPdsMemoryPat( pds_memory_handle, 16, Y15	, FL	, FH , FH ); //-- ADR 31
		setPdsMemoryPat( pds_memory_handle, 17, Y14	, FL	, FH , FH ); //-- ADR 30
		setPdsMemoryPat( pds_memory_handle, 18, FL	, FL	, FH , FH ); //-- ADR 29
		setPdsMemoryPat( pds_memory_handle, 19, FL	, FL	, FH , FH ); //-- ADR 28
		setPdsMemoryPat( pds_memory_handle, 20, Y11	, FL	, FH , FH ); //-- ADR 27
		setPdsMemoryPat( pds_memory_handle, 21, Y10	, FL	, FH , FH ); //-- ADR 26
		setPdsMemoryPat( pds_memory_handle, 22, FL	, FL	, FH , FH ); //-- ADR 25
		setPdsMemoryPat( pds_memory_handle, 23, FL	, FL	, FH , FH ); //-- ADR 24
		setPdsMemoryPat( pds_memory_handle, 24, X23	, FL	, FH , FH ); //-- ADR 23
		setPdsMemoryPat( pds_memory_handle, 25, X22	, FL	, FH , FH ); //-- ADR 22
		setPdsMemoryPat( pds_memory_handle, 26, X21	, FL	, FH , FH ); //-- ADR 21
		setPdsMemoryPat( pds_memory_handle, 27, X20	, FL	, FH , FH ); //-- ADR 20
		setPdsMemoryPat( pds_memory_handle, 28, X19	, FL	, FH , FH ); //-- ADR 19
		setPdsMemoryPat( pds_memory_handle, 29, X18	, FL	, FH , FH ); //-- ADR 18
		setPdsMemoryPat( pds_memory_handle, 30, X17	, FL	, FH , FH ); //-- ADR 17
		setPdsMemoryPat( pds_memory_handle, 31, X16	, FL	, FH , FH ); //-- ADR 16 
		setPdsMemoryPat( pds_memory_handle, 32, X15	, FL	, FH , FH ); //-- ADR 15
		setPdsMemoryPat( pds_memory_handle, 33, X14	, FL	, FH , FH ); //-- ADR 14
		setPdsMemoryPat( pds_memory_handle, 34, X13	, FL	, FH , FH ); //-- ADR 13
		setPdsMemoryPat( pds_memory_handle, 35, X12	, FL	, FH , FH ); //-- ADR 12
		setPdsMemoryPat( pds_memory_handle, 36, X11	, FL	, FH , FH ); //-- ADR 11
		setPdsMemoryPat( pds_memory_handle, 37, X10	, FL	, FH , FH ); //-- ADR 10
		setPdsMemoryPat( pds_memory_handle, 38, X9 	, FL	, FH , FH ); //-- ADR 9 
		setPdsMemoryPat( pds_memory_handle, 39, X8 	, FL	, FH , FH ); //-- ADR 8 
		setPdsMemoryPat( pds_memory_handle, 40, X7 	, FL	, FH , FH ); //-- ADR 7 
		setPdsMemoryPat( pds_memory_handle, 41, X6 	, FL	, FH , FH ); //-- ADR 6 
		setPdsMemoryPat( pds_memory_handle, 42, X5 	, FL	, FH , FH ); //-- ADR 5 
		setPdsMemoryPat( pds_memory_handle, 43, X4 	, FL	, FH , FH ); //-- ADR 4 
		setPdsMemoryPat( pds_memory_handle, 44, X3 	, FL	, FH , FH ); //-- ADR 3 
		setPdsMemoryPat( pds_memory_handle, 45, X2 	, FL	, FH , FH ); //-- ADR 2 
		setPdsMemoryPat( pds_memory_handle, 46, X1 	, FL	, FH , FH ); //-- ADR 1 
		setPdsMemoryPat( pds_memory_handle, 47, X0 	, FL	, FH , FH ); //-- ADR 0 
		setPdsMemoryPat( pds_memory_handle, 48, SD17, SD17	, FH , FH ); //-- DAT 71 
		setPdsMemoryPat( pds_memory_handle, 49, SD16, SD16	, FH , FH ); //-- DAT 70 
		setPdsMemoryPat( pds_memory_handle, 50, SD15, SD15	, FH , FH ); //-- DAT 69 
		setPdsMemoryPat( pds_memory_handle, 51, SD14, SD14	, FH , FH ); //-- DAT 68 
		setPdsMemoryPat( pds_memory_handle, 52, SD13, SD13	, FH , FH ); //-- DAT 67 
		setPdsMemoryPat( pds_memory_handle, 53, SD12, SD12	, FH , FH ); //-- DAT 66 
		setPdsMemoryPat( pds_memory_handle, 54, SD11, SD11	, FH , FH ); //-- DAT 65 
		setPdsMemoryPat( pds_memory_handle, 55, SD10, SD10	, FH , FH ); //-- DAT 64 
		setPdsMemoryPat( pds_memory_handle, 56,  SD9,  SD9	, FH , FH ); //-- DAT 63 
		setPdsMemoryPat( pds_memory_handle, 57,  SD8,  SD8	, FH , FH ); //-- DAT 62
		setPdsMemoryPat( pds_memory_handle, 58,  SD7,  SD7	, FH , FH ); //-- DAT 61
		setPdsMemoryPat( pds_memory_handle, 59,  SD6,  SD6	, FH , FH ); //-- DAT 60
		setPdsMemoryPat( pds_memory_handle, 60,  SD5,  SD5	, FH , FH ); //-- DAT 59
		setPdsMemoryPat( pds_memory_handle, 61,  SD4,  SD4	, FH , FH ); //-- DAT 58
		setPdsMemoryPat( pds_memory_handle, 62,  SD3,  SD3	, FH , FH ); //-- DAT 57
		setPdsMemoryPat( pds_memory_handle, 63,  SD2,  SD2	, FH , FH ); //-- DAT 56
		setPdsMemoryPat( pds_memory_handle, 64,  SD1,  SD1	, FH , FH ); //-- DAT 55
		setPdsMemoryPat( pds_memory_handle, 65,  SD0,  SD0	, FH , FH ); //-- DAT 54
		setPdsMemoryPat( pds_memory_handle, 66,  D17,  D17	, FH , FH ); //-- DAT 53
		setPdsMemoryPat( pds_memory_handle, 67,  D16,  D16	, FH , FH ); //-- DAT 52
		setPdsMemoryPat( pds_memory_handle, 68,  D15,  D15	, FH , FH ); //-- DAT 51
		setPdsMemoryPat( pds_memory_handle, 69,  D14,  D14	, FH , FH ); //-- DAT 50
		setPdsMemoryPat( pds_memory_handle, 70,  D13,  D13	, FH , FH ); //-- DAT 49
		setPdsMemoryPat( pds_memory_handle, 71,  D12,  D12	, FH , FH ); //-- DAT 48
		setPdsMemoryPat( pds_memory_handle, 72,  D11,  D11	, FH , FH ); //-- DAT 47
		setPdsMemoryPat( pds_memory_handle, 73,  D10,  D10	, FH , FH ); //-- DAT 46
		setPdsMemoryPat( pds_memory_handle, 74,   D9,   D9	, FH , FH ); //-- DAT 45
		setPdsMemoryPat( pds_memory_handle, 75,   D8,   D8	, FH , FH ); //-- DAT 44
		setPdsMemoryPat( pds_memory_handle, 76,   D7,   D7	, FH , FH ); //-- DAT 43
		setPdsMemoryPat( pds_memory_handle, 77,   D6,   D6	, FH , FH ); //-- DAT 42
		setPdsMemoryPat( pds_memory_handle, 78,   D5,   D5	, FH , FH ); //-- DAT 41
		setPdsMemoryPat( pds_memory_handle, 79,   D4,   D4	, FH , FH ); //-- DAT 40
		setPdsMemoryPat( pds_memory_handle, 80,   D3,   D3	, FH , FH ); //-- DAT 39
		setPdsMemoryPat( pds_memory_handle, 81,   D2,   D2	, FH , FH ); //-- DAT 38
		setPdsMemoryPat( pds_memory_handle, 82,   D1,   D1	, FH , FH ); //-- DAT 37
		setPdsMemoryPat( pds_memory_handle, 83,   D0,   D0	, FH , FH ); //-- DAT 36
		setPdsMemoryPat( pds_memory_handle, 84, SD17, SD17	, FH , FH ); //-- DAT 35
		setPdsMemoryPat( pds_memory_handle, 85, SD16, SD16	, FH , FH ); //-- DAT 34
		setPdsMemoryPat( pds_memory_handle, 86, SD15, SD15	, FH , FH ); //-- DAT 33
		setPdsMemoryPat( pds_memory_handle, 87, SD14, SD14	, FH , FH ); //-- DAT 32
		setPdsMemoryPat( pds_memory_handle, 88, SD13, SD13	, FH , FH ); //-- DAT 31
		setPdsMemoryPat( pds_memory_handle, 89, SD12, SD12	, FH , FH ); //-- DAT 30
		setPdsMemoryPat( pds_memory_handle, 90, SD11, SD11	, FH , FH ); //-- DAT 29
		setPdsMemoryPat( pds_memory_handle, 91, SD10, SD10	, FH , FH ); //-- DAT 28
		setPdsMemoryPat( pds_memory_handle, 92,  SD9,  SD9	, FH , FH ); //-- DAT 27
		setPdsMemoryPat( pds_memory_handle, 93,  SD8,  SD8	, FH , FH ); //-- DAT 26
		setPdsMemoryPat( pds_memory_handle, 94,  SD7,  SD7	, FH , FH ); //-- DAT 25
		setPdsMemoryPat( pds_memory_handle, 95,  SD6,  SD6	, FH , FH ); //-- DAT 24
		setPdsMemoryPat( pds_memory_handle, 96,  SD5,  SD5	, FH , FH ); //-- DAT 23
		setPdsMemoryPat( pds_memory_handle, 97,  SD4,  SD4	, FH , FH ); //-- DAT 22
		setPdsMemoryPat( pds_memory_handle, 98,  SD3,  SD3	, FH , FH ); //-- DAT 21
		setPdsMemoryPat( pds_memory_handle, 99,  SD2,  SD2	, FH , FH ); //-- DAT 20
		setPdsMemoryPat( pds_memory_handle,100,  SD1,  SD1	, FH , FH ); //-- DAT 19
		setPdsMemoryPat( pds_memory_handle,101,  SD0,  SD0	, FH , FH ); //-- DAT 18
		setPdsMemoryPat( pds_memory_handle,102,  D17,  D17	, FH , FH ); //-- DAT 17
		setPdsMemoryPat( pds_memory_handle,103,  D16,  D16	, FH , FH ); //-- DAT 16
		setPdsMemoryPat( pds_memory_handle,104,  D15,  D15	, FH , FH ); //-- DAT 15
		setPdsMemoryPat( pds_memory_handle,105,  D14,  D14	, FH , FH ); //-- DAT 14
		setPdsMemoryPat( pds_memory_handle,106,  D13,  D13	, FH , FH ); //-- DAT 13
		setPdsMemoryPat( pds_memory_handle,107,  D12,  D12	, FH , FH ); //-- DAT 12
		setPdsMemoryPat( pds_memory_handle,108,  D11,  D11	, FH , FH ); //-- DAT 11
		setPdsMemoryPat( pds_memory_handle,109,  D10,  D10	, FH , FH ); //-- DAT 10
		setPdsMemoryPat( pds_memory_handle,110,   D9,   D9	, FH , FH ); //-- DAT 9 
		setPdsMemoryPat( pds_memory_handle,111,   D8,   D8	, FH , FH ); //-- DAT 8 
		setPdsMemoryPat( pds_memory_handle,112,   D7,   D7	, FH , FH ); //-- DAT 7 
		setPdsMemoryPat( pds_memory_handle,113,   D6,   D6	, FH , FH ); //-- DAT 6 
		setPdsMemoryPat( pds_memory_handle,114,   D5,   D5	, FH , FH ); //-- DAT 5 
		setPdsMemoryPat( pds_memory_handle,115,   D4,   D4	, FH , FH ); //-- DAT 4 
		setPdsMemoryPat( pds_memory_handle,116,   D3,   D3	, FH , FH ); //-- DAT 3 
		setPdsMemoryPat( pds_memory_handle,117,   D2,   D2	, FH , FH ); //-- DAT 2 
		setPdsMemoryPat( pds_memory_handle,118,   D1,   D1	, FH , FH ); //-- DAT 1 
		setPdsMemoryPat( pds_memory_handle,119,   D0,   D0	, FH , FH ); //-- DAT 0 
	}
	UTL_SendPdsMemory( pds_memory_handle);
	setPinPds( "SI" ,	UT_PDS_DATAMUX_A        );
	setPinPds( "SO" ,	UT_PDS_DATAMUX_B        );
}
void ClearPdsMemory()
{
	setPinPds( "SI"  ,	UT_PDS_DATAMUX_BYPDSPIN );
	setPinPds( "SO"  ,	UT_PDS_DATAMUX_BYPDSPIN );
}



