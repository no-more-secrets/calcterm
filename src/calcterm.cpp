//// Fancy ncurses terminal for calculators
//// Author: David P. Sicilia, June 2016

#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>

struct Entry
{
    std::string one_line;
    std::vector<std::string> grid;
};

struct Stripe
{
    Entry e;
    bool is_left;
    int y;
    int x;
};

auto render_stripe( int width, Stripe const& s ) -> std::vector<std::string>
{
    auto out = std::vector<std::string>( s.y+2 );
    int start_x = s.is_left ? 1 : width-1-s.x;
    auto pad = std::string( start_x, ' ' );
    auto end_pad = std::string( width-start_x-s.x, ' ' );
    for( int i = 0; i < s.y; ++i )
        out[i+1] = pad + s.e.grid[i] + end_pad; 
    out[0]     = pad + std::string( s.x, ' ' ) + end_pad;
    out[s.y+1] = pad + std::string( s.x, ' ' ) + end_pad;
    return out;
}

auto draw_stripe( int width, int start_y, bool highlight, Stripe const& s ) -> void
{
    auto text = render_stripe( width, s );
    for( int i = text.size(); i > 0; --i ) {
        if( start_y-i < 0 )
            break;
        if( highlight ) attron( A_REVERSE );
        mvprintw( start_y-text.size()+i, 0, text[i-1].c_str() );
        attroff( A_REVERSE );
    }
}

auto draw_stripes( int highlight, std::vector<Stripe> v ) -> void
{
    int height = 0, width = 0;
    getmaxyx( stdscr, height, width );
    (void)height;
    int start_y = height-2;
    int highlight_j = v.size() - highlight;
    for( int j = v.size(); j > 0; --j ) {
        if( start_y < 0 )
            break;
        Stripe const& s = v[j-1];
        bool highlight = (j == highlight_j);
        draw_stripe( width, start_y, highlight, s );
        start_y -= (s.y+2);
    }
}

std::vector<std::string> s1 = {
    "  1 + 2  ",
    "---------",
    "      5  ",
    " 3 + --- ",
    "      6  "
};

std::vector<std::string> s2 = {
    "     1     0.002     r    ",
    "    --- + ------- + 7     ",
    "     x       y            ",
    "--------------------------",
    "                        w ",
    " /                     \\  ",
    " |     1     4 + t     |  ",
    " |1 + --- + ------- + y|  ",
    " |     6       y       |  ",
    " \\                     /  "
};

std::vector<std::string> s3 = {
    "                                        -w",
    "/                  \\ /                 \\  ",
    "| 1       1       r| | t + 4         7 |  ",
    "|--- + ------- + 7 |*|------- + y + ---|  ",
    "| x     y*500      | |   y           6 |  ",
    "\\                  / \\                 /  "
};

std::vector<Stripe> vs = {
    { {"",s2}, false,  (int)s2.size(), (int)s2[0].length() },
    { {"",s3}, true,   (int)s3.size(), (int)s3[0].length() },
    { {"",s1}, false,  (int)s1.size(), (int)s1[0].length() },
    { {"",s1}, true,   (int)s1.size(), (int)s1[0].length() },
    { {"",s3}, false,  (int)s3.size(), (int)s3[0].length() },
    { {"",s2}, true,   (int)s2.size(), (int)s2[0].length() },
    { {"",s2}, false,  (int)s2.size(), (int)s2[0].length() },
    { {"",s1}, true,   (int)s1.size(), (int)s1[0].length() },
    { {"",s1}, false,  (int)s1.size(), (int)s1[0].length() },
    { {"",s2}, true,   (int)s2.size(), (int)s2[0].length() },
    { {"",s3}, false,  (int)s3.size(), (int)s3[0].length() },
    { {"",s3}, true,   (int)s3.size(), (int)s3[0].length() },
    { {"",s1}, false,  (int)s1.size(), (int)s1[0].length() },
};

int main()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    draw_stripes( -1, vs );
    int ch;
    int highlight = -1;
    int height = 0, width = 0;
    getmaxyx( stdscr, height, width );
    while( (ch = getch()) != (int)'q' )
    {
        switch( ch )
        {
            case KEY_UP:   highlight += 1; break;
            case KEY_DOWN: highlight -= 1; break;
        };
        draw_stripes( highlight, vs );
        move( height-1, 0 );
        //refresh();
    }
    endwin();
    return 0;
}
