define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=Locale.h Parser.h

$(_flavor_)_$(_feat_)_lib_objs=Parser Locale

$(_flavor_)_$(_feat_)_lib=util

endef

include $(PROJ_MAK_DIR)/epilog.mk
