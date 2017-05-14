#include "line_editor.hpp"
#include "assert.hpp"

#include <ncurses.h>

#include <algorithm>
#include <vector>

using namespace std;

#define ASSERT_INVARIANTS                  \
                                           \
    ASSERT( pos >= 0 )                     \
    ASSERT( pos <= (int)buffer.length() );

LineEditor::LineEditor() : buffer()
                               , pos( 0 ) {
    ASSERT_INVARIANTS
}

void LineEditor::clear() {
    buffer.clear();
    pos = 0;
    ASSERT_INVARIANTS
}

static bool is_char_allowed( char c ) {
    static string const cs{
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789 +-*/^._,()[]!@#$%&={}|?<>`~"
    };
    return find( begin( cs ), end( cs ), c ) != end( cs );
}

void LineEditor::key_press( bool        ctrl,
                            bool        alt,
                            int         pressed,
                            char const* name ) {
    if( ctrl && !alt ) {
        if( name[1] == 'U' ) {
            buffer.erase( 0, pos ); 
            pos = 0;
            ASSERT_INVARIANTS
            return;
        }
        if( pressed == 0x7f ) {
            // Hack for OS X in which backspace seems
            // to appear as a control character with
            // the key being 0x7f.
            pressed = KEY_BACKSPACE;
            ctrl = alt = false;
        }
    }
    if( pressed < 128 && is_char_allowed( char( pressed ) ) ) {
        buffer.insert( pos, 1, char( pressed ) );
        ++pos;
        ASSERT_INVARIANTS
        return;
    }
    switch( pressed ) {

    case KEY_LEFT:
        --pos;
        pos = (pos < 0) ? 0 : pos;
        ASSERT_INVARIANTS
        return;

    case KEY_RIGHT:
        ++pos;
        pos = (pos > (int)buffer.length()) ? buffer.length() : pos;
        ASSERT_INVARIANTS
        return;

    case KEY_BACKSPACE:
        if( pos > 0 ) {
            buffer.erase( pos-1, 1 );
            --pos;
            ASSERT_INVARIANTS
        }
        return;

    case KEY_DC:
        if( pos < (int)buffer.length() ) {
            buffer.erase( pos, 1 );
            ASSERT_INVARIANTS
        }
        return;

    case KEY_END:
        pos = buffer.length();
        ASSERT_INVARIANTS
        return;

    case KEY_HOME:
        pos = 0;
        ASSERT_INVARIANTS
        return;
    };
    ASSERT_INVARIANTS
}
