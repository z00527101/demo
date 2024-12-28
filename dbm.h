#ifndef _MX_DBM_H
#define _MX_DBM_H

void dbm_reset();
void dbmap_set_condition(char* pinlst, long bitmode);
int dbmap_write( char *patName, unsigned int *dbm_data, int depth, int start, int dbm_bitmode);
int Setup_Dbm( char *patName, int dbm_bitmode, unsigned int *dbm_data, int depth, int sta);
void setDbmPointer(char *patname);
#endif
