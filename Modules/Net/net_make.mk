define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=NetManager.h Socket.h Types.h

$(_flavor_)_$(_feat_)_lib_objs=NetManager Socket

$(_flavor_)_$(_feat_)_lib=net

endef

include $(PROJ_MAK_DIR)/epilog.mk
