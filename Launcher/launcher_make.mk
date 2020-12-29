define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=Dread.h

std_$(_feat_)_bin_objs=std-dread-launcher

slv_$(_feat_)_bin_objs=slv-dread-launcher

mst_$(_feat_)_bin_objs=mst-dread-launcher

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
	filepersistence            \
	persistence                \
	rfid                       \
	scheduler                  \
	sd                         \
	softwareserial             \
	spi                        \
	timers                     \
	util                       \
	utilities                  \

slv_$(_feat_)_bin_libs=  \
	hid                        \
	cardreader                 \
	gpio                       \
	hardwareserial             \
	net                        \
	netgateway                 \
	persistence                \
	rfid                       \
	router                     \
	rs485                      \
	scheduler                  \
	softwareserial             \
	spi                        \
	timers                     \
	util                       \
	utilities                  \

mst_$(_feat_)_bin_libs=  \
	hid                        \
	connectionhandler          \
	dreadclient                \
	gpio                       \
	gprs                       \
	fsm                        \
	hardwareserial             \
	modem                      \
	net                        \
	netgateway                 \
	filepersistence            \
	persistence                \
	router                     \
	rs485                      \
	scheduler                  \
	sd                         \
	softwareserial             \
	spi                        \
	timers                     \
	util                       \
	utilities                  \

$(_flavor_)_$(_feat_)_bin=$(_flavor_)-dread-launcher

endef

include $(PROJ_MAK_DIR)/epilog.mk

