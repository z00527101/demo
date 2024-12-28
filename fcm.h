#ifndef _FCM_H
#define _FCM_H

void ulConfigFcm(char *pinlist, int xbit, int ybit, int transfer_size);
void ulDisableFcm(void);
void ulReadFcm(unsigned int *data, int size, int x_st, int x_sp, int y_st, int y_sp, int dut);
void ulPresetFcm();
void ulPresetFcmAll();
void ulConfigCfm(char *pinlist, int startxbit,int stopxbit,int startybit,int stopybit);
void ulConfigCfm_MMA(char *pinlist, int startxbit,int stopxbit,int startybit,int stopybit);
void ulReadCfm(unsigned int *data, int size, int x_st, int x_sp,int y_st, int y_sp, int dut);
void ulPresetCfm();

#endif
