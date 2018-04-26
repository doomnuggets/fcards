#ifndef __GUI_TYPE_H
#define __GUI_TYPE_H

#include <menu.h>
#include "../deck.h"
#include "../card.h"

/* GUI layout:
,--------------------------------,
| NAV     | CONTENT              |
|         |                      |
| deck 1  |                      |
| deck 2  |                      |
| deck 3  |                      |
| ...     |                      |
|         |                      |
|_________|______________________|
| FOOTER                         |
*--------------------------------*
 */

typedef struct GUI {
    int max_height;
    int max_width;
    int num_menu_items;
    int answer_state;
    char *content_buffer;
    WINDOW *navigation; // The sidebar which contains a list of available decks.
    WINDOW *content; // The main panel on which the cards are displayed.
    WINDOW *footer; // A small area at the bottom of navigation and content.
    WINDOW *active_window;
    Deck *active_deck;
    Card *active_card;
    MENU *menu;
    ITEM **menu_items;
} GUI;

#endif
