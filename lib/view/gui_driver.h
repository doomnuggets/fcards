#include "gui_type.h"
#include "gui.h"
#include "../constants.h"

// Handle the content window navigation.
// Handles the navigation of cards and their content.
void content_driver(GUI *gui, const int action) {
    if(gui->active_deck == NULL) {
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
}
