#include "input.hpp"
#include "scope_exit.hpp"
#include "assert.hpp"

#define ASSERT_INVARIANTS                         \
    ASSERT( width     > 0 );                      \
    ASSERT( width % 2 == 0 );                     \
                                                  \
    ASSERT( start_pos >= 0 );                     \
    ASSERT( buffer.length() == 0 ||               \
            (start_pos < (int)buffer.length()) ); \
    ASSERT( start_pos % (width/2) == 0 );         \
                                                  \
    ASSERT( pos >= start_pos );                   \
    ASSERT( pos <  start_pos+width );             \
    ASSERT( pos <= (int)buffer.length() );

Input::Input(int w) : width(w)
                    , buffer("")
                    , start_pos(0)
                    , pos(0) {
    ASSERT_INVARIANTS
}

void Input::draw( int y, int x ) const {
    std::string out = (char const*)&buffer[start_pos];
    int residual = buffer.length()-start_pos;
    residual = (residual >= width) ? 0 : width-residual;
    out += std::string( residual, ' ' );
    mvaddnstr( y, x, out.c_str(), width );
}

int Input::get_cursor() const {
    return pos-start_pos;
}

void Input::update_start() {
    if( start_pos != (int)buffer.size() )
        if( pos >= start_pos && pos < start_pos+width )
            return;
    start_pos = (pos/(width/2))*(width/2);
    if( start_pos == pos )
        start_pos -= width/2;
    if( start_pos < 0 )
        start_pos = 0;
    ASSERT_INVARIANTS
}

void Input::paste( std::string const& new_string ) {
    buffer.insert( pos, new_string );
    pos += new_string.length();
    ASSERT_INVARIANTS
    update_start();
    ASSERT_INVARIANTS
}

bool Input::key_press( int pressed ) {
    if( (pressed >= 'a' && pressed <= 'z') ||
        (pressed >= 'A' && pressed <= 'Z') ||
        (pressed >= '0' && pressed <= '9') ||
        (pressed == ' ') ||
        (pressed == '+') ||
        (pressed == '-') ||
        (pressed == '*') ||
        (pressed == '/') ||
        (pressed == '^') ||
        (pressed == '(') ||
        (pressed == ')')) {
        buffer.insert(pos, 1, (char)pressed);
        ++pos;
        update_start();
    }
    else if( pressed == KEY_LEFT ) {
        --pos;
        pos = (pos < 0) ? 0 : pos;
        update_start();
    }
    else if( pressed == KEY_RIGHT ) {
        ++pos;
        pos = (pos > (int)buffer.length()) ? buffer.length() : pos;
        update_start();
    }
    else if( pressed == KEY_BACKSPACE ) {
        if( pos > 0 ) {
            buffer.erase( pos-1, 1 );
            --pos;
            update_start();
        }
    }
    else if( pressed == KEY_DC ) {
        if( pos < (int)buffer.length() ) {
            buffer.erase( pos, 1 );
            update_start();
        }
    }
    ASSERT_INVARIANTS
    return true;
}
