define $(_flavor_)_$(_feat_)_MAKE

host_$(_feat_)_inc=

host_$(_feat_)_bin_objs= \
NetManagerTests          \
Stubs                    \
SchedulerTests           \

host_$(_feat_)_bin_libs=  \
	net                   \
	utilities             \
	scheduler             \

host_$(_feat_)_bin=unit-tests

endef

include $(PROJ_MAK_DIR)/epilog.mk
