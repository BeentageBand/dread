define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=Dread.h

std_$(_feat_)_bin_objs=std-dread-launcher

std_$(_feat_)_bin_libs=  \
	hid                        \
	cardreader                 \
	connectionhandler          \
	dreadclient                \
	gpio                       \
	gprs                       \
	hardwareserial             \
	fsm                        \
	modem                      \
	persistence                \
	rfid                       \
	scheduler                  \
	sd                         \
	softwareserial             \
	spi                        \
	timers                     \
	util                       \
	utilities                  \

std_$(_feat_)_bin=std-dread-launcher

endef

include $(PROJ_MAK_DIR)/epilog.mk

