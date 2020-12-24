define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=NGClient.h NGServer.h Server.h

$(_flavor_)_$(_feat_)_lib_objs=NGClient NGServer

$(_flavor_)_$(_feat_)_lib=netgateway

endef

include $(PROJ_MAK_DIR)/epilog.mk
