#include <string>

/****************************************************************
* InputView
****************************************************************/
class InputView {

    int         start_pos;

public:

    int const   width;

    InputView( int w ) : start_pos( 0 ), width( w ) {}

    int         rel_pos( int abs_pos ) const;

    std::string render( int abs_pos, std::string const& buffer );

};
