define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=Stream.h NonPersistence.h Persistence.h

$(_flavor_)_$(_feat_)_lib_objs=NonPersistence

$(_flavor_)_$(_feat_)_lib=persistence

endef

include $(PROJ_MAK_DIR)/epilog.mk
