/****************************************************************
* InputView: represents a movable window into a string. It  takes
* a string and renders a slice of it depending on the  "location"
* of the window. This is not simply slicing a string though; this
* class must maintain internal state, because the movement of the
* window is intended to follow the movement of a cursor  position
* (which  must  also  be  supplied  to render the window) and the
* movement of the window is dependent not  only  on  the  current
* cursor position but also on the history of previous  cursor  po-
* sitions. This is required in order to deliver a natural  experi-
* ence  that  a  user  would expect when a string is navigated by
* moving a cursor along a line.
****************************************************************/
#pragma once

#include <string>

class InputView {

    int m_start_pos = 0;

public:

    // Size of the window.
    int const width;

    InputView( int w ) : width( w ) {}

    // Given  an  absolute  cursor  position, return the distance
    // from the start of the viewing window.
    int rel_pos( int p ) const { return p-m_start_pos; }

    // Will always return a string whose length is == width. This
    // function  cannot be const because in general it must be al-
    // lowed to mutate the InputView's starting position  (state).
    // The InputView must maintain this state because  there  are
    // actually multiple ways  that  the  InputView  could be ren-
    // dered given the same abs_cursor_pos  input,  and  the  one
    // that  is  chosen  it  affected  by previous renderings and
    // cursor positions. This is to  allow a natural editing expe-
    // rience that a user would expect where the cursor can  move
    // from  the  start  to  end of a line and only moving to the
    // next frame when the cursor touches one of the  extremities.
    std::string render( int                abs_cursor_pos,
                        std::string const& buffer );
};
