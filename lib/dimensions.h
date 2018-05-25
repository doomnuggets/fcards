#ifndef __DIMENSIONS_H
#define __DIMENSIONS_H

#include "gui_type.h"


// 1920x1080 dimensions:
// fullscreen:
//      Max Y: 56
//      Max X: 213
//
// halfscreen (vertical):
//      Max Y: 56
//      Max X: 106
//
// halfscreen (horizontal):
//      Max Y: 28
//      Max X: 213
//

int NAVIGATION_WIDTH = 0;
int NAVIGATION_HEIGHT = 0;

int CONTENT_WIDTH = 0;
int CONTENT_HEIGHT = 0;

int FOOTER_WIDTH = 0;
int FOOTER_HEIGHT = 2;

void recalculate_dimensions(GUI *gui) {
    getmaxyx(stdscr, gui->max_height, gui->max_width);
    NAVIGATION_WIDTH = (int)(gui->max_width / 4);
    CONTENT_WIDTH = (int)(((gui->max_width / 4) * 3) - CONTENT_PADDING_X);
    FOOTER_WIDTH = (int)(gui->max_width);
    NAVIGATION_HEIGHT = (int)(gui->max_height - FOOTER_HEIGHT - 2);
    CONTENT_HEIGHT = (int)(gui->max_height - FOOTER_HEIGHT - 2);
}

#endif
