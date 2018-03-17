/****************************************************************
* LineEditor: class that accepts a stream of key presses and uses
* them to modify an internal string buffer.
****************************************************************/
#pragma once

#include <string>

class LineEditor {

public:

    using buf_t = std::string;

private:

    buf_t m_buffer = buf_t();
    int   m_pos    = 0;

public:

    buf_t const& buffer() const { return m_buffer; }
    int          pos()    const { return m_pos;    }

    void input( bool        ctrl,
                bool        alt,
                int         pressed,
                char const* name );
};
