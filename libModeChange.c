/*********************************************************************************
 * NOTICE: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF            *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.        *
 * IN NO EVENT SHALL ADVANTEST BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING       *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER           *
 * DEALINGS IN THE SOFTWARE.                                                     *
 *********************************************************************************/
#include "libModeChange.h"
#include <stdio.h>

static	void	InternalError( const char *file, int line, const char *func) {
	fprintf( stderr,"ERROR: Internal Error!(%s:%d:%s)\n",file,line,func);
	UTL_Stop();
}

static	void	VsMVM( VsHandle h, int vtcvsno) {
	double	mrange_max, mrange_min;
	UTL_GetVsMrangeMax ( h, &mrange_max);
	UTL_GetVsMrangeMin ( h, &mrange_min);

	int	mrange = 0;
	if      (mrange_max <=  8.0 && mrange_min >= -0.3 ) mrange = 0x01; //-- M(8V)   = 0x1
	else if (mrange_max <= 32.0 && mrange_min >= -0.3 ) mrange = 0x03; //-- M(32V)  = 0x3
	else if (mrange_max <=  0.3 && mrange_min >= -8.0 ) mrange = 0x81; //-- M(-8V)  = 0x81
	else if (mrange_max <=  0.3 && mrange_min >= -10.0) mrange = 0x83; //-- M(-32V) = 0x83
	else                                                InternalError( __FILE__, __LINE__, __func__);
	UTVTC_VsCond_SourceRange_Set          ( vtcvsno, UT_VSCOND_RANGE_NA);
	UTVTC_VsCond_MeasureRange_Set         ( vtcvsno, mrange);
	UTVTC_VsCond_RadioButtonCond_Value_Set( vtcvsno, UT_VS_RADIOBUTTONCOND_LPFFREQ, UT_DCT_NONE);
}

static	void	VsVSIM( VsHandle h, int vtcvsno) {
	double	srange_max, srange_min;
	UTL_GetVsSrangeMax ( h, &srange_max);
	UTL_GetVsSrangeMin ( h, &srange_min);

	int	srange = 0;
	if      (srange_max <=  8.0 && srange_min >= -8.0  ) srange = 0x01; //--   R(8V)  = 0x1
	else if (srange_max <= 32.0 && srange_min >= -10.0 ) srange = 0x03; //--   R(32V) = 0x3
	else                                                 InternalError( __FILE__, __LINE__, __func__);
	UTVTC_VsCond_SourceRange_Set ( vtcvsno, srange);

	double	mrange_max, mrange_min;
	UTL_GetVsMrangeMax ( h, &mrange_max);
	UTL_GetVsMrangeMin ( h, &mrange_min);

	int	mrange = 0;
	if      (mrange_max <=   8e-6 && mrange_min >=   -8e-6 ) mrange = 0x02; //--   M(8uA)   = 0x2
	else if (mrange_max <=  80e-6 && mrange_min >=  -80e-6 ) mrange = 0x03; //--   M(80uA)  = 0x3
	else if (mrange_max <= 800e-6 && mrange_min >= -800e-6 ) mrange = 0x04; //--   M(800uA) = 0x4
	else if (mrange_max <=   8e-3 && mrange_min >=   -8e-3 ) mrange = 0x05; //--   M(8mA)   = 0x5
	else if (mrange_max <=  90e-3 && mrange_min >=  -80e-3 ) mrange = 0x06; //--   M(128mA) = 0x6
	else                                                     InternalError( __FILE__, __LINE__, __func__);
	UTVTC_VsCond_MeasureRange_Set( vtcvsno, mrange);
	double val;
	if (UTL_GetVsSource  ( h, &val) == 1) UTVTC_VsCond_SliderCond_Value_Set( vtcvsno, UT_VS_COND_SOURCE  , val);
	if (UTL_GetVsPclamp  ( h, &val) == 1) UTVTC_VsCond_SliderCond_Value_Set( vtcvsno, UT_VS_COND_CLAMPPOS, val);
	if (UTL_GetVsMclamp  ( h, &val) == 1) UTVTC_VsCond_SliderCond_Value_Set( vtcvsno, UT_VS_COND_CLAMPNEG, val);
	if (UTL_GetVsSlewRate( h, &val) == 1) UTVTC_VsCond_SliderCond_Value_Set( vtcvsno, UT_VS_COND_SLEWRATE, val);
	UTVTC_VsCond_RadioButtonCond_Value_Set( vtcvsno, UT_VS_RADIOBUTTONCOND_LPFFREQ, UT_DCT_NONE);
}

