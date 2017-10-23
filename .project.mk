# ===============================================================
# This is where all of the top-level, project-specific
# information is supplied.
# ===============================================================

HIDE_SYMS         = 1
VISIBILITY_HIDDEN = $(if $(HIDE_SYMS),-fvisibility=hidden,)
CXXFLAGS         += $(VISIBILITY_HIDDEN)

CXXFLAGS += -std=c++1z

GPLUSPLUS ?= /usr/bin/g++

CC  := $(GPLUSPLUS)
CXX := $(GPLUSPLUS)
LD  := $(GPLUSPLUS)

ifeq ($(OS),Linux)
    # On linux let's do a static linkage
    LDFLAGS += -static-libgcc -static-libstdc++
endif

ENABLE_BIN_FOLDER = 1

MAIN.deps    := ICALCTERM_INTERFACE
DEFCALC.deps := ICALCTERM_INTERFACE

main_is      := MAIN

# ===============================================================
# Here we traverse the source tree
# ===============================================================
$(call enter,src)
