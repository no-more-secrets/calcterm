#include <string>

/****************************************************************
* KeyboardInput
****************************************************************/
class KeyboardInput {

public:

    KeyboardInput();

    bool operator==( KeyboardInput const& rhs ) {
        return buffer == rhs.buffer && pos == rhs.pos;
    }

    std::string const& get_buffer() const { return buffer; }
    int                get_pos()    const { return pos; }

    void               clear();

    void               key_press( bool        ctrl,
                                  bool        alt,
                                  int         pressed,
                                  char const* name );
private:

    std::string buffer;
    int         pos;

};
