export AR=avr-ar
export CXX=avr-g++
export CC=avr-gcc
export BUILD_DIR=$(realpath .)
export INSTALL_DIR=$(BUILD_DIR)/out
export CFLAGS=\
	-funsigned-bitfields                                                  \
	-fdata-sections                                                       \
	-fpack-struct                                                         \
	-fshort-enums                                                         \
	-g2                                                                   \
	-Wall                                                                 \
	-mmcu=atmega328p                                                      \
	-Os                                                                   \
	-ffunction-sections                                                   \
	-DF_CPU=8000000L                                                      \
	-fno-threadsafe-statics                                               \
	-I $(INSTALL_DIR)/include

LIBS=\
		 app-card-reader \
		 support-scheduler support-persistence support-scheduler \
		 modules-sim900 modules-sd modules-sd-utility modules-rs485 modules-mifare modules-hid \
		 drivers-gpio drivers-hardware-serial drivers-software-serial drivers-timers drivers-spi \
		 dread-utilities

LDFLAGS=-L /usr/lib/avr/ -Wl,--gc-sections -lm -L $(INSTALL_DIR)/lib $(LIBS:%=-l%)
SUBDIRS=Application Drivers Modules Support Utilities 

SRC=$(wildcard *.cpp)

.PHONY: all $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)

all: $(SUBDIRS:%=%-all) $(INSTALL_DIR)/bin/dread-launcher

clean: $(SUBDIRS:%=%-clean) $(wildcard *.[oa])
	rm -f *.[oa];
	
$(INSTALL_DIR)/bin/dread-launcher : $(SRC:.cpp=.o) $(INSTALL_DIR)/bin 
	$(CXX) $(CFLAGS) -o $@ $< $(LDFLAGS);

clobber : clean
	-rm -rf $(INSTALL_DIR);

$(SUBDIRS:%=%-all) : $(INSTALL_DIR)/lib $(INSTALL_DIR)/include
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) :
	make clean -C $(@:%-clean=%);

$(INSTALL_DIR) $(INSTALL_DIR)/lib $(INSTALL_DIR)/bin $(INSTALL_DIR)/include : 
	-mkdir -p $@;

%.o : %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^;
