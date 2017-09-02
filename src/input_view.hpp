#include <string>

class LineEditor;

/****************************************************************
* InputView
****************************************************************/
class InputView {

    int         start_pos;

public:

    int const   width;

    InputView( int w ) : start_pos( 0 ), width( w ) {}

    int         get_cursor( LineEditor const& le ) const;

    std::string render( LineEditor const& le );

};
