PROJ_DIR=$(realpath .)
PROJ_MAK_DIR=$(PROJ_DIR)/build

define Project_Make

FLAVOR_LIST=std
#Flavor
std_build=avr
host_build=linux
avr_MCU=atmega328p

std_PROJ_INC+=\
include/avr/config.h

host_PROJ_INC+=\
include/host/config.h \
$(shell pkg-config --cflags gmock_main gtest gmock)

$(host_build)_PROJ_LIBS=$(shell pkg-config --libs --static gmock_main gtest gmock)

endef

include $(PROJ_MAK_DIR)/project.mk
