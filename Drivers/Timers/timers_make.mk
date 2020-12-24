define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=MsTimer2.h

$(_flavor_)_$(_feat_)_lib_objs=hooks MsTimer2 SystemClk

$(_flavor_)_$(_feat_)_lib=timers

endef

include $(PROJ_MAK_DIR)/epilog.mk
