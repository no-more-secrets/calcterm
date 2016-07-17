TOPLEVELWD := $(dir $(lastword $(MAKEFILE_LIST)))

makerules = $(TOPLEVELWD)makerules

include $(makerules)/presrc.mk

CWD := $(TOPLEVELWD)
$(call enter,src)

include $(makerules)/postsrc.mk

bin_link = $(TOPLEVELWD)calcterm

$(bin_link): $(TOPLEVELWD)src/calcterm
	[ ! -e $(bin_link) ] && ln -s src/calcterm $(bin_link)

.PHONY: link