extern	void SendVs_ForceWet( VsHandle h, long vsno) {
	uint	mode;
	UT_VErrorInfo errinfo;
	UTL_GetVsMode ( h, &mode);

//--	long	vtcvsno;
	int	vtcvsno;
	UTVTC_SocketVsConversion_Vs_ToVtcVs( vsno-1, &vtcvsno);
	if      ( mode == UT_DCT_MVM  )	VsMVM ( h, vtcvsno);
	else if ( mode == UT_DCT_VSIM )	VsVSIM( h, vtcvsno);
	else                            InternalError( __FILE__, __LINE__, __func__);

	UTVTC_VsCond_Mode_Set( vtcvsno, mode);
	if ( UTVTC_VsCond_Check ( vtcvsno, &errinfo) == UT_FALSE) InternalError( __FILE__, __LINE__, __func__);
	UTVTC_VsCond_Send ( vtcvsno);
	UTL_SendVs( h, vsno);
}

extern	void RelayOnVs_ForceWet( long vsno) {
      //--  long    vtcvsno;
        int    vtcvsno;
        UTVTC_SocketVsConversion_Vs_ToVtcVs( vsno-1,&vtcvsno);
	UTVTC_VsRelay_TargetVs_Clear  (UT_VSRELAY_VCON);
	UTVTC_VsRelay_TargetVs_Set    (UT_VSRELAY_VCON, vtcvsno);
	UTVTC_VsRelay_SelectedVs_Write(UT_VSRELAY_VCON, UT_TRUE);
}

extern	void RelayOffVs_ForceWet( long vsno) {
      //--  long    vtcvsno;
        int    vtcvsno;
        UTVTC_SocketVsConversion_Vs_ToVtcVs( vsno-1, &vtcvsno);
	UTVTC_VsRelay_TargetVs_Clear  (UT_VSRELAY_VCON);
	UTVTC_VsRelay_TargetVs_Set    (UT_VSRELAY_VCON, vtcvsno);
	UTVTC_VsRelay_SelectedVs_Write(UT_VSRELAY_VCON, UT_FALSE);
}

static	void	DcISVM( DcHandle h) {
	double	srange_max, srange_min;
	UTL_GetDcSrangeMax ( h, &srange_max);
	UTL_GetDcSrangeMin ( h, &srange_min);
	int	srange = 0;
	if      (srange_max <=  20e-6 && srange_min >=  -20e-6 ) srange = 0x3; //--   R(80uA)  = 0x3
	else if (srange_max <= 200e-6 && srange_min >= -200e-6 ) srange = 0x4; //--   R(800uA) = 0x4
	else if (srange_max <=   2e-3 && srange_min >=   -2e-3 ) srange = 0x5; //--   R(8mA)   = 0x5
	else if (srange_max <=  20e-3 && srange_min >=  -20e-3 ) srange = 0x6; //--   R(128mA) = 0x6
	else                                                     InternalError( __FILE__, __LINE__, __func__);
//--      UTVTC_DcCond_SourceRange_Set          ( UT_DCCHANNEL_DCTYPE_MDC, UT_VSCOND_RANGE_NA);
	UTVTC_DcCond_SourceRange_Set          ( UT_DCCHANNEL_DCTYPE_MDC, srange);

	double	mrange_max, mrange_min;
	int	mrange = 0;
	UTL_GetDcMrangeMax ( h, &mrange_max);
	UTL_GetDcMrangeMin ( h, &mrange_min);
	if      (mrange_max <= 13.0 && mrange_min >= -0.3 ) mrange = 0x01; //-- M(8V)   = 0x1
	else if (mrange_max <=  0.3 && mrange_min >= -1.8 ) mrange = 0x81; //-- M(-8V)  = 0x81
	else                                                InternalError( __FILE__, __LINE__, __func__);

	UTVTC_DcCond_MeasureRange_Set         ( UT_DCCHANNEL_DCTYPE_MDC, mrange);
	double val;
	if ( UTL_GetDcSource  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_SOURCE  , val);
	if ( UTL_GetDcPclamp  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_CLAMPPOS, val);
	if ( UTL_GetDcMclamp  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_CLAMPNEG, val);
	if ( UTL_GetDcSlewRate( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_SLEWRATE, val);
	UTVTC_DcCond_RadioButtonCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_RADIOBUTTONCOND_LPFFREQ, UT_DCT_NONE);
}

