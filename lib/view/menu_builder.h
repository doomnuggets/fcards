#ifndef __MENU_BUILDER_H
#define __MENU_BUILDER_H

#include <stdlib.h>
#include <menu.h>

#include "../util.h"
#include "../constants.h"
#include "../deck.h"
#include "gui_type.h"


// Convert an array of decks into menu items.
void convert_to_menu_items(Deck **decks, ITEM **menu_items) {
    int num_of_items = ARRAY_SIZE(decks);
    for(int i = 0; i < num_of_items; i++) {
        menu_items[i] = new_item(decks[i]->name, decks[i]->name);
    }
}

// Construct the menu with the passed items.
void build_menu(GUI *gui, ITEM **items) {
    items = NULL;
    gui->menu = new_menu(items);
    if(gui->menu == NULL) {
        perror("Failed to build menu.");
        return;
    }
    set_menu_win(gui->menu, gui->navigation);
}


#endif
