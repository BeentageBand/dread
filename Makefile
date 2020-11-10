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
	-I $(INSTALL_DIR)/include                                             \
	-I $(INSTALL_DIR)/..                                                  \

export CXXFLAGS=$(CFLAGS)

SUBDIRS=Application Drivers Modules Support Utilities 

BIN=std-dread-launcher slv-dread-launcher

SRC=$(wildcard *.cpp)

.PHONY: all $(SUBDIRS:%=%-clean) $(SUBDIRS:%=%-all)  Launcher-all Launcher-clean

all: $(SUBDIRS:%=%-all) $(BIN:%=$(INSTALL_DIR)/bin/%) 

clean: $(SUBDIRS:%=%-clean) Launcher-clean
	rm -f *.[oa];
	
$(BIN:%=$(INSTALL_DIR)/bin/%) : $(INSTALL_DIR)/bin Launcher-all 

clobber : clean
	-rm -rf $(INSTALL_DIR);

$(SUBDIRS:%=%-all) Launcher-all : $(INSTALL_DIR)/lib $(INSTALL_DIR)/include
	make all -C $(@:%-all=%);

$(SUBDIRS:%=%-clean) Launcher-clean:
	make clean -C $(@:%-clean=%);

$(INSTALL_DIR) $(INSTALL_DIR)/lib $(INSTALL_DIR)/bin $(INSTALL_DIR)/include : 
	-mkdir -p $@;

