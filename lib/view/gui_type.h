#ifndef __GUI_TYPE_H
#define __GUI_TYPE_H

#include <menu.h>

/* GUI layout:
**********************************
* NAV     | CONTENT              *
*         |                      *
* deck 1  |                      *
* deck 2  |                      *
* deck 3  |                      *
* ...     |                      *
*_________|______________________*
* FOOTER                         *
**********************************
 */

typedef struct GUI {
    WINDOW *navigation; // The sidebar which contains a list of available decks.
    WINDOW *content; // The main panel on which the cards are displayed.
    WINDOW *footer; // A small area at the bottom of navigation and content.
    int max_height;
    int max_width;
    int num_menu_items;
    ITEM **menu_items;
    MENU *menu;
} GUI;

#endif
