#include "input_view.hpp"
#include "line_editor.hpp"
#include "scope_exit.hpp"
#include "assert.hpp"

using namespace std;

#define ASSERT_INVARIANTS                                         \
    ASSERT( width     > 0 );                                      \
                                                                  \
    ASSERT( start_pos >= 0 );                                     \
    ASSERT( input->get_buffer().length() == 0 ||                  \
            (start_pos < (int)input->get_buffer().length()) );    \
                                                                  \
    ASSERT( ((width & 1) && (start_pos % width == 0            || \
                             start_pos % width == (width-1)    || \
                             start_pos % width == (width/2)    || \
                             start_pos % width == (width/2)-1))   \
        || (!(width & 1) && (start_pos % (width/2) == 0)) );      \
                                                                  \
    ASSERT( input->get_pos() >= start_pos );                      \
    ASSERT( input->get_pos() <  start_pos+width );

InputView::InputView( LineEditor const* input, int w )
    : input( input )
    , width(w)
    , start_pos( 0 ) {
    ASSERT_INVARIANTS
}

int InputView::get_cursor() const {
    return input->get_pos()-start_pos;
}

string InputView::render() {
    int pos = input->get_pos();

    // First update internal state
    if( start_pos == (int)input->get_buffer().size() ||
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
    string out = (char const*)&input->get_buffer()[start_pos];
    int residual = input->get_buffer().length()-start_pos;
    residual = (residual >= width) ? 0 : width-residual;
    out += string( residual, ' ' );
    return out;
}
