export PKG=app
SUBDIRS=CardReader ConnectionHandler DreadClient NetGateway

.PHONY: all clean $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)

all: $(SUBDIRS:%=%-all)

clean :$(SUBDIRS:%=%-clean) 

$(SUBDIRS:%=%-all) : 
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) :
	make clean -C $(@:%-clean=%);
