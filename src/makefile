ifndef TOPLEVELWD
    include $(dir $(lastword $(MAKEFILE_LIST)))../Makefile
    $(call set_default_goal,CLI)
else
    TP_LINK_CLI := -lncurses
    $(call make_exe,CLI,calcterm)
endif
