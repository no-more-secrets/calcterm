ifndef TOPLEVELWD
    include $(dir $(lastword $(MAKEFILE_LIST)))../Makefile
else
    # In general, must enter in order of dependencies.
    locations := icalc defcalc
    $(call enter_all,$(locations))

    TP_LINK_MAIN := -lncurses
    $(call make_exe,MAIN,calcterm)
endif
