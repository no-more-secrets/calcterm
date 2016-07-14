ifndef TOPLEVELWD
    include $(dir $(lastword $(MAKEFILE_LIST)))../makefile
    $(call set_default_goal,DEFCALC)
else
    $(call make_so,DEFCALC,defcalc)
endif
