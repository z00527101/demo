onm:
	@make -f makefile.body csv_sou_gen
	@make -f makefile.body dbm_load_gen
	@make -f makefile.body cln_all
	@make -f makefile.body obj_dbm status=online 
	@make -f makefile.body mpa 
ofm:
	@make -f makefile.body csv_sou_gen
	@make -f makefile.body dbm_load_gen
	@make -f makefile.body cln_all
	@make -f makefile.body obj_dbm status=offline 
	@make -f makefile.body mpa 
cln:
	@make -f makefile.body cln_all