static	void	DcVSIM( DcHandle h) {
	double	srange_max, srange_min;
	UTL_GetDcSrangeMax ( h, &srange_max);
	UTL_GetDcSrangeMin ( h, &srange_min);
	int	srange = 0x01;
	if      (srange_max <= 13.0 && srange_min >= -1.0  ) srange = 0x1; //--   R(8V)  = 0x1
	UTVTC_DcCond_SourceRange_Set( UT_DCCHANNEL_DCTYPE_MDC, srange);

	double	mrange_max, mrange_min;
	UTL_GetDcMrangeMax ( h, &mrange_max);
	UTL_GetDcMrangeMin ( h, &mrange_min);
	int	mrange = 0;
	if      (mrange_max <=   5e-6 && mrange_min >=   -5e-6 ) mrange = 0x2; //--   M(8uA)   = 0x2
	else if (mrange_max <=  20e-6 && mrange_min >=  -20e-6 ) mrange = 0x3; //--   M(80uA)  = 0x3
	else if (mrange_max <= 200e-6 && mrange_min >= -200e-6 ) mrange = 0x4; //--   M(800uA) = 0x4
	else if (mrange_max <=   2e-3 && mrange_min >=   -2e-3 ) mrange = 0x5; //--   M(8mA)   = 0x5
	else if (mrange_max <=  20e-3 && mrange_min >=  -20e-3 ) mrange = 0x6; //--   M(128mA) = 0x6
	else                                                     InternalError( __FILE__, __LINE__, __func__);

	UTVTC_DcCond_MeasureRange_Set( UT_DCCHANNEL_DCTYPE_MDC, mrange);
	double val;
	if ( UTL_GetDcSource  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_SOURCE  , val);
	if ( UTL_GetDcPclamp  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_CLAMPPOS, val);
	if ( UTL_GetDcMclamp  ( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_CLAMPNEG, val);
	if ( UTL_GetDcSlewRate( h, &val) == 1) UTVTC_DcCond_SliderCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_COND_SLEWRATE, val);
	UTVTC_DcCond_RadioButtonCond_Value_Set( UT_DCCHANNEL_DCTYPE_MDC, UT_VS_RADIOBUTTONCOND_LPFFREQ, UT_DCT_NONE);
}

extern	void SendDc_ForceWet( DcHandle h) {
	PowerCtrlHandle power_cntrl     = UTL_GetPowerCtrlHandle();
	UTL_ClearPowerCtrlUnit( power_cntrl);
	UTL_AddPowerCtrlDcAll ( power_cntrl);
	UTL_OffPowerCtrl      ( power_cntrl);

	uint	mode;
	UT_VErrorInfo errinfo;
	UTL_GetDcMode ( h, &mode);
	if      ( mode == UT_DCT_ISVM ) DcISVM( h);
	else if ( mode == UT_DCT_VSIM ) DcVSIM( h);
	else                            InternalError( __FILE__, __LINE__, __func__);

	UTVTC_DcCond_Mode_Set                 ( UT_DCCHANNEL_DCTYPE_MDC, mode);
	if ( UTVTC_DcCond_Check( UT_DCCHANNEL_DCTYPE_MDC, &errinfo) == UT_FALSE) InternalError( __FILE__, __LINE__, __func__);
	UTVTC_DcCond_Send ( UT_DCCHANNEL_DCTYPE_MDC);
	UTL_OnPowerCtrl       ( power_cntrl);
	UTL_DeleteHandle      ( power_cntrl);
	UTL_SendDc( h, 1);
}

