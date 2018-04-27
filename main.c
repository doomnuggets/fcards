#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/card.h"
#include "lib/deck.h"
#include "lib/parser.h"
#include "lib/view/gui.h"
#include "lib/view/gui_driver.h"


int main(int argc, char *argv[]) {
    char *deck_path = calloc(MAX_DECK_PATH, sizeof(char));
    strcpy(deck_path, "/home/me/Documents/code/ccards/decks/test/");
    Deck *deck = parse_deck(deck_path);
    Deck *decks[] = {deck, NULL};
    initscr();
	clear();
	noecho();
	cbreak();
    curs_set(0);
    start_color();

    GUI *gui = new_gui(decks);
    draw_gui(gui);
    render_navigation(gui);
    wrefresh_all(gui);

    int input_char = ' ';
    gui->active_deck = decks[0];
    gui->active_card = decks[0]->top;
    gui->active_window = gui->navigation;
    while(input_char != EXIT_CHAR) {
        // Switch focus from navigation <-> content window.
        input_char = wgetch(gui->active_window);
        if(input_char == '\t') {
            if(gui->active_window == gui->navigation) {
                gui->active_window = gui->content;
            }
            else {
                gui->active_window = gui->navigation;
            }
            draw_gui(gui);
            render_navigation(gui);
            render_content(gui, gui->active_card, HIDE_ANSWER, gui->content_buffer);
            wrefresh_all(gui);
            continue;
        }
        else if(input_char == EXIT_CHAR) {
            break;
        }

        // Handle menu navigation mapping.
        if(gui->active_window == gui->navigation) {
            switch(input_char) {
                case KEY_DOWN:
                case 'j':
                    menu_driver(gui->menu, REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                case 'k':
                    menu_driver(gui->menu, REQ_UP_ITEM);
                    break;
            }
        }
        else { // Handle the content display.
            switch(input_char) {
                case KEY_RIGHT:
                case 'l':
                    content_driver(gui, REQ_NEXT_CARD);
                    break;
                case KEY_LEFT:
                case 'h':
                    content_driver(gui, REQ_PREV_CARD);
                    break;
                case 'r':
                    content_driver(gui, REQ_RAND_CARD);
                    break;
                case ' ':
                    content_driver(gui, REQ_TOGGLE_ANSWER);
                    break;
            }
        }

        draw_gui(gui);
        render_navigation(gui);
        render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
        wrefresh_all(gui);
    }

    free_gui(gui);
    free_deck(deck);
    free(deck_path);
    endwin();
    return 0;
}
