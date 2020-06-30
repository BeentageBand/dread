export PKG=modules
SUBDIRS=GPRS RFID RS485 SD

.PHONY: all clean $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)

all: $(SUBDIRS:%=%-all)

clean :$(SUBDIRS:%=%-clean) 

$(SUBDIRS:%=%-all) : 
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) :
	make clean -C $(@:%-clean=%);
