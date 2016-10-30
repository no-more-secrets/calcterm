ifndef root
    include $(dir $(lastword $(MAKEFILE_LIST)))../Makefile
    $(call set_default_goal,DEFCALC)
else
    $(call make_so,DEFCALC,defcalc)
endif
