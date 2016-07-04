#include <ncurses.h>
#include <string>

class Input
{
public:
    Input(int width);
    bool key_press( int pressed );
    std::string const& get_string() const { return buffer; }
    void paste( std::string const& new_string );
    int get_cursor() const;
    void draw( int y, int x ) const;
    void clear();
private:
    void update_start();
    int width;
    std::string buffer;
    int start_pos;
    int pos;
};
