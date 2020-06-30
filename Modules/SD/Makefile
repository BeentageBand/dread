SRC=$(wildcard *.cpp)
INC=$(wildcard *.h)
LIB=lib$(PKG)-sd.a
SUBDIRS=utility

.PHONY: all clean $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)

all: $(SUBDIRS:%=%-all) $(INSTALL_DIR)/lib/$(LIB) $(INC:%=$(INSTALL_DIR)/include/%)

clean : $(SUBDIRS:%=%-clean) $(wildcard *.[oa])
	rm -f $^;

$(SUBDIRS:%=%-all) : 
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) :
	make clean -C $(@:%-clean=%);

$(LIB) : $(SRC:.cpp=.o)
	$(AR) -rcs $@ $^;

$(INSTALL_DIR)/lib/$(LIB) : $(LIB)
	-cp -f $^ $@;

$(INSTALL_DIR)/include/%.h : %.h
	-cp -f $^ $@;

%.o : %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^;
