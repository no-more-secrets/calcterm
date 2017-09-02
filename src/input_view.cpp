#include "input_view.hpp"
#include "line_editor.hpp"
#include "scope_exit.hpp"
#include "assert.hpp"

using namespace std;

#define ASSERT_INVARIANTS                                         \
    ASSERT( width     > 0 );                                      \
                                                                  \
    ASSERT( start_pos >= 0 );                                     \
    ASSERT( (le.get_buffer().length() == 0 && start_pos == 0) ||  \
            (start_pos < (int)le.get_buffer().length()) );        \
                                                                  \
    ASSERT( ((width & 1) && (start_pos % width == 0            || \
                             start_pos % width == (width-1)    || \
                             start_pos % width == (width/2)    || \
                             start_pos % width == (width/2)-1))   \
        || (!(width & 1) && (start_pos % (width/2) == 0)) );      \
                                                                  \
    ASSERT( le.get_pos() >= start_pos );                          \
    ASSERT( le.get_pos() <  start_pos+width );

int InputView::get_cursor( LineEditor const& le ) const {
    ASSERT_INVARIANTS
    return le.get_pos()-start_pos;
}

string InputView::render( LineEditor const& le ) {
    int pos = le.get_pos();

    // First update internal state
    if( start_pos == (int)le.get_buffer().size() ||
        pos       <  start_pos          ||
        pos       >= start_pos+width )  {
        if( width & 1 ) {
            start_pos = pos - ((pos % width) % (width/2));
            if( start_pos == pos )
                start_pos -= (width/2 + 1);
            if( start_pos < 0 )
                start_pos = 0;
            ASSERT_INVARIANTS
        }
        else {
            start_pos = pos - (pos % (width/2));
            if( start_pos == pos )
                start_pos -= width/2;
            if( start_pos < 0 )
                start_pos = 0;
            ASSERT_INVARIANTS
        }
    }
    ASSERT_INVARIANTS

    // Now render
    string out = (char const*)&le.get_buffer()[start_pos]; // ?! ?! safe? correct? FIXME
    int residual = le.get_buffer().length()-start_pos;
    residual = (residual >= width) ? 0 : width-residual;
    out += string( residual, ' ' );
    ASSERT( out.size() == size_t( width ) );
    return out;
}
