#include "common.h"
#include "dbmpat/STB_READ_REG.h"

extern unsigned int _binary_STB_READ_REG_dbm_start[];
int DbmLoad()
{
int dbmap=0;
if(FirstRun){dbm_reset();}
dbmap=Setup_Dbm(                  "STB_READ_REG.mpa",                  STB_READ_REG_dbm_bitmode,                  STB_READ_REG_dbm_data,                  STB_READ_REG_dbm_size/sizeof(unsigned int),dbmap);
return 0;
}
