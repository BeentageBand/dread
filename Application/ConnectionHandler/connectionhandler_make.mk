define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=ConnectionHandlerTypes.h ConnectionHandler.h

$(_flavor_)_$(_feat_)_lib_objs=ConnectionHandler

$(_flavor_)_$(_feat_)_lib=connectionhandler

endef

include $(PROJ_MAK_DIR)/epilog.mk
