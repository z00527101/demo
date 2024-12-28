	TRANS = $(ATFSSYSTEM)/$(ATFSSYS)/bin/trans
	SUBPATSRC=subpat.asc
	SUBPATOBJ20=subpat20mhz.asc
	SUBPATOBJ15=subpat15mhz.asc
	SUBPATOBJ10=subpat10mhz.asc
	SUBPATOBJ5=subpat5mhz.asc

	MPATSRC= $(shell ls *.asc | grep -v "sub")
	MPATOBJ=${MPATSRC:.asc=.mpa}

all: create20MHZ create15MHZ create10MHZ create5MHZ cln_sub 
#all:create20MHZ cln_sub 
	perl atags *

body:$(MPATOBJ) $(MPATPREP) 

%.prep:%.asc
	@cpp -P $(FLOW) $< >$@
%.mpa: %.prep 
	$(TRANS) $< 


$(SUBPATOBJ20):$(SUBPATSRC)
	@cpp -P $(FLOW) $< >$@
$(SUBPATOBJ15):$(SUBPATSRC)
	@cpp -P $(FLOW) $< >$@
$(SUBPATOBJ10):$(SUBPATSRC)
	@cpp -P $(FLOW) $< >$@
$(SUBPATOBJ5):$(SUBPATSRC)
	@cpp -P $(FLOW) $< >$@
	

create20MHZ:
	@$(MAKE) $(SUBPATOBJ20) FLOW="-D RATE20MHZ"	
	@$(MAKE) body FLOW="-D RATE20MHZ"	

create15MHZ:
	@$(MAKE) $(SUBPATOBJ15) FLOW="-D RATE15MHZ"	
	@$(MAKE) body FLOW="-D RATE15MHZ"	

create10MHZ:
	@$(MAKE) $(SUBPATOBJ10) FLOW="-D RATE10MHZ"	
	@$(MAKE) body FLOW="-D RATE10MHZ"	

create5MHZ:
	@$(MAKE) $(SUBPATOBJ5) FLOW="-D RATE5MHZ"	
	@$(MAKE) body FLOW="-D RATE5MHZ"	

cln:
	rm -f *.mpa
	rm -f sub*mhz.asc

cln_sub:
	rm -f sub*mhz.asc
tag:
	perl atags *
