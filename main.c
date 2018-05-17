#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

#include "lib/card.h"
#include "lib/deck.h"
#include "lib/parser.h"
#include "lib/gui.h"
#include "lib/navigation.h"


int main(int argc, char *argv[]) {
    char deck_root[] = "/home/me/Documents/code/ccards/decks";
    Deck **decks = parse_decks(deck_root);

    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    GUI *gui = new_gui(decks);
    gui->active_window = gui->navigation;
    render_gui(gui);
    render_content(gui, gui->active_card, HIDE_ANSWER, gui->content_buffer);
    wrefresh_all(gui);

    int input_char = ' ';
    while((input_char = wgetch(gui->active_window)) != EXIT_CHAR) {
        // Switch focus from navigation <-> content window.
        if(input_char == '\t') {
            if(gui->active_window == gui->navigation) {
                gui->active_window = gui->content;
            }
            else {
                gui->active_window = gui->navigation;
            }
            render_gui(gui);
            render_content(gui, gui->active_card, HIDE_ANSWER, gui->content_buffer);
            wrefresh_all(gui);
            continue;
        }

        // Handle menu navigation mapping.
        if(gui->active_window == gui->navigation) {
            switch(input_char) {
                case KEY_DOWN:
                case 'j':
                    navigation_driver(gui, REQ_DOWN_ITEM);
                    break;
                case KEY_UP:
                case 'k':
                    navigation_driver(gui, REQ_UP_ITEM);
                    break;
            }
        }
        else { // Handle the content display.
            switch(input_char) {
                case 'l':
                    content_driver(gui, REQ_NEXT_CARD);
                    break;
                case 'h':
                    content_driver(gui, REQ_PREV_CARD);
                    break;
                case 'r':
                    content_driver(gui, REQ_RAND_CARD);
                    break;
                case ' ':
                    content_driver(gui, REQ_TOGGLE_ANSWER);
                    break;
                default:
                    continue;
            }
        }
        render_gui(gui);
        refresh();
        render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
        wrefresh_all(gui);
    }

    free_gui(gui);
    for(int i = 0; decks[i] != NULL; i++) {
        if(decks[i] != NULL) {
            free_deck(decks[i]);
        }
    }
    endwin();
    return 0;
}
