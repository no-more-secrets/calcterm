# ===============================================================
# This is where all of the top-level, project-specific
# information is supplied.
# ===============================================================

HIDE_SYMS         = 1
VISIBILITY_HIDDEN = $(if $(HIDE_SYMS),-fvisibility=hidden,)
CXXFLAGS         += $(VISIBILITY_HIDDEN)

# Flex seems to put the 'register' keyword in generated code
# which is deprecated by c++1z and so causes a warning to be
# emitted which we can ignore.
CXXFLAGS += -std=c++1z -Wno-register

STATIC_LIBSTDCXX=

MAIN.deps    := ICALCTERM_INTERFACE
DEFCALC.deps := ICALCTERM_INTERFACE

main_is      := MAIN

# ===============================================================
# Here we traverse the source tree
# ===============================================================
$(call enter,src)
