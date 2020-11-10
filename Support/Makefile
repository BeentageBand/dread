export PKG=support
SUBDIRS=FSM Modem Persistence Router Scheduler Util 

.PHONY: all clean $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)

all: $(SUBDIRS:%=%-all)

clean :$(SUBDIRS:%=%-clean) 

$(SUBDIRS:%=%-all) : 
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) :
	make clean -C $(@:%-clean=%);
