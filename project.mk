###############################################################################
# This is where all of the top-level, project-specific information is supplied.

HIDE_SYMS = 1

VISIBILITY_HIDDEN = $(if $(HIDE_SYMS),-fvisibility=hidden,)

CFLAGS         += -MMD -MP -m64 -Wall -Wpedantic
CXXFLAGS       += $(CFLAGS) -std=c++1y $(VISIBILITY_HIDDEN)

CFLAGS_DEBUG   += $(CXXFLAGS) -g -ggdb
CFLAGS_RELEASE += $(CXXFLAGS) -Ofast -march=corei7 -mtune=corei7

CFLAGS_LIB     += -fPIC

ifdef OPT
    CXXFLAGS_TO_USE = $(CFLAGS_RELEASE)
else
    CXXFLAGS_TO_USE = $(CFLAGS_DEBUG)
endif

GCC_HOME=/usr/local/bin

CC  := $(GCC_HOME)/g++
CXX := $(CC)
LD  := $(CC)

LDFLAGS     :=
LDFLAGS_LIB := $(LDFLAGS) -shared 

INSTALL_PREFIX := $(HOME)/tmp

ifeq ($(OS),OSX)
    CFLAGS += -DOS_OSX
else
    CFLAGS += -DOS_LINUX
    CFLAGS_DEBUG += -gstabs
endif

MAIN.deps    := ICALCTERM_INTERFACE
DEFCALC.deps := ICALCTERM_INTERFACE
