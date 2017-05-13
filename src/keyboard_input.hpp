#include <string>

/****************************************************************
* KeyboardInput
****************************************************************/
class KeyboardInput {

    std::string buffer;
    int         pos;

public:

    KeyboardInput();
    KeyboardInput( std::string const& s, int p );

    std::string const& get_buffer() const { return buffer; }
    int                get_pos()    const { return pos; }

    void               clear();

    void               key_press( bool        ctrl,
                                  bool        alt,
                                  int         pressed,
                                  char const* name );

};
