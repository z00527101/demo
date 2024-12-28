onm:     
	cd pat;make
	make -f makefile.body status=online
#	@perl atags *
	ctags *
ofm:    
	@make -f makefile.body status=offline 
	cd pat;make
#	@perl atags *
	ctags *

cln:                                          
	@make -f makefile.body cln
	cd pat;make cln

tag:
	ctags *
#	@perl atags *
 
