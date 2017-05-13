#include <string>

class KeyboardInput;

/****************************************************************
* InputView
****************************************************************/
class InputView {

    KeyboardInput const* input;
    int                  width;
    int                  start_pos;

public:

    InputView( KeyboardInput const* input, int width );

    int         get_width()  const { return width; }
    int         get_cursor() const;
    std::string render();

};
