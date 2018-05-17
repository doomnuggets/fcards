#ifndef __MENU_BUILDER_H
#define __MENU_BUILDER_H

#include <stdlib.h>
#include <ncurses.h>
#include <menu.h>

#include "util.h"
#include "constants.h"
#include "deck.h"
#include "gui_type.h"


// Convert an array of decks into menu items.
void convert_to_menu_items(Deck **decks, ITEM **menu_items, int num_items) {
    for(int i = 0; i < num_items; i++) {
        menu_items[i] = new_item(decks[i]->name, NULL);
    }
    menu_items[num_items] = 0;
}

// Construct the menu with the passed items.
void build_menu(GUI *gui, ITEM **items) {
    gui->menu = new_menu(items);
    if(gui->menu == NULL) {
        perror("Failed to build menu.");
        return;
    }
    set_menu_win(gui->menu, gui->navigation);
    set_menu_sub(gui->menu, derwin(gui->navigation, 0, 0, 1, 1));
    set_menu_fore(gui->menu, COLOR_PAIR(2));
    set_menu_mark(gui->menu, "");
    post_menu(gui->menu);
}


#endif
