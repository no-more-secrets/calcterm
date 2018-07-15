/****************************************************************
* InputView: this  class  represents  a  moveable  window  into a
* string. It takes a string and renders a slice of  it  depending
* on the "location" of the window. This is not simply  slicing  a
* string though; this class must maintain internal state, because
* the movement of the window is intended to follow  the  movement
* of a cursor position (which must also be supplied to render the
* window) and the movement of the window is dependent not only on
* the  current cursor position but also on the the history of pre-
* vious  cursor positions. This is required in order to deliver a
* natural  experience  that  a user would expect when a string is
* navigated by moving a cursor along a line.
****************************************************************/
#include "input-view.hpp"
#include "scope-exit.hpp"
#include "macros.hpp"

using namespace std;

#define ASSERT_INVARIANTS_BUF                                       \
    ASSERT( (buffer.length() == 0 && m_start_pos == 0) ||           \
            (m_start_pos < (int)buffer.length()) );

#define ASSERT_INVARIANTS_POS                                       \
    ASSERT( width     > 0 );                                        \
    ASSERT( m_start_pos >= 0 );                                     \
    ASSERT( ((width & 1) && (m_start_pos % width == 0            || \
                             m_start_pos % width == (width-1)    || \
                             m_start_pos % width == (width/2)    || \
                             m_start_pos % width == (width/2)-1))   \
        || (!(width & 1) && (m_start_pos % (width/2) == 0)) );      \
    ASSERT( abs_cursor_pos >= m_start_pos );                        \
    ASSERT( abs_cursor_pos <  m_start_pos+width );

#define ASSERT_INVARIANTS                                           \
    ASSERT_INVARIANTS_POS                                           \
    ASSERT_INVARIANTS_BUF

string InputView::render( int           abs_cursor_pos,
                          string const& buffer ) {

    int pos = abs_cursor_pos;

    // First update internal state
    if( m_start_pos == (int)buffer.size() ||
        pos       <  m_start_pos          ||
        pos       >= m_start_pos+width )  {
        if( width & 1 ) {
            m_start_pos = pos - ((pos % width) % (width/2));
            if( m_start_pos == pos )
                m_start_pos -= (width/2 + 1);
            if( m_start_pos < 0 )
                m_start_pos = 0;
            ASSERT_INVARIANTS
        }
        else {
            m_start_pos = pos - (pos % (width/2));
            if( m_start_pos == pos )
                m_start_pos -= width/2;
            if( m_start_pos < 0 )
                m_start_pos = 0;
            ASSERT_INVARIANTS
        }
    }
    ASSERT_INVARIANTS

    // Now  render. First take all characters in the buffer after
    // the starting position, then cut  it  down  to a maximum of
    // the  width of the window, padding with spaces if necessary
    // since we must always return a string with precisely
    // `width` characters.
    string out( buffer.begin()+m_start_pos, buffer.end() );
    out.resize( width, ' ' );
    ASSERT( out.size() == size_t( width ) );
    return out;
}
