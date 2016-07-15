//// Fancy ncurses terminal for calculators
//// Author: David P. Sicilia, June 2016

#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "assert.hpp"
#include "scope_exit.hpp"
#include "input.hpp"

#include "icalc/icalc.h"

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
    ASSERT( s.x >= 0 )
    ASSERT( s.y >= 0 )
    auto out = std::vector<std::string>( s.y+2 );
    int start_x = s.is_left ? 1 : width-1-s.x;
    ASSERT( start_x >= 0 )
    auto pad = std::string( start_x, ' ' );
    ASSERT( width-start_x-s.x >= 0 )
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
        ASSERT( start_y-text.size()+i >= 0 )
        mvprintw( start_y-text.size()+i, 0, text[i-1].c_str() );
    }
    if( highlight ) attroff( A_REVERSE );
}

auto draw_stripes( int highlight, std::vector<Stripe> const& v ) -> void
{
    int height = 0, width = 0;
    getmaxyx( stdscr, height, width );
    (void)height;
    int start_y = height-3;
    ASSERT( start_y >= 0 )
    int highlight_j = v.size() - highlight;
    ASSERT( highlight_j >= 0 )
    for( int j = start_y; j >= 0; --j )
        mvhline( j, 0, ' ', width );
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

#define GET_FUNC_SYM( sym )                                         \
    sym ## _t sym;                                                  \
    *(void**)(&sym) = dlsym( calc, #sym );                          \
    char const* STRING_JOIN( error__, __LINE__ ) = dlerror();       \
    if( STRING_JOIN( error__, __LINE__) ) {                         \
        fprintf( stderr, "%s\n", STRING_JOIN( error__, __LINE__) ); \
        return 1;                                                   \
    }

int _main(int argc, char* argv[])
{
    char const* deflibname = "src/defcalc/libdefcalc.so";
    char const* libname;
    if( argc == 1 )
        libname = deflibname;
    else
        libname = argv[1];
    printf("Loading library: %s\n", libname);
    void* calc = dlopen( libname, RTLD_LAZY );
    if( !calc ) {
        fprintf( stderr, "Unable to load library %s\n", libname );
        return 1;
    }
    SCOPE_EXIT( dlclose( calc ) )

    GET_FUNC_SYM( CI_init )
    GET_FUNC_SYM( CI_submit )
    GET_FUNC_SYM( CI_result_release )

    CI_init( NULL );

    initscr();
    SCOPE_EXIT( endwin() )
    raw();
    nonl();
    noecho();
    keypad(stdscr, TRUE);
    draw_stripes( -1, vs );
    int ch;
    int highlight = -1;
    int height = 0, width = 0;
    getmaxyx( stdscr, height, width );
    Input in( width-2 );
    bool editing = true, update_stripes = true;
    mvhline( height-2, 0, ACS_S7, width );
    move( height-1, 1 );
    while( (ch = getch()) != (int)'q' )
    {
        char const* name = keyname( ch );
        ASSERT( strlen( name ) > 0 )
        bool ctrl = (name[0] == '^' && strlen( name ) > 1);
        mvprintw( 0, 0, "%x         ", ch );
        mvprintw( 1, 0, "%s         ", name );
        mvprintw( 2, 0, "%d         ", width );
        //ASSERT( (char)(ch & 0xff) != 'K' )
        if( ch == KEY_UP || (ctrl && name[1] == 'K') ) {
            highlight += 1;
            editing = false;
            update_stripes = true;
        }
        else if ( ch == KEY_DOWN || (ctrl && name[1] == 'J') ) {
            highlight -= 1;
            if( highlight < -1 )
                highlight = -1;
            if( highlight == -1 )
                editing = true;
            update_stripes = true;
        }
        else if( ctrl && name[1] == 'L' ) {
            if( editing ) {
                vs.clear();
                update_stripes = true;
            }
        }
        else if( ch == '\n' || ch == '\r' ) {
            if( editing ) {
                std::string const& str = in.get_string();
                if( !str.empty() ) {
                    CI_Result* res = CI_submit( str.c_str() );
                    if( res ) {
                        SCOPE_EXIT( CI_result_release( res ) )
                        ASSERT( res->y > 0 )
                        std::string one_line( res->one_line );
                        Stripe s({ {one_line,{one_line}}, true, (int)1, (int)one_line.length() });
                        vs.push_back( s );
                        std::vector<std::string> grid;
                        size_t length_0 = strlen( res->grid[0] );
                        for( int j = 0; j < res->y; j++ ) {
                            ASSERT( strlen( res->grid[j] ) == length_0 )
                            grid.push_back( std::string( res->grid[j] ) );
                        }
                        Stripe s2({ {str,grid}, false, (int)res->y, (int)grid[0].length() });
                        vs.push_back( s2 );
                        in.clear();
                        update_stripes = true;
                    }
                }
            }
            else {
                std::string to_insert = vs[vs.size() - highlight - 1].e.one_line;
                in.paste( to_insert );
                highlight = -1;
                editing = true;
                update_stripes = true;
            }
        }
        else {
            if( editing )
                in.key_press( ctrl, false, ch, name );
        }

        if( update_stripes ) {
            draw_stripes( highlight, vs );
            update_stripes = false;
        }
        in.draw( height-1, 1 );
        if( editing ) {
            curs_set(1);
            move( height-1, 1+in.get_cursor() );
        }
        else {
            curs_set(0);
        }
        refresh();
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
