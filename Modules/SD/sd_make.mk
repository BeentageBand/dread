define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=SD.h SdFat.h Sd2Card.h Sd2PinMap.h SdInfo.h FatStructs.h SdFatUtil.h

$(_flavor_)_$(_feat_)_lib_objs=SD File Sd2Card SdFile SdVolume

$(_flavor_)_$(_feat_)_lib=sd

endef

include $(PROJ_MAK_DIR)/epilog.mk
