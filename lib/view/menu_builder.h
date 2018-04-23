#ifndef __MENU_BUILDER_H
#define __MENU_BUILDER_H

#include <stdlib.h>
#include <menu.h>

#include "../util.h"
#include "../constants.h"
#include "../deck.h"
#include "gui_type.h"


ITEM **convert_to_menu_items(Deck **decks) {
    int num_of_items = ARRAY_SIZE(decks);
    ITEM **items = (ITEM **)calloc(num_of_items + 1, sizeof(ITEM *));
    if(items == NULL) {
        perror("convert_to_items: Failed to allocate enough memory for all deck ITEMs.");
        return NULL;
    }
    for(int i = 0; i < num_of_items; i++) {
        items[i] = new_item(decks[i]->name, decks[i]->name);
    }
    return items;
}

MENU *build_menu(GUI *gui, ITEM **items) {
    MENU *menu = new_menu(items);
    set_menu_win(menu, gui->navigation);
    WINDOW *sub_window = derwin(gui->navigation, gui->max_height, NAVIGATION_MAX_WIDTH - 1, 1, 1);
    set_menu_sub(menu, sub_window);
    return menu;
}


#endif
