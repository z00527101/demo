
onsmpa:
	@make -f makefile.body mpa 
onm:
	@make -f makefile.body cln_all
	@make -f makefile.body obj_dbm status=online 
	@make -f makefile.body mpa 
ofm:
	@make -f makefile.body cln_all
	@make -f makefile.body obj_dbm status=offline 
	@make -f makefile.body mpa 
	
ons:
	@make -f makefile.body single_cln_objmpa status=online 
ofs:
	@make -f makefile.body single_cln_objmpa status=offline

cln:
	@make -f makefile.body cln_objmpa

onmcsv:
	@make -f makefile.body cln_all
	@make -f makefile.body obj_csv status=online 
	@make -f makefile.body mpa 
ofmcsv:
	@make -f makefile.body cln_all
	@make -f makefile.body obj_csv status=offline 
	@make -f makefile.body mpa 
onscsv:
	@make -f makefile.body single_cln_all status=online 
ofscsv:
	@make -f makefile.body single_cln_all status=offline
clncsv:
	@make -f makefile.body cln_all

