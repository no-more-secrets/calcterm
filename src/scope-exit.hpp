/****************************************************************
* Defines a utility that can  be  used  to  run arbitrary code at
* scope  exit. However, because this code will be run within a de-
* structor of a class, it should not throw any exceptions
****************************************************************/
#pragma once

#include "macros.hpp"

namespace {

template<typename T> struct ScopeExit {
    ScopeExit(T f) : f(f) {}
    ~ScopeExit() noexcept { f(); }
    T f;
};

} // anonymous namespace

#define SCOPE_EXIT(code) \
    ScopeExit STRING_JOIN( exit_, __LINE__ )( [&]{ code; } );
