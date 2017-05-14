#include <string>

class LineEditor;

/****************************************************************
* InputView
****************************************************************/
class InputView {

    LineEditor const* input;
    int               width;
    int               start_pos;

public:

    InputView( LineEditor const* input, int width );

    int         get_width()  const { return width; }
    int         get_cursor() const;
    std::string render();

};
