#include "input_view.hpp"
#include "scope_exit.hpp"
#include "assert.hpp"

using namespace std;

#define ASSERT_INVARIANTS_BUF                                     \
    ASSERT( (buffer.length() == 0 && start_pos == 0) ||           \
            (start_pos < (int)buffer.length()) );

#define ASSERT_INVARIANTS_POS                                     \
    ASSERT( width     > 0 );                                      \
    ASSERT( start_pos >= 0 );                                     \
    ASSERT( ((width & 1) && (start_pos % width == 0            || \
                             start_pos % width == (width-1)    || \
                             start_pos % width == (width/2)    || \
                             start_pos % width == (width/2)-1))   \
        || (!(width & 1) && (start_pos % (width/2) == 0)) );      \
    ASSERT( abs_pos >= start_pos );                               \
    ASSERT( abs_pos <  start_pos+width );

#define ASSERT_INVARIANTS                                         \
    ASSERT_INVARIANTS_POS                                         \
    ASSERT_INVARIANTS_BUF

int InputView::rel_pos( int abs_pos ) const {
    ASSERT_INVARIANTS_POS
    return abs_pos-start_pos;
}

string InputView::render( int abs_pos, string const& buffer ) {
    int pos = abs_pos;

    // First update internal state
    if( start_pos == (int)buffer.size() ||
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

    // Now  render. First take all characters in the buffer after
    // the starting position, then cut  it  down  to a maximum of
    // the  width of the window, padding with spaces if necessary
    // since we must always return a string with precisely
    // `width` characters.
    string out( buffer.begin()+start_pos, buffer.end() );
    out.resize( width, ' ' );
    ASSERT( out.size() == size_t( width ) );
    return out;
}
