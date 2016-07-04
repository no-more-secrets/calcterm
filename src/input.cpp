#include "input.hpp"
#include "scope_exit.hpp"
#include <stdexcept>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define ASSERT( a ) \
    if( !(a) )        \
        throw std::logic_error( \
            "Assertion error:" __FILE__ ":" TOSTRING(__LINE__) \
        );

#define ASSERT_INVARIANTS                        \
    ASSERT( width     > 0 );                     \
    ASSERT( width % 2 == 0 );                    \
                                                 \
    ASSERT( start_pos >= 0 );                    \
    ASSERT( start_pos < (int)buffer.length() );  \
    ASSERT( start_pos % (width/2) == 0 );        \
                                                 \
    ASSERT( pos >= start_pos );                  \
    ASSERT( pos <  start_pos+width );            \
    ASSERT( pos <= (int)buffer.length() );

#define SCOPE_ASSERT SCOPE_EXIT( ASSERT_INVARIANTS )

Input::Input(int w) : width(w)
                    , buffer("")
                    , start_pos(0)
                    , pos(0) {
    SCOPE_ASSERT
}

void Input::update_start() {
    SCOPE_ASSERT
    start_pos = (pos/(width/2))*(width/2);
}

void Input::paste( std::string const& new_string ) {
    SCOPE_ASSERT
    buffer.insert( pos, new_string );
    pos += new_string.length();
}

void Input::draw( int y, int x ) const {

}

int Input::get_cursor() const {
    return pos-start_pos;
}

bool Input::key_press( int pressed ) {
    SCOPE_ASSERT
    return true;
}
