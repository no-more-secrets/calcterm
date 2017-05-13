#include "keyboard_input.hpp"
#include "assert.hpp"

#include <ncurses.h>

using namespace std;

#define ASSERT_INVARIANTS                  \
                                           \
    ASSERT( pos >= 0 )                     \
    ASSERT( pos <= (int)buffer.length() );

KeyboardInput::KeyboardInput() : buffer()
                               , pos( 0 )
{}

KeyboardInput::KeyboardInput( string const& buffer,
                              int           pos )
    : buffer( buffer )
    , pos( pos )
{}

void KeyboardInput::clear() {
    buffer.clear();
    pos = 0;
    ASSERT_INVARIANTS
}

void KeyboardInput::key_press( bool        ctrl,
                               bool        alt,
                               int         pressed,
                               char const* name ) {
    if( !ctrl && !alt ) {
        if( (pressed >= 'a' && pressed <= 'z') ||
            (pressed >= 'A' && pressed <= 'Z') ||
            (pressed >= '0' && pressed <= '9') ||
            (pressed == ' ') ||
            (pressed == '+') ||
            (pressed == '-') ||
            (pressed == '*') ||
            (pressed == '/') ||
            (pressed == '^') ||
            (pressed == '.') ||
            (pressed == '_') ||
            (pressed == ',') ||
            (pressed == '(') ||
            (pressed == ')')) {
            buffer.insert(pos, 1, (char)pressed);
            ++pos;
        }
        else if( pressed == KEY_LEFT ) {
            --pos;
            pos = (pos < 0) ? 0 : pos;
        }
        else if( pressed == KEY_RIGHT ) {
            ++pos;
            pos = (pos > (int)buffer.length()) ? buffer.length() : pos;
        }
        else if( pressed == KEY_BACKSPACE ) {
            if( pos > 0 ) {
                buffer.erase( pos-1, 1 );
                --pos;
            }
        }
        else if( pressed == KEY_DC ) {
            if( pos < (int)buffer.length() ) {
                buffer.erase( pos, 1 );
            }
        }
        else if( pressed == KEY_END ) {
            pos = buffer.length();
        }
        else if( pressed == KEY_HOME ) {
            pos = 0;
        }
    }
    else if( ctrl && !alt ) {
        if( name[1] == 'U' ) {
            buffer.erase( 0, pos ); 
            pos = 0;
        }
    }
    ASSERT_INVARIANTS
}
