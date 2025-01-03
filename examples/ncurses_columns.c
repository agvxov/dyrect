// @BAKE gcc $@ -o $*.out -I../ $(pkg-config --cflags --libs ncurses) -lm
#include <stdio.h>
#include <ncurses.h>
#include "dyrect.h"

WINDOW * windows[5] = { NULL, NULL, NULL, NULL, NULL };

void do_resize(void) {
    rect_t parent = center(get_screen_rect(), scale(get_screen_rect(), 0.8));
    for (int i = 0; i < 5; i++) {
        delwin(windows[i]);
        // NOTE: we are actually redundantly recalculating,
        //        both for scaling and in the expansion of UNPACK;
        //       a temp var should be deployed;
        //       now, this is generally bad and should be avoided,
        //        however it *does work* and developer comfort is a top priority
        windows[i] = newwin(UNPACK(next(rfloor(scalex(parent, 0.2)), i)));
        refresh();
    }
    
    clear();
    wrefresh(stdscr);
}

signed main(void) {
    initscr();
    noecho();
    curs_set(0);
    halfdelay(1);

    do_resize();

    while(1) {
        switch (wgetch(stdscr)) {
            case KEY_RESIZE: do_resize();
        }

        for (int i = 0; i < 5; i++) {
            int maxy, maxx;
            getmaxyx(windows[i], maxy, maxx);
            for (int h = 0; h < (maxy * maxx); h++) {
                waddch(windows[i], '0' + i);
            }
            box(windows[i], ACS_VLINE, ACS_HLINE);
            wrefresh(windows[i]);
        }
    }

    endwin();
    return 0;
}
