/****************************************************************
* Some general macro utilities.
****************************************************************/
#pragma once

#include <stdexcept>
#include <sstream>

#define STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define STRING_JOIN(arg1, arg2) STRING_JOIN2(arg1, arg2)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASSERTM( a, b )                       \
    if( !(a) ) {                              \
        std::ostringstream out;               \
        out << "error:" __FILE__ ":";         \
        out << TOSTRING(__LINE__) ": " << #a; \
        std::ostringstream out_msg;           \
        out_msg << b;                         \
        if( !out_msg.str().empty() )          \
            out << std::endl << b;            \
        throw std::logic_error( out.str() );  \
    }

#define ASSERT( a ) ASSERTM( a, "" )
