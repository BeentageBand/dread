PROJ_DIR=$(realpath .)
PROJ_MAK_DIR=$(PROJ_DIR)/build

std_PROJ_INC+=\
$(PROJ_DIR)/include/avr/ \

slv_PROJ_INC+=\
$(PROJ_DIR)/include/avr/ \

mst_PROJ_INC+=\
$(PROJ_DIR)/include/avr/ \

host_PROJ_INC+=\
$(PROJ_DIR)/include/linux/ \
$(shell pkg-config --cflags gmock_main gtest gmock)

std_build=avr
slv_build=avr
mst_build=avr
host_build=linux

avr_MCU=atmega328p

$(host_build)_PROJ_LIBS=$(shell pkg-config --libs --static gmock_main gtest gmock)

std_SUBDIRS=\
Application/CardReader         \
Application/ConnectionHandler  \
Application/DreadClient        \
Drivers/GPIO                   \
Drivers/HardwareSerial         \
Drivers/SoftwareSerial         \
Drivers/SPI                    \
Drivers/Timers                 \
Launcher                       \
Modules/GPRS                   \
Modules/HID                    \
Modules/RFID                   \
Modules/SD                     \
Support/FSM                    \
Support/Modem                  \
Support/Persistence            \
Support/Scheduler              \
Support/Util                   \
Utilities

slv_SUBDIRS=\
Application/CardReader         \
Application/NetGateway         \
Drivers/GPIO                   \
Drivers/HardwareSerial         \
Drivers/SoftwareSerial         \
Drivers/SPI                    \
Drivers/Timers                 \
Launcher                       \
Modules/HID                    \
Modules/Net                    \
Modules/RFID                   \
Modules/RS485                  \
Modules/SD                     \
Support/FSM                    \
Support/Persistence            \
Support/Scheduler              \
Support/Router                 \
Support/Util                   \
Utilities

mst_SUBDIRS=\
Application/NetGateway         \
Application/ConnectionHandler/ \
Application/DreadClient/       \
Drivers/GPIO                   \
Drivers/HardwareSerial         \
Drivers/SoftwareSerial         \
Drivers/SPI                    \
Drivers/Timers                 \
Launcher                       \
Modules/GPRS                   \
Modules/HID                    \
Modules/Net                    \
Modules/RS485                  \
Modules/SD                     \
Support/FSM                    \
Support/Modem                  \
Support/Persistence            \
Support/Scheduler              \
Support/Router                 \
Support/Util                   \
Utilities

host_SUBDIRS=     \
Modules/Net       \
Support/Scheduler \
Utilities         \
UnitTests


define Project_Make

FLAVOR_LIST=std slv mst host
#Flavor

endef

include $(PROJ_MAK_DIR)/project.mk
