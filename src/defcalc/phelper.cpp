#include <cstdlib>
#include "phelper.hpp"

auto parse_long( char* l ) -> long {
    long res = atol( l );
    free( l );
    return res;
}

auto make_decimal( char* l ) -> double {
    std::string s( l );
    s = "0." + s;
    double res = atof( s.c_str() );
    free( l );
    return res;
}

template<>
auto toNE( double d ) -> NumElement {
    NumElement ne;
    ne.type = NumElement::ElemType::DOUBLE;
    ne._double = d;
    return ne;
}

template<>
auto toNE( long l ) -> NumElement {
    NumElement ne;
    ne.type = NumElement::ElemType::LONG;
    ne._long = l;
    return ne;
}
