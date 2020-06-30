CSRC=$(wildcard *.c)
CCSRC=$(wildcard *.cpp)
INC=$(wildcard *.h)
LIB=lib$(PKG)-timers.a

all: $(INSTALL_DIR)/lib/$(LIB) $(INC:%=$(INSTALL_DIR)/include/%)

clean : $(wildcard *.[oa])
	rm -f $^;

$(LIB) : $(CSRC:.c=.o) $(CCSRC:.cpp=.o)
	$(AR) -rcs $@ $^;

$(INSTALL_DIR)/lib/$(LIB) : $(LIB)
	-cp -f $^ $@;

$(INSTALL_DIR)/include/%.h : %.h
	-cp -f $^ $@;

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^;

%.o : %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^;
