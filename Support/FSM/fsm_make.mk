define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=FSM.h FSMState.h

$(_flavor_)_$(_feat_)_lib_objs=FSM FSMState

$(_flavor_)_$(_feat_)_lib=fsm

endef

include $(PROJ_MAK_DIR)/epilog.mk
