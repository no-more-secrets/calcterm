#include <cstdlib>
#include <cmath>
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

auto noop( st_t& ) -> void { }

auto plus( st_t& st ) -> void {
    auto d1 = st.top()._double;
    st.pop();
    auto d2 = st.top()._double;
    st.pop();
    st.push( toNE( d1 + d2 ) );
}

auto minus( st_t& st ) -> void {
    auto d1 = st.top()._double;
    st.pop();
    auto d2 = st.top()._double;
    st.pop();
    st.push( toNE( d2 - d1 ) );
}

auto times( st_t& st ) -> void {
    auto d1 = st.top()._double;
    st.pop();
    auto d2 = st.top()._double;
    st.pop();
    st.push( toNE( d2 * d1 ) );
}

auto div( st_t& st ) -> void {
    auto d1 = st.top()._double;
    st.pop();
    auto d2 = st.top()._double;
    st.pop();
    st.push( toNE( d2 / d1 ) );
}

auto neg( st_t& st ) -> void {
    auto d = st.top()._double;
    st.pop();
    st.push( toNE( -d ) );
}

auto pow_( st_t& st ) -> void {
    auto d1 = st.top()._double;
    st.pop();
    auto d2 = st.top()._double;
    st.pop();
    st.push( toNE( pow( d2, d1 ) ) );
}

auto log_( st_t& st ) -> void {
    auto d = st.top()._double;
    st.pop();
    st.push( toNE( log( d ) ) );
}

auto int_( st_t& st ) -> void {
    auto d = st.top()._double;
    st.pop();
    st.push( toNE( double( int( d ) ) ) );
}

auto reale( st_t& st ) -> void {
    auto l = st.top()._long;
    st.pop();
    auto d = st.top()._double;
    st.pop();
    st.push( toNE( d * pow( 10.0, (double)l ) ) );
}

auto pfintdotint( st_t& st, char* _1, char* _3 ) -> void {
    auto l1 = parse_long( _1 );
    auto decimal = make_decimal( _3 );
    st.push( toNE( (double)l1 + decimal ) );
}

auto pfintdot( st_t& st, char* _1 ) -> void {
    auto l = parse_long( _1 );
    st.push( toNE( (double)l ) );
}

auto pfint( st_t& st, char* _1 ) -> void {
    auto l = parse_long( _1 );
    st.push( toNE( (double)l ) );
}

auto pfdotint( st_t& st, char* _2 ) -> void {
    auto d = make_decimal( _2 );
    st.push( toNE( d ) );
}

auto inegint( st_t& st, char* _2 ) -> void {
    auto l = parse_long( _2 );
    st.push( toNE( -l ) );
}

auto iplusint( st_t& st, char* _2 ) -> void {
    auto l = parse_long( _2 );
    st.push( toNE( l ) );
}

auto iint( st_t& st, char* _1 ) -> void {
    auto l = parse_long( _1 );
    st.push( toNE( l ) );
}
