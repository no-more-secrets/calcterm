/****************************************************************
* LineEditor: class that accepts a stream of key presses and uses
* them to edit a one-line block  of  text together with a "cursor
* position" within that line.
****************************************************************/
#include "line-editor.hpp"
#include "macros.hpp"

#include <ncurses.h>

#include <algorithm>
#include <string_view>
#include <vector>

using namespace std;

#define ASSERT_INVARIANTS                     \
    ASSERT( m_pos >= 0 )                      \
    ASSERT( m_pos <= (int)m_buffer.length() );

namespace {

bool is_char_allowed( char c ) {

    static string_view const cs{
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789 +-*/^._,()[]!@#$%&={}|?<>`~"
    };
    return find( begin( cs ), end( cs ), c ) != end( cs );
}

} // anonymous namespace

void LineEditor::input( bool        ctrl,
                        bool        alt,
                        int         pressed,
                        char const* name ) {

    if( ctrl && !alt ) {
        if( name[1] == 'U' ) {
            m_buffer.erase( 0, m_pos );
            m_pos = 0;
            ASSERT_INVARIANTS
            return;
        }
        if( pressed == 0x7f ) {
            // Hack for OS X in which backspace seems  to  appear
            // as a control  character  with  the  key being 0x7f.
            pressed = KEY_BACKSPACE;
            ctrl = alt = false;
        }
    }

    if( pressed < 128 && is_char_allowed( char( pressed ) ) ) {
        m_buffer.insert( m_pos, 1, char( pressed ) );
        ++m_pos;
        ASSERT_INVARIANTS
        return;
    }

    switch( pressed ) {

    case KEY_LEFT:
        --m_pos;
        m_pos = (m_pos < 0) ? 0 : m_pos;
        ASSERT_INVARIANTS
        return;

    case KEY_RIGHT:
        ++m_pos;
        m_pos = (m_pos > (int)m_buffer.length()) ? m_buffer.length() : m_pos;
        ASSERT_INVARIANTS
        return;

    case KEY_BACKSPACE:
        if( m_pos > 0 ) {
            m_buffer.erase( m_pos-1, 1 );
            --m_pos;
            ASSERT_INVARIANTS
        }
        return;

    case KEY_DC:
        if( m_pos < (int)m_buffer.length() ) {
            m_buffer.erase( m_pos, 1 );
            ASSERT_INVARIANTS
        }
        return;

    case KEY_END:
        m_pos = m_buffer.length();
        ASSERT_INVARIANTS
        return;

    case KEY_HOME:
        m_pos = 0;
        ASSERT_INVARIANTS
        return;
    };

    ASSERT_INVARIANTS
}
