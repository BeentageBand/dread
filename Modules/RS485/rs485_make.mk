define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=RS485.h SC16IS740.h

$(_flavor_)_$(_feat_)_lib_objs=RS485 SC16IS740

$(_flavor_)_$(_feat_)_lib=rs485

endef

include $(PROJ_MAK_DIR)/epilog.mk
