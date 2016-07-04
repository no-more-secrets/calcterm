//// Fancy ncurses terminal for calculators
//// Author: David P. Sicilia, June 2016

#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "scope_exit.hpp"
#include "input.hpp"

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
    if( highlight ) attron( A_REVERSE );
    for( int i = text.size(); i > 0; --i ) {
        if( start_y-i < 0 )
            break;
        mvprintw( start_y-text.size()+i, 0, text[i-1].c_str() );
    }
    if( highlight ) attroff( A_REVERSE );
}

auto draw_stripes( int highlight, std::vector<Stripe> const& v ) -> void
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

std::vector<std::string> s4 = { "1" };

std::vector<std::string> s5 = { "1.000000000000000000000000000000000000000000000000" };

std::vector<Stripe> vs = {
    { {"123",s2}, false,  (int)s2.size(), (int)s2[0].length() },
    { {"123",s3}, true,   (int)s3.size(), (int)s3[0].length() },
    { {"123",s1}, false,  (int)s1.size(), (int)s1[0].length() },
    { {"123",s1}, true,   (int)s1.size(), (int)s1[0].length() },
    { {"123",s3}, false,  (int)s3.size(), (int)s3[0].length() },
    { {"123",s1}, true,   (int)s1.size(), (int)s1[0].length() },
    { {"123",s2}, false,  (int)s2.size(), (int)s2[0].length() },
    { {"123",s1}, true,   (int)s1.size(), (int)s1[0].length() },
    { {"123",s1}, false,  (int)s1.size(), (int)s1[0].length() },
    { {"123",s2}, true,   (int)s2.size(), (int)s2[0].length() },
    { {"123",s3}, false,  (int)s3.size(), (int)s3[0].length() },
    { {"123",s3}, true,   (int)s3.size(), (int)s3[0].length() },
    { {"123",s1}, false,  (int)s1.size(), (int)s1[0].length() },
    { {"123",s4}, true,   (int)s4.size(), (int)s4[0].length() },
    { {"123",s5}, false,  (int)s5.size(), (int)s5[0].length() },
};

int _main(int argc, char* argv[])
{
    initscr();
    SCOPE_EXIT( endwin() )
    //raw();
    //nonl();
    noecho();
    keypad(stdscr, TRUE);
    draw_stripes( -1, vs );
    int ch;
    int highlight = -1;
    int height = 0, width = 0;
    getmaxyx( stdscr, height, width );
    Input in( width-1 );
    bool editing = true;
    while( (ch = getch()) != (int)'q' )
    {
        switch( ch )
        {
            case KEY_UP:
                highlight += 1;
                editing = false;
                break;
            case KEY_DOWN:
                highlight -= 1;
                if( highlight == -1 )
                    editing = true;
                break;
            case '\n': {
                if( editing ) {
                    std::string const& str = in.get_string();
                    if( !str.empty() ) {
                        Stripe s({ {str,{str}}, true, (int)1, (int)str.length() });
                        vs.push_back( s );
                        in.clear();
                    }
                }
                else {
                    std::string to_insert = vs[vs.size() - highlight - 1].e.one_line; 
                    in.paste( to_insert );
                    highlight = -1;
                    editing = true;
                }
                break;
            }
            default:
                if( editing )
                    in.key_press( ch );
                break;
        };
        draw_stripes( highlight, vs );
        in.draw( height-1, 0 );
        move( height-1, 0+in.get_cursor() );
        //refresh();
    }
    return 0;
}

int main( int argc, char* argv[] )
{
    try {
        return _main(argc, argv);
    } catch( std::exception const& e ) {
        std::cout << "exception:" << e.what() << std::endl;
        return 1;
    }
}
