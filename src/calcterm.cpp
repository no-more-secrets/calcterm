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

#include "icalcterm/icalcterm.h"

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
    int start_y = height-4;
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

std::vector<Stripe> vs = { };

#define GET_FUNC_SYM( sym )                                         \
    sym ## _t sym;                                                  \
    *(void**)(&sym) = dlsym( calc, #sym );                          \
    char const* STRING_JOIN( error__, __LINE__ ) = dlerror();       \
    if( STRING_JOIN( error__, __LINE__) ) {                         \
        fprintf( stderr, "%s\n", STRING_JOIN( error__, __LINE__) ); \
        return 1;                                                   \
    }

#ifdef OS_LINUX
char const* deflibname = "libdefcalc.so";
#else
#ifdef OS_OSX
char const* deflibname = "libdefcalc.dylib";
#endif
#endif

int _main(int argc, char* argv[])
{
    char const* libname;
    if( argc == 1 )
        libname = deflibname;
    else
        libname = argv[1];
    //printf("Loading library: %s\n", libname);
    void* calc = dlopen( libname, RTLD_NOW );
    if( !calc ) {
        fprintf( stderr, "Unable to load library %s:\n%s\n", libname, dlerror() );
        return 1;
    }
    SCOPE_EXIT( dlclose( calc ) )

    GET_FUNC_SYM( CI_init )
    GET_FUNC_SYM( CI_config )
    GET_FUNC_SYM( CI_submit )
    GET_FUNC_SYM( CI_result_free )

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
    mvhline( height-3, 1, ACS_HLINE, width-1 );
    mvaddch( height-3, 0, ACS_ULCORNER );
    mvaddch( height-3, width-1, ACS_URCORNER );
    mvaddch( height-2, 0, ACS_VLINE );
    mvaddch( height-2, width-1, ACS_VLINE );
    mvhline( height-1, 1, ACS_HLINE, width-1 );
    mvaddch( height-1, 0, ACS_LLCORNER );
    mvaddch( height-1, width-1, ACS_LRCORNER );
    move( height-2, 1 );
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
        else if( ch == '\n' || ch == '\r' || ch == '\\' ) {
            if( editing ) {
                bool approx = (ch == '\\');
                std::string const& str = in.get_string();
                if( !str.empty() ) {
                    CI_Result* res = CI_submit( str.c_str() );
                    if( res ) {
                        SCOPE_EXIT( CI_result_free( res ) )

                        auto output_grid = []( char* _one_line, char** _grid, int rows, bool left_right ) {
                            ASSERT( rows > 0 )
                            auto one_line = std::string( _one_line );
                            auto grid     = std::vector<std::string>();
                            auto length_0 = strlen( _grid[0] );
                            for( int j = 0; j < rows; j++ ) {
                                ASSERT( strlen( _grid[j] ) == length_0 )
                                grid.push_back( std::string( _grid[j] ) );
                            }
                            Stripe s({ {one_line, grid}, left_right, rows, (int)length_0 });
                            vs.push_back( s );
                        };

                        output_grid( res->input.one_line, res->input.grid, res->input.grid_rows, true );
                        if( approx && res->num_outputs > 1 ) {
                            output_grid( res->outputs[1].one_line, res->outputs[1].grid, res->outputs[1].grid_rows, false );
                        } else if( res->num_outputs > 0 ) {
                            output_grid( res->outputs[0].one_line, res->outputs[0].grid, res->outputs[0].grid_rows, false );
                        }

                        in.clear();
                        update_stripes = true;
                    }
                }
            }
            else {
                std::string to_insert = vs[vs.size() - highlight - 1].e.one_line;
                //to_insert = "(" + to_insert + ")";
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
        in.draw( height-2, 1 );
        if( editing ) {
            curs_set(1);
            move( height-2, 1+in.get_cursor() );
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
