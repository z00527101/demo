#ifndef		___libModeChange_H
#define		___libModeChange_H
#ifdef		_cplusplus
extern "C" {
#endif	//--	_cplusplus

#include <test.h>
extern	void SendVs_ForceWet( VsHandle h, long vsno);
extern	void SendDc_ForceWet( DcHandle h);
extern	void RelayOnVs_ForceWet( long vsno);
extern	void RelayOffVs_ForceWet( long vsno); 

#ifdef		_cplusplus
};
#endif	//--	_cplusplus
#endif	//--	___ModeChange_H
