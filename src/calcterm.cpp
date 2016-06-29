//// Fancy ncurses terminal for calculators
//// Author: David P. Sicilia, June 2016

#include <ncurses.h>

int main()
{
    initscr();
    printw("Hello, World");
    getch();
    endwin();
    return 0;
}
