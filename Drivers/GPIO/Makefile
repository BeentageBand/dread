SRC=$(wildcard *.c)
INC=$(wildcard *.h)
LIB=lib$(PKG)-gpio.a

.PHONY: all clean

all: $(INSTALL_DIR)/lib/$(LIB) $(INC:%=$(INSTALL_DIR)/include/%)

clean : $(wildcard *.[oa])
	rm -f $^;

$(LIB) : $(SRC:.c=.o)
	$(AR) -rcs $@ $^;

$(INSTALL_DIR)/lib/$(LIB) : $(LIB)
	-cp -f $^ $@;

$(INSTALL_DIR)/include/%.h : %.h
	-cp -f $^ $@;

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^;
