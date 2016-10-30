# ===============================================================
# This is where all of the top-level, project-specific
# information is supplied.
# ===============================================================

HIDE_SYMS         = 1
VISIBILITY_HIDDEN = $(if $(HIDE_SYMS),-fvisibility=hidden,)
CXXFLAGS         += $(VISIBILITY_HIDDEN)

CXXFLAGS += -std=c++1y

GCC_HOME=/usr/bin

CC  := $(GCC_HOME)/g++
CXX := $(CC)
LD  := $(CC)

ENABLE_BIN_FOLDER = 1

MAIN.deps    := ICALCTERM_INTERFACE
DEFCALC.deps := ICALCTERM_INTERFACE

# ===============================================================
# Here we traverse the source tree
# ===============================================================
$(call enter,src)
