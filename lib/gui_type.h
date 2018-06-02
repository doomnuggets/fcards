#ifndef __GUI_TYPE_H
#define __GUI_TYPE_H

#include <menu.h>
#include "constants.h"
#include "deck.h"
#include "card.h"

/* GUI layout:
,--------------------------------,
| NAV     | CONTENT              |
|         |                      |
| deck 1  |                      |
| deck 2  |                      |
| deck 3  |                      |
| ...     |                      |
|         |                      |
*--------------------------------*
 */

typedef struct GUI {
    int max_height;
    int max_width;
    int num_menu_items;
    int answer_state;
    char *content_buffer;
    WINDOW *navigation;
    WINDOW *content;
    WINDOW *active_window;
    Deck *active_deck;
    Deck **decks;
    Card *active_card;
    MENU *menu;
    ITEM **menu_items;
} GUI;

#endif
