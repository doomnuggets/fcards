#ifndef __DRIVER_H
#define __DRIVER_H

#include "deck.h"
#include "gui_type.h"
#include "gui.h"
#include "constants.h"

// Handle the navigation of cards and their content.
void content_driver(GUI *gui, const int action) {
    if(gui->active_deck == NULL) {
        return;
    }
    if(gui->active_card == NULL) {
        return;
    }

    switch(action) {
        case REQ_TOGGLE_ANSWER:
            gui->answer_state = 1^gui->answer_state;
            render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
            memset(gui->content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
            break;
        case REQ_NEXT_CARD:
            if(gui->active_card->next != NULL) {
                gui->active_card = gui->active_card->next;
                gui->answer_state = HIDE_ANSWER;
                render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
                memset(gui->content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
            }
            break;
        case REQ_PREV_CARD:
            if(gui->active_card->prev != NULL) {
                gui->active_card = gui->active_card->prev;
                gui->answer_state = HIDE_ANSWER;
                render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
                memset(gui->content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
            }
            break;
        case REQ_RAND_CARD:
            gui->active_card = pick_random_card(gui->active_deck);
            gui->answer_state = HIDE_ANSWER;
            render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
            memset(gui->content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
            break;
    }
    wrefresh(gui->content);
}

// Handle the deck navigation.
void navigation_driver(GUI *gui, const int action) {
    if(gui->active_deck == NULL) {
        return;
    }
    if(gui->decks == NULL) {
        return;
    }

    menu_driver(gui->menu, action);
    gui->active_deck = gui->decks[item_index(current_item(gui->menu))];
    gui->active_card = gui->active_deck->top;
    render_content(gui, gui->active_card, HIDE_ANSWER, gui->content_buffer);
    memset(gui->content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
    wrefresh(gui->content);
    wrefresh(gui->navigation);
}

#endif
