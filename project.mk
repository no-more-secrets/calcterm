###############################################################################
# This is where all of the top-level, project-specific information is supplied.

CFLAGS         += -MMD -MP -m64 -Wall -Wpedantic
CXXFLAGS       += $(CFLAGS) -std=c++1y

CFLAGS_DEBUG   += $(CXXFLAGS) -g -ggdb
CFLAGS_RELEASE += $(CXXFLAGS) -Ofast -march=corei7 -mtune=corei7

CFLAGS_LIB     += -fPIC

ifdef OPT
    CXXFLAGS_TO_USE = $(CFLAGS_RELEASE)
else
    CXXFLAGS_TO_USE = $(CFLAGS_DEBUG)
endif

CC  := /Users/dsicilia/dev/tools/bin/g++-6.1.0
CXX := $(CC)
LD  := $(CC)

LDFLAGS :=
LDFLAGS_LIB := -shared

INSTALL_PREFIX := $(HOME)/tmp

ifeq ($(OS),OSX)
    CFLAGS += -DOS_OSX
else
    CFLAGS += -DOS_LINUX
    CFLAGS_DEBUG += -gstabs
endif

MAIN.deps    := ICALC_INTERFACE
DEFCALC.deps := ICALC_INTERFACE
