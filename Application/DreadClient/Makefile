SRC=$(wildcard *.cpp)
INC=$(wildcard *.h)
LIB=lib$(PKG)-dread-client.a

.PHONY: all clean

all: $(INSTALL_DIR)/lib/$(LIB) $(INC:%=$(INSTALL_DIR)/include/%)

clean : $(wildcard *.[oa])
	rm -f $^;

$(LIB) : $(SRC:.cpp=.o)
	$(AR) -rcs $@ $^;

$(INSTALL_DIR)/lib/$(LIB) : $(LIB)
	-cp -f $^ $@;

$(INSTALL_DIR)/include/%.h : %.h
	-cp -f $^ $@;

%.o : %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^;
