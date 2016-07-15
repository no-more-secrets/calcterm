TOPLEVELWD := $(dir $(lastword $(MAKEFILE_LIST)))

makerules = $(TOPLEVELWD)makerules

include $(makerules)/toplevel.mk

CWD := $(TOPLEVELWD)
$(call enter,src)

include $(makerules)/postsrc.mk

bin_link = $(TOPLEVELWD)calcterm

all: $(BINARIES) $(bin_link)

$(bin_link): $(TOPLEVELWD)src/calcterm
	[ ! -e $(bin_link) ] && ln -s src/calcterm $(bin_link)

.DEFAULT_GOAL := all

.PHONY: all link
