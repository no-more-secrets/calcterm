#include <string>

/****************************************************************
* LineEditor
****************************************************************/
class LineEditor {

    using buf_t  = std::string;
    buf_t buffer = buf_t();
    int pos      = 0;

public:

    buf_t const& get_buffer() const { return buffer; }
    int          get_pos()    const { return pos; }

    void         input( bool        ctrl,
                        bool        alt,
                        int         pressed,
                        char const* name );
};
