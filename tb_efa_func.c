#include "common.h"
#include <math.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <dirent.h>
#include <string.h>    


char *tm_pinlist[FLASH_NUM]={"EF0_TM0", "EF1_TM0"};
#define MAX_LINE_LENGTH 1024		
#define MAX_PATH 1024
char row[MAX_LINE_LENGTH];
int dut_active[DDUTCNT]={0};			//--DDUTCNT.record file dut			
typedef struct efa_file 
{
		int notch;
		int Xcoordinate;
		int Ycoordinate;
		int bin_number;
		int mass_start_word; 
		int mass_end_word;
}efaFile;
efaFile efafile[100];


//--**************************************************************************************************
//-- when there is FLASH_NUM in the initial program or use new structure, please delete it
//#define  FLASH_NUM  1
//int g_flash_no=1;//-- when there is g_flash_no in the initial program or use new structure, please delete it
//--**************************************************************************************************






int read_diagonal[40]={881,55,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_FF[40]={882,34,96,128,136,140,144,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_FF[40]={883,37,46,49,52,56,59,62,65,100,132,148,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_HT_FF[40]={884,226,229,232,247,323,332,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_CKBD[40]={885,68,242,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M0_CKBD[40]={886,69,84,90,93,152,218,243,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_CKBD[40]={887,70,85,99,153,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_HT_CKBD[40]={888,219,244,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_ICKBD[40]={889,103,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M0_ICKBD[40]={890,104,116,122,125,252,318,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_ICKBD[40]={891,105,117,131,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};	
int read_M1_HT_ICKBD[40]={892,253,319,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_M0_CKBD_CG3P5V[40]={893,78,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_M0_ICKBD_CG3P5V[40]={894,113,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_CKBD_LVCTL[40]={895,71,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_ICKBD_LVCTL[40]={896,106,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};
int read_M0_2x8ROWS_00[40]={897,45,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800};




//-- for EFA_40_128Kx64
int find_mode=1;//-- chip=0;sector=1;row=2;word=3
int cover_mode=0;//-- word_add=0;row_add=1
#define max_max_bit 10
double tm0_vol=0.5; //-- for EFA_40_128Kx64
int write_mask_low=0x800000;
int write_mask_high=0x400000;
int io_no=64;
int start_sector=0;
int end_sector=0x1fe00;
int start_row=0;
int end_row=0x1ff80;
int start_word=0;
int end_word=0x1ffff;
int sector_shift_bit=0x200;//-- SECT_SHIFT_ADDR
int row_shift_bit=0x80;//-- ROW_SHIFT_ADDR
int column_shift_bit=0x01;




//--**************************************************************************************************
char* efa_bin_string="";
const char* efa_diagonal="diagonal";
const char* efa_ff="FF";
const char* efa_ckbd="CKBD";
const char* efa_ickbd="ICKBD";
const char* efa_m0="M0";
const char* efa_m1="M1";
const char* efa_ht="HT";
const char* efa_cg3p5v="CG3P5V";
const char* efa_lvctl="LVCTL";
const char* efa_2x8rows="2x8ROWS_00";

//-- initial parameter, don't change
int fail_word_addr[max_max_bit][DDUTCNT][FLASH_NUM];
int measure_word_addr[max_max_bit*5][DDUTCNT][FLASH_NUM];

int repeat_no=0;
int ext_dutrepeat[DDUTCNT]={0};
int ext_bin_no[DDUTCNT]={0};
int ext_mass_start_addr[DDUTCNT][FLASH_NUM][10]={0};//-- 10 is repeat_no,define max is 10.
int ext_mass_end_addr[DDUTCNT][FLASH_NUM][10]={0};//-- 10 is repeat_no,define max is 10.
//--**************************************************************************************************




//-- EFA  void**********************************************************************************************************************************************************
//-- EFA  void**********************************************************************************************************************************************************
//-- EFA  void**********************************************************************************************************************************************************
//-- EFA  void**********************************************************************************************************************************************************
void debug_set_active_dut()
{

//int ext_bin_no[DDUTCNT]={95,190,117,147};
ext_bin_no[0]=78;
ext_bin_no[1]=15;
ext_bin_no[2]=37;
ext_bin_no[3]=77;//--59
ext_bin_no[4]=77;
ext_bin_no[5]=1;


ext_mass_start_addr[3][0][0]=0x03;//-- debug here
ext_mass_end_addr[3][0][0]=0x08;//-- debug here


	//--set_active_dut
	int dut_active[DDUTCNT]={0};			//--DDUTCNT.record file dut			
	dut_active[0]=1;
//--	dut_active[1]=1;
//--	dut_active[2]=1;
//--	dut_active[3]=1;
//--	dut_active[5]=1;


///////-----******************************************************************************
	//-- set result
	START_DUT_LOOP(UT_CDUT)
			if(dut_active[dut-1])
			{
					UTL_ResetFinalResultOnly(dut,UT_RES_ALL);	//--Setting only the final test result to PASS
					printf("dut%d is active dut \n",dut);
			}else{
					UTL_SetFinalResultOnly(dut,UT_RES_ALL);		//--Setting only the final test result to FAI

			}
	END_DUT_LOOP		
} 

void read_efa_file() {    
		DIR *dp=NULL;    
		struct dirent *ent=NULL;    
		char filename[MAX_PATH];    
		char *path="/home/fsdiag/SMIC_EFA/";
		int count=0;
		int file_flag=0;
		int row_flag=0;


		if ((dp = opendir(path)) == NULL) {    
				printf("cannot open directory  0000\n");    
				UTL_Stop();
		}   


		while ((ent = readdir(dp)) != NULL) //--Read files and subdirectories in the file directory 
		{    
				if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {    	
						continue;
				}

				strcpy(filename, ent->d_name);    


				//-- Check if filename contains DS or PDE
				char* efa_pointer = strstr(filename, "EFA_");
				if (efa_pointer == NULL ) {
						//-- contain EFA 
						printf("------------please put right CSV file------------------\n");
						UTL_Stop();
				}
				file_flag=1;		//--judge whether there is a file

				count++;

				if (count>1)		
				{
						printf("------------please put only one CSV file------------------\n");
						UTL_Stop();
				}




				printf("--------------------------path:%s\t\t\t\tfilename:%s-----------------------\n", path,filename);    



				char path_filename[1024];
				sprintf(path_filename,"%s%s",path,filename);


				FILE *csv_file = fopen(path_filename,"r");
				printf("-----------------the path_filename is:%s------------------------\n", path_filename);

				if(csv_file==NULL){		//--if no file,the program will stop while loop
						printf("cannot open CSV File\n");
						UTL_Stop();
				}

				//----------------------read file row------------------------------------------						
				int i=0;
				int j=0;
				char *str=NULL;
				int file_header=1;

				char *header_array[100]={NULL};
				char *search_efa="X coordinate,Y coordinate,bin number,mass start word,mass end word";


				//--if(fgets(row, sizeof(row), csv_file)!=NULL) {}
			
				while(fgets(row, sizeof(row), csv_file)!=NULL) 
				{

						printf("file line is:%s\n", row);

						//------------------judge header-----------------------------------------
						if(file_header)
						{
								if((strstr(row,search_efa))==NULL)
								{
										printf("----error EFA file header---\n");
										UTL_Stop(); 
								}

								file_header=0;
								continue;
						}
						row_flag=1;		//--judge whether there is a row

						sscanf(row,"%d,%d,%d,%x,%x",&efafile[i].Xcoordinate,&efafile[i].Ycoordinate,&efafile[i].bin_number,&efafile[i].mass_start_word,&efafile[i].mass_end_word);
						printf("%d\t%d\t%d\t0x%x\t0x%x\n",efafile[i].Xcoordinate,efafile[i].Ycoordinate,efafile[i].bin_number,efafile[i].mass_start_word,efafile[i].mass_end_word);

						i++;
				}
				fclose(csv_file);
				printf("----------------the total number of lines in the CSV file is:%d----------------\n",i);

				//-------------------compare g_die_X[dut-1], g_die_Y[dut-1]--------------------

				int repeat_flag=0;


			START_DUT_LOOP(UT_CDUT)
						//--START_FLASH_LOOP
				int array_no=0;
				//--g_die_X[0]=47;g_die_Y[0]=28;
				//--g_die_X[1]=55;g_die_Y[1]=28;
				//--g_die_X[2]=39;g_die_Y[2]=31;
				//--g_die_X[3]=55;g_die_Y[3]=57;
				printf((char *)"[D%02d] (%d,%d)\n",dut, g_die_X[dut-1], g_die_Y[dut-1]);		
				for(j=0;j<i;j++)
				{
						if(g_die_X[dut-1]==efafile[j].Xcoordinate &&g_die_Y[dut-1] == efafile[j].Ycoordinate)
						{	
								if(efafile[j].bin_number==1)	
								{

										dut_active[dut-1]=1;			//--record active dut										

										ext_bin_no[dut-1]=efafile[j].bin_number;
									START_FLASH_LOOP
										ext_mass_start_addr[dut-1][g_flash_no-1][array_no] = efafile[j].mass_start_word;	//next row will cover previous row,need confirm
										ext_mass_end_addr[dut-1][g_flash_no-1][array_no] = efafile[j].mass_end_word;
									END_FLASH_LOOP
										//--printf("-----------efafile[%d].bin_number:%d",j,efafile[j].bin_number);
										//--printf("-----------ext_bin_no[%d]:%d",dut-1,ext_bin_no[dut-1]);
										//--printf("-----------ext_mass_start_addr[%d][%d][%d]:0x%x",dut-1,g_flash_no-1,array_no,ext_mass_start_addr[dut-1][g_flash_no-1][array_no]);


										array_no=array_no+1;										
										ext_dutrepeat[dut-1]=ext_dutrepeat[dut-1]+1;
										//--printf("---------------ext_dutrepeat[%d]:%d------\n",dut,ext_dutrepeat[dut-1]);
								}else
								{
										dut_active[dut-1]=1;			//--record active dut										
										//--printf("dut%d ext_dutrepeat[%d]:%d\n",dut,dut,ext_dutrepeat[dut-1]);	

										ext_dutrepeat[dut-1]=1;
										ext_bin_no[dut-1]=efafile[j].bin_number;
										ext_mass_start_addr[dut-1][g_flash_no-1][0] = efafile[j].mass_start_word;	//--next row will cover previous row,need confirm
										ext_mass_end_addr[dut-1][g_flash_no-1][0] = efafile[j].mass_end_word;


								}
						}
				}
				ext_dutrepeat[dut-1]=ext_dutrepeat[dut-1]-1;

				//--END_FLASH_LOOP
			END_DUT_LOOP


		}   
		closedir(dp); 

		if(!file_flag)		//--if no file in /home/SMIC_EFA, stop.
		{
				printf("-----please put a EFA file --------\n");			

				UTL_Stop();
		}
		if(!row_flag)		//--if no file in /home/SMIC_EFA, stop:
		{
				printf("-----please check file row --------\n");			
				UTL_Stop();
		}

		//--judge dut_active
		START_DUT_LOOP(UT_CDUT)
				if(!dut_active[dut-1])
				{

						printf("----------the dut[D%02d] not in csv file, reject----------\n",dut);
						UTL_ResetFinalResultOnly(dut,UT_RES_ALL);	//--Setting only the final test result to PASS
				}else{
						printf("----------the dut[D%02d] is in csv file, continue to execute downwards----------\n",dut);
						UTL_SetFinalResultOnly(dut,UT_RES_ALL);		//--Setting only the final test result to FAIL

				}

		END_DUT_LOOP		

}


void tb_efa_cell_current_40_128Kx64()
{

//-- set the initial number -1 to fail_word_addr and measure_word_addr
for(int ii=0;ii<max_max_bit;ii++){for(int jj=0;jj<DDUTCNT;jj++){for(int kk=0;kk<FLASH_NUM;kk++){fail_word_addr[ii][jj][kk]=-1;}}}
for(int ii=0;ii<max_max_bit*5;ii++){for(int jj=0;jj<DDUTCNT;jj++){for(int kk=0;kk<FLASH_NUM;kk++){measure_word_addr[ii][jj][kk]=-1;}}}

//-- ******************************************************************************************************
START_DUT_LOOP_EFA(UT_CDUT)
printf("dut:%d,start EFA cell current\n",dut);
for(repeat_no=0;repeat_no<=ext_dutrepeat[dut-1];repeat_no++){
START_FLASH_LOOP
printf("flash:%d,start EFA cell current\n",g_flash_no);
//-- ******************************************************************************************************
	printf("\n\ndut%d ext_bin_no[%d]:%d\n",dut,dut,ext_bin_no[dut-1]);	
	printf("dut%d ext_dutrepeat[%d]:%d\n",dut,dut,ext_dutrepeat[dut-1]);	
	printf("dut%d ext_mass_start_addr[%d][%d][%d]:0x%x\n",dut,dut,g_flash_no,repeat_no,ext_mass_start_addr[dut-1][g_flash_no-1][repeat_no]);
	printf("dut%d ext_mass_end_addr[%d][%d][%d]:0x%x\n",dut,dut,g_flash_no,repeat_no,ext_mass_end_addr[dut-1][g_flash_no-1][repeat_no]);	

//-- start 

	//-- find the pattern will read by bin number
	if(	query(read_diagonal,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_diagonal";   	printf("dut%d bin_number = %d    bin_type = read_diagonal	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_FF,		ext_bin_no[dut-1])!=-1){efa_bin_string="read_FF";	       	printf("dut%d bin_number = %d    bin_type = read_FF		\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_FF,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_FF";      	printf("dut%d bin_number = %d    bin_type = read_M1_FF		\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_HT_FF,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_HT_FF";   	printf("dut%d bin_number = %d    bin_type = read_M1_HT_FF	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_CKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_CKBD";		printf("dut%d bin_number = %d    bin_type = read_CKBD		\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M0_CKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M0_CKBD";    	printf("dut%d bin_number = %d    bin_type = read_M0_CKBD	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_CKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_CKBD";		printf("dut%d bin_number = %d    bin_type = read_M1_CKBD    	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_HT_CKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_HT_CKBD";	printf("dut%d bin_number = %d    bin_type = read_M1_HT_CKBD 	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_ICKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_ICKBD";		printf("dut%d bin_number = %d    bin_type = read_ICKBD      	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M0_ICKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M0_ICKBD";		printf("dut%d bin_number = %d    bin_type = read_M0_ICKBD   	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_ICKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_ICKBD";		printf("dut%d bin_number = %d    bin_type = read_M1_ICKBD   	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M1_HT_ICKBD,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M1_HT_ICKBD";	printf("dut%d bin_number = %d    bin_type = read_M1_HT_ICKBD	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M0_CKBD_CG3P5V,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M0_CKBD_CG3P5V";	printf("dut%d bin_number = %d    bin_type = read_M0_CKBD_CG3P5V	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M0_ICKBD_CG3P5V,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M0_ICKBD_CG3P5V";	printf("dut%d bin_number = %d    bin_type = read_M0_ICKBD_CG3P5V\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_CKBD_LVCTL,		ext_bin_no[dut-1])!=-1){efa_bin_string="read_CKBD_LVCTL";	printf("dut%d bin_number = %d    bin_type = read_CKBD_LVCTL	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_ICKBD_LVCTL,		ext_bin_no[dut-1])!=-1){efa_bin_string="read_ICKBD_LVCTL";	printf("dut%d bin_number = %d    bin_type = read_ICKBD_LVCTL	\n",dut,ext_bin_no[dut-1]);}
	else if(query(read_M0_2x8ROWS_00,	ext_bin_no[dut-1])!=-1){efa_bin_string="read_M0_2x8ROWS_00";	printf("dut%d bin_number = %d    bin_type = read_M0_2x8ROWS_00	\n",dut,ext_bin_no[dut-1]);}
	else {if(ext_bin_no[dut-1]!=1) printf("\n This bin is not in the binlist!!! !!! !!! \n\n");else printf("\n This is bin1, will measure mass addr cell current!!!\n\n");}

//-- if not bin1 , then to find the fail bit
if(ext_bin_no[dut-1]!=1)
{
//-- determine physical address or logic address
	if(!strstr(efa_bin_string,efa_diagonal)){	FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");	printf("physical address\n");}
	else{												printf("logic address\n");}

//-- determine read test mode
	if(strstr(efa_bin_string,efa_m0)&& (!strstr(efa_bin_string,efa_cg3p5v))){		FlashPatExec("patFlash.mpa", "MARGIN0");	printf("MARGIN0\n");}
	else if(strstr(efa_bin_string,efa_m1) && (!strstr(efa_bin_string,efa_ht))){	FlashPatExec("patFlash.mpa", "MARGIN1");	printf("MARGIN1\n");}
	else if(strstr(efa_bin_string,efa_m1) && strstr(efa_bin_string,efa_ht)){	FlashPatExec("patFlash.mpa", "MARGIN1_HT");	printf("MARGIN1_HT\n");}
	else if(strstr(efa_bin_string,efa_cg3p5v)){	
			setHvUnMask("VPP0_PINS");
			setHvVSIM("VPP0_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
			printf("set external tester HV\n");
			FlashPatExec("patFlash.mpa", "MARGIN0_CG3P5V");	printf("MARGIN0_CG3P5V\n");}
	else if(strstr(efa_bin_string,efa_lvctl)){					FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_01");	printf("LVCTL_01\n");}

//-- find fail bit
        ulConfigFcm("SO", 0, 6, 8192);
        ulPresetFcm(); 


	if(find_mode==0 || strstr(efa_bin_string,efa_diagonal)){	printf("find_2_word_of_main_array\n");find_2_word_of_main_array(dut);}
	else if(find_mode==1 && (!strstr(efa_bin_string,efa_diagonal))){	printf("find_1_word_each_sector\n");find_1_word_each_sector(dut);}
	else if(find_mode==2 && (!strstr(efa_bin_string,efa_diagonal))){	printf("find_1_word_each_row\n");find_1_word_each_row(dut);}
	else if(find_mode==3 && (!strstr(efa_bin_string,efa_diagonal))){	printf("find_1_word_each_word\n");find_1_word_each_word(dut);}
	else if(find_mode==4 && (!strstr(efa_bin_string,efa_diagonal))){	printf("find_1_bit_each_word\n");find_1_bit_each_word();}

//-- exit read test mode whether or not have entered test mode 
	FlashPatExec("patFlash.mpa", "TMODE_EXIT");
	if(strstr(efa_bin_string,efa_cg3p5v)){
			setHvVSIM("VPP0_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
            		setHvMask("VPP0_PINS"); 
			printf("close external tester HV\n");
			tb_flash_ip_reset();	printf("tb_flash_ip_reset();\n");	//--maybe some product can't exit M1 mode, need this step
			FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_00");	printf("logic address\n");	
			tb_reload_all_trim_code();	printf("tb_reload_all_trim_code();\n");	//--maybe some product can't exit M1 mode, need this step
			if(!strstr(efa_bin_string,efa_diagonal)){	FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");	printf("physical address\n");}
			else{												printf("logic address\n");}
			}
	if(strstr(efa_bin_string,efa_lvctl)){	FlashPatExec("patFlash.mpa", "FLASH_SH_LVCTL_00");	printf("LVCTL_00\n");}
}

//-- measure cell current
//	measure_cell_current_40_128Kx64(dut);
	measure_cell_current_modeonce_40_128Kx64(dut);

//-- end
//-- ******************************************************************************************************
	printf("flash:%d end EFA cell current\n",g_flash_no);		
END_FLASH_LOOP
}
	printf("\ndut%d end EFA cell current\n\n",dut);		
END_DUT_LOOP_EFA
//-- ******************************************************************************************************
} 
//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************
int query(int *bin_lab,int ext_bin_no)
{
	int exist=-1;
	for(int i=0;i<40;i++)
	{
		if(bin_lab[i]==ext_bin_no){exist=i;break;}
	}
	printf(" in query match bin number, exist is %d \n",exist);
	return exist;
}
void find_2_word_of_main_array(int dut)
{
        ulConfigFcm("SO", 0, 6, 8192);
        ulPresetFcm();
 
	setRegAddr(AREA_M,0);
    	if((strstr(efa_bin_string,efa_diagonal) || strstr(efa_bin_string,efa_ckbd)) && (!strstr(efa_bin_string,efa_ickbd))){    setRegDataIn(0x0,0x0,0x0);		 }
    	else if(strstr(efa_bin_string,efa_ff) || strstr(efa_bin_string,efa_ickbd)){ setRegDataIn(0xff,0xffffffff,0xffffffff);}

	if(strstr(efa_bin_string,efa_diagonal)){				printf("EFA_BI_CREAD_DIAG_ALL\n");FlashPatExec("patFlash.mpa", "EFA_BI_CREAD_DIAG_ALL");	}
	else if(strstr(efa_bin_string,efa_ff)){					printf("EFA_BI_CSLRD_NORL_ALL\n");FlashPatExec("patFlash.mpa", "EFA_BI_CSLRD_NORL_ALL");	}
	else if(strstr(efa_bin_string,efa_ckbd) || strstr(efa_bin_string,efa_ickbd)){	printf("EFA_BI_CSLRD_CKBD_ALL\n");FlashPatExec("patFlash.mpa", "EFA_BI_CSLRD_CKBD_ALL");	}

	//--//--//--//--send addr to measure_word_addr
	unsigned int data[DDUTCNT][48];
	int bist_fail3=0, bist_fail2=0, bist_fail1=0;
	ulReadFcm(data[dut-1], 48, 0, 0, 0, 47, dut);
	bist_fail3=data[dut-1][5];
	bist_fail2=data[dut-1][26];
	bist_fail1=data[dut-1][47];
	if(bist_fail1==1){
			fail_word_addr[0][dut-1][g_flash_no-1]=0x0;
	        for(int ii=20;ii>=1;ii--){
			fail_word_addr[0][dut-1][g_flash_no-1] |= data[dut-1][47-ii]<<(ii-1);
	        }                
	}                        
	if(bist_fail2==1){
			fail_word_addr[1][dut-1][g_flash_no-1]=0x0;
	        for(int ii=41;ii>=22;ii--){
	                fail_word_addr[1][dut-1][g_flash_no-1] |= data[dut-1][47-ii]<<(ii-22);
	        }     
	}
	if(bist_fail1)  printf("fail1 wordaddr:0x%x\n",fail_word_addr[0][dut-1][g_flash_no-1]);
	if(bist_fail2)  printf("fail2 wordaddr:0x%x\n",fail_word_addr[1][dut-1][g_flash_no-1]);
	if(bist_fail3)  printf("fail bist3:%d\n",bist_fail3);
					
	if((bist_fail1==1)&&(bist_fail2==1)){
	        measure_word_addr[0][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1];
	        measure_word_addr[1][dut-1][g_flash_no-1]=fail_word_addr[1][dut-1][g_flash_no-1];
	        measure_word_addr[max_max_bit*1][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]+column_shift_bit;
	        measure_word_addr[max_max_bit*2][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]-column_shift_bit;
	        measure_word_addr[max_max_bit*3][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]+row_shift_bit;
	        measure_word_addr[max_max_bit*4][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]-row_shift_bit;
	        measure_word_addr[max_max_bit*1+1][dut-1][g_flash_no-1]=fail_word_addr[1][dut-1][g_flash_no-1]+column_shift_bit;
	        measure_word_addr[max_max_bit*2+1][dut-1][g_flash_no-1]=fail_word_addr[1][dut-1][g_flash_no-1]-column_shift_bit;
	        measure_word_addr[max_max_bit*3+1][dut-1][g_flash_no-1]=fail_word_addr[1][dut-1][g_flash_no-1]+row_shift_bit;
	        measure_word_addr[max_max_bit*4+1][dut-1][g_flash_no-1]=fail_word_addr[1][dut-1][g_flash_no-1]-row_shift_bit;
	}
	if((bist_fail1==1)&&(bist_fail2==0)){
	        measure_word_addr[0][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1];
	        measure_word_addr[max_max_bit*1][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]+column_shift_bit;
	        measure_word_addr[max_max_bit*2][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]-column_shift_bit;
	        measure_word_addr[max_max_bit*3][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]+row_shift_bit;
	        measure_word_addr[max_max_bit*4][dut-1][g_flash_no-1]=fail_word_addr[0][dut-1][g_flash_no-1]-row_shift_bit;
	}
    if(fail_word_addr[0][dut-1][g_flash_no-1]==-1){printf("\n NO FAIL WORD FOUND!!! !!! !!!\n\n");}
}
void find_1_word_each_sector(int dut)
{
	int measure_no_flag=0;
	ulConfigFcm("SO", 0, 6, 8192);

	for(int sector=start_sector;sector<=end_sector;sector=sector+sector_shift_bit){
		if((measure_no_flag>=max_max_bit)){printf("measure_no_flag is enough.\n");break;}                     
		if(strstr(efa_bin_string,efa_2x8rows)&&(sector<end_sector-1*sector_shift_bit)&&(sector>1*sector_shift_bit)){continue;}
                
	        ulPresetFcm(); 
		setRegAddr(AREA_M,sector);
	    	if((strstr(efa_bin_string,efa_diagonal) || strstr(efa_bin_string,efa_ckbd)) ||strstr(efa_bin_string,efa_2x8rows)&& (!strstr(efa_bin_string,efa_ickbd))){    setRegDataIn(0x0,0x0,0x0);		 }
	    	else if(strstr(efa_bin_string,efa_ff) || strstr(efa_bin_string,efa_ickbd)){ setRegDataIn(0xff,0xffffffff,0xffffffff);}
	
		if(strstr(efa_bin_string,efa_ff)||strstr(efa_bin_string,efa_2x8rows)){  FlashPatExec("patFlash.mpa", "EFA_BI_SSLRD_NORL");	}
		else if(strstr(efa_bin_string,efa_ckbd) || strstr(efa_bin_string,efa_ickbd)){	FlashPatExec("patFlash.mpa", "EFA_BI_SSLRD_CKBD");	}
	
		//--//--//--//--send addr to measure_word_addr
		unsigned int data[DDUTCNT][48];
		int bist_fail3=0, bist_fail2=0, bist_fail1=0;
		ulReadFcm(data[dut-1], 48, 0, 0, 0, 47, dut);
		bist_fail3=data[dut-1][5];
		bist_fail2=data[dut-1][26];
		bist_fail1=data[dut-1][47];
		if(bist_fail1==1){
			fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]=0x0;
		        for(int ii=20;ii>=1;ii--){
		                fail_word_addr[measure_no_flag][dut-1][g_flash_no-1] |= data[dut-1][47-ii]<<(ii-1);
		        }                
            		printf("sector=0x%x\t",sector);
    			if(strstr(efa_bin_string,efa_ff)){					printf("EFA_BI_SSLRD_NORL\t");	}
    			else if(strstr(efa_bin_string,efa_ckbd) || strstr(efa_bin_string,efa_ickbd)){	printf("EFA_BI_SSLRD_CKBD\t");	}
			printf("fail1 wordaddr[%d][%d][%d]:0x%x\n",measure_no_flag,dut,g_flash_no,fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]);
			measure_word_addr[measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1];
			measure_word_addr[max_max_bit*1+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]+column_shift_bit;
			measure_word_addr[max_max_bit*2+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]-column_shift_bit;
			measure_word_addr[max_max_bit*3+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]+row_shift_bit;
			measure_word_addr[max_max_bit*4+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]-row_shift_bit;
			measure_no_flag++;
		} 
		if(bist_fail2)  printf("fail bist2:%d\n",bist_fail2);
		if(bist_fail3)  printf("fail bist3:%d\n",bist_fail3);
	}
    if(fail_word_addr[0][dut-1][g_flash_no-1]==-1){printf("\n NO FAIL WORD FOUND!!! !!! !!!\n\n");}
}
void find_1_word_each_row(int dut)
{
	int measure_no_flag=0;
	ulConfigFcm("SO", 0, 6, 8192);
for(int row=start_row;row<=end_row;row=row+row_shift_bit)
{
		if((measure_no_flag>=max_max_bit)){printf("measure_no_flag is enough.\n");break;}                     
		if(strstr(efa_bin_string,efa_2x8rows)&&(row<end_row-7*row_shift_bit)&&(row>7*row_shift_bit)){continue;}
        	ulPresetFcm(); 
		    	setRegAddr(AREA_M,row);printf("row=0x%x\t",row);
    		if((strstr(efa_bin_string,efa_diagonal) || strstr(efa_bin_string,efa_ckbd))||strstr(efa_bin_string,efa_2x8rows)&& (!strstr(efa_bin_string,efa_ickbd))){setRegDataIn(0x0,0x0,0x0);	}
    		else if(strstr(efa_bin_string,efa_ff) || strstr(efa_bin_string,efa_ickbd)){ setRegDataIn(0xff,0xffffffff,0xffffffff);}

		if((strstr(efa_bin_string,efa_ff))||strstr(efa_bin_string,efa_2x8rows)){printf("EFA_BI_RSLRD_NORL\t");FlashPatExec("patFlash.mpa", "EFA_BI_RSLRD_NORL");	}
		else if(strstr(efa_bin_string,efa_ckbd) || strstr(efa_bin_string,efa_ickbd)){	printf("EFA_BI_RSLRD_CKBD\t");FlashPatExec("patFlash.mpa", "EFA_BI_RSLRD_CKBD");	}

		//--//--//--//--send addr to measure_word_addr
		unsigned int data[DDUTCNT][48];
		int bist_fail3=0, bist_fail2=0, bist_fail1=0;
		ulReadFcm(data[dut-1], 48, 0, 0, 0, 47, dut);
		bist_fail3=data[dut-1][5];
		bist_fail2=data[dut-1][26];
		bist_fail1=data[dut-1][47];
		if(bist_fail1==1){
			fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]=0x0;
		        for(int ii=20;ii>=1;ii--){
		                fail_word_addr[measure_no_flag][dut-1][g_flash_no-1] |= data[dut-1][47-ii]<<(ii-1);
		        }                
			printf("fail1 wordaddr[%d]:0x%x\n",measure_no_flag,fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]);
		        measure_word_addr[measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1];
		        measure_word_addr[max_max_bit*1+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]+column_shift_bit;
		        measure_word_addr[max_max_bit*2+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]-column_shift_bit;
		        measure_word_addr[max_max_bit*3+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]+row_shift_bit;
		        measure_word_addr[max_max_bit*4+measure_no_flag][dut-1][g_flash_no-1]=fail_word_addr[measure_no_flag][dut-1][g_flash_no-1]-row_shift_bit;
			measure_no_flag++;
		}   
		if(bist_fail2)  printf("fail bist2:%d\n",bist_fail2);
		if(bist_fail3)  printf("fail bist3:%d\n",bist_fail3);
    }
	
    if(fail_word_addr[0][dut-1][g_flash_no-1]==-1){printf("\n NO FAIL WORD FOUND!!! !!! !!!\n\n");}
}
void find_1_word_each_word(int dut)
{

}
void find_1_bit_each_word()
{

}





void measure_cell_current_40_128Kx64(int dut)
{
	printf("measure_cell_current_40_128Kx64\n");		

	//-- set force voltage
	if(tm0_vol>3){tm0_vol=0.5;printf("The set of tm0_vol is to big, please check!(measure with tm0_vol=0.5)\n");}
	setDcVSIM(tm0_vol V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 5);
//	ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2

	int start_addr=0;
	int end_addr=0;

	//-- measure only find fail bit and reference
	if(ext_bin_no[dut-1]!=1)
	{
		if(cover_mode==0)
		{
			//-- //-- cover mode = word_add  measure
			for(int ii=0;ii<max_max_bit*5;ii++)
			{
				if(measure_word_addr[ii][dut-1][g_flash_no-1]>=0)
				{
					start_addr=measure_word_addr[ii][dut-1][g_flash_no-1];
					end_addr=start_addr;
					cellcurrent_measure_40_128Kx64(dut, start_addr, end_addr);
				}
			}
		}
		else
		{
			//-- //-- cover mode = row_add  measure
			for(int ii=0;ii<max_max_bit;ii++)
			{
				if(fail_word_addr[ii][dut-1][g_flash_no-1]>=0)
				{
					if(fail_word_addr[ii][dut-1][g_flash_no-1]<row_shift_bit){fail_word_addr[ii][dut-1][g_flash_no-1]=fail_word_addr[ii][dut-1][g_flash_no-1]+row_shift_bit;}
					start_addr=fail_word_addr[ii][dut-1][g_flash_no-1]&(row_shift_bit*2047)-row_shift_bit;
					end_addr=start_addr+(row_shift_bit*3)-1;
					printf("\n\nreference_addr=0x%x,start_addr_a0=0x%x,end_addr_a0=0x%x\n",fail_word_addr[ii][dut-1][g_flash_no-1],start_addr,end_addr);
					cellcurrent_measure_40_128Kx64(dut, start_addr, end_addr);
				}
			}
		}
	}
	//-- measure mass words from start_addr to end addr
	else
	{
		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");	printf("physical address\n");				
		start_addr=ext_mass_start_addr[dut-1][g_flash_no-1][repeat_no];
		end_addr=ext_mass_end_addr[dut-1][g_flash_no-1][repeat_no];
		cellcurrent_measure_40_128Kx64(dut, start_addr, end_addr);
	}
}






void cellcurrent_measure_40_128Kx64(int dut,int start_addr,int end_addr)
{
	//-- //-- set parameter save measure value
	double temp_1[io_no][DDUTCNT];
	double temp_2[io_no][DDUTCNT];

	for(int i=start_addr;i<=end_addr;i++){
		for(int j=0;j<io_no;j++){
			if (j<io_no/2){	setRegAddr(AREA_M,i+write_mask_low);	setRegDataIn(0xff,0xffffffff,~(0x1<<j));}
			else {			setRegAddr(AREA_M,i+write_mask_high);	setRegDataIn(0xff,0xffffffff,~(0x1<<(j-32)));}
			FlashPatExec("patFlash.mpa", "EFA_CELL_CURRENT");
			UTL_WaitTime(2 MS);
			DcExec(tm_pinlist[g_flash_no-1]);
			FlashPatExec("patFlash.mpa","TMODE_EXIT");
			
			TransMeasData(tm_pinlist[g_flash_no-1], "REF_current",200 UA , -200 UA, 1 UA, "UA",temp_1[j],dut);
		}
		printf("\ncell_current\taddr:0x%x\tDUT%2d(uA): \t",i,dut);
		for(int j=0;j<io_no; j++)printf("%.2f,",temp_1[j][dut-1]);
	}
}


void measure_cell_current_modeonce_40_128Kx64(int dut)
{
	printf("measure_cell_current_modeonce_40_128Kx64\n");		

	//-- set force voltage
	if(tm0_vol>3){tm0_vol=0.5;printf("The set of tm0_vol is to big, please check!(measure with tm0_vol=0.5)\n");}
	setDcVSIM(tm0_vol V, R20V, R200uA,200 UA, -200 UA, 100 UA,-100 UA, 5);
//	ulSetSettlingTime(2 MS,2 MS,2 MS);//t:MVM/VSIM t1/t2
	int start_addr=0;
	int end_addr=0;
	
	//-- CELL CURRENT mode or other mode enter
	if(strstr(efa_bin_string,efa_cg3p5v))
	{	
		setHvUnMask("VPP0_PINS");
		setHvVSIM("VPP0_PINS",3.5 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);	
		printf("set external tester HV\n");
		FlashPatExec("patFlash.mpa", "CG3P5V_CELL_CURRENT_MODE");	printf("CG3P5V_CELL_CURRENT_MODE\n");
	}
	else 
	{
		FlashPatExec("patFlash.mpa", "CELL_CURRENT_MODE");	printf("CELL_CURRENT_MODE\n");
	}
	
	
	//-- measure only find fail bit and reference
	if(ext_bin_no[dut-1]!=1)
	{
		if(cover_mode==0)
		{
			//-- //-- cover mode = word_add  measure
			for(int ii=0;ii<max_max_bit*5;ii++)
			{
				if(measure_word_addr[ii][dut-1][g_flash_no-1]>=0)
				{
					start_addr=measure_word_addr[ii][dut-1][g_flash_no-1];
					end_addr=start_addr;
					cellcurrent_measure_modeonce_40_128Kx64(dut, start_addr, end_addr);
				}
			}
		}
		else
		{
			//-- //-- cover mode = row_add  measure
			for(int ii=0;ii<max_max_bit;ii++)
			{
				if(fail_word_addr[ii][dut-1][g_flash_no-1]>=0)
				{
					if(fail_word_addr[ii][dut-1][g_flash_no-1]<row_shift_bit){fail_word_addr[ii][dut-1][g_flash_no-1]=fail_word_addr[ii][dut-1][g_flash_no-1]+row_shift_bit;}
					start_addr=fail_word_addr[ii][dut-1][g_flash_no-1]&(row_shift_bit*2047)-row_shift_bit;
					end_addr=start_addr+(row_shift_bit*3)-1;
					printf("\n\nreference_addr=0x%x,start_addr_a0=0x%x,end_addr_a0=0x%x\n",fail_word_addr[ii][dut-1][g_flash_no-1],start_addr,end_addr);
					cellcurrent_measure_modeonce_40_128Kx64(dut, start_addr, end_addr);
				}
			}
		}
	}
	//-- measure mass words from start_addr to end addr
	else
	{
		FlashPatExec("patFlash.mpa", "FLASH_SH_ADDRS_01");	printf("physical address\n");				
		start_addr=ext_mass_start_addr[dut-1][g_flash_no-1][repeat_no];
		end_addr=ext_mass_end_addr[dut-1][g_flash_no-1][repeat_no];
		cellcurrent_measure_modeonce_40_128Kx64(dut, start_addr, end_addr);
	}
	
	//-- CELL CURRENT mode exit or other mode exit 
	FlashPatExec("patFlash.mpa","TMODE_EXIT");
	if(strstr(efa_bin_string,efa_cg3p5v)){
			setHvVSIM("VPP0_PINS",0 V,R32V ,M8uA,1 MA,-1 MA,8 UA, -8 UA, 5);
			setHvMask("VPP0_PINS"); 
			printf("\nclose external tester HV\n");
	}
}

void cellcurrent_measure_modeonce_40_128Kx64(int dut,int start_addr,int end_addr)
{
	//-- //-- set parameter save measure value
	double temp_1[io_no][DDUTCNT];
	double temp_2[io_no][DDUTCNT];

	for(int i=start_addr;i<=end_addr;i++){
		for(int j=0;j<io_no;j++){
			if (j<io_no/2){	setRegAddr(AREA_M,i+write_mask_low);	setRegDataIn(0xff,0xffffffff,~(0x1<<j));}
			else {			setRegAddr(AREA_M,i+write_mask_high);	setRegDataIn(0xff,0xffffffff,~(0x1<<(j-32)));}
			FlashPatExec("patFlash.mpa", "EFA_CELL_CURRENT_MODEONCE");
			UTL_WaitTime(1 MS);
			DcExec(tm_pinlist[g_flash_no-1]);
			TransMeasData(tm_pinlist[g_flash_no-1], "REF_current",200 UA , -200 UA, 1 UA, "UA",temp_1[j],dut);
		}
		printf("\ncell_current\taddr:0x%x\tDUT%2dFlash%d(uA): \t",i,dut,g_flash_no);
		for(int j=0;j<io_no; j++)printf("%.2f,",temp_1[j][dut-1]);
	}
}




//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************
//-- EFA reference void**********************************************************************************************************************************************************









