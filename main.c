#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/card.h"
#include "lib/deck.h"
#include "lib/parser.h"
#include "lib/view/gui.h"


int main(int argc, char *argv[]) {
    char *deck_path = calloc(MAX_DECK_PATH, sizeof(char));
    strcpy(deck_path, "/home/me/Documents/code/ccards/decks/test/");
    Deck *deck = parse_deck(deck_path);

    Deck *decks[] = {deck, NULL};

    WINDOW *default_window = initscr();
	clear();
	noecho();
	cbreak();
    start_color();

    GUI *gui = new_gui(decks);
    draw_gui(gui);
    wrefresh_all(gui);

    int input_char = ' ';
    WINDOW *active_window = gui->navigation;
    wmove(active_window, 1, 1);
    Deck *active_deck = decks[0];
    Card *active_card = active_deck->top;
    int answer_hidden = 1;
    char *content_buffer = calloc(MAX_CARD_CONTENT, sizeof(char));
    while(input_char != EXIT_CHAR) {
        // Switch focus from navigation <-> content window.
        input_char = wgetch(active_window);
        if(input_char == '\t') {
            if(active_window == gui->navigation) {
                active_window = gui->content;
            }
            else {
                active_window = gui->navigation;
            }
            continue;
        }
        else if(input_char == 'q') {
            break;
        }

        // Handle menu navigation mapping
        if(active_window == gui->navigation) {
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
        else { // Handle content.
            switch(input_char) {
                case KEY_RIGHT:
                case 'l':
                    // Render the active_card->next card.
                    active_card = active_card->next;
                    render_content(gui, active_card, HIDE_ANSWER, content_buffer);
                    answer_hidden = 1;
                    memset(content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
                    break;
                case KEY_LEFT:
                case 'h':
                    // Render the active_card->prev card.
                    active_card = active_card->prev;
                    render_content(gui, active_card, HIDE_ANSWER, content_buffer);
                    answer_hidden = 1;
                    memset(content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
                    break;
                case 'r':
                    // Render a random card from the active_deck.
                    active_card = pick_random_card(active_deck);
                    render_content(gui, active_card, HIDE_ANSWER, content_buffer);
                    answer_hidden = 1;
                    memset(content_buffer, '\0', sizeof(char) * MAX_CARD_CONTENT);
                    break;
                case ' ':
                    // Toggle the display of active_card->answer.
                    render_content(gui, active_card, 1^answer_hidden, content_buffer);
                    answer_hidden = 1^answer_hidden;
                    break;
            }
        }
    }

    free(content_buffer);
    free_gui(gui);
    free_deck(deck);
    free(deck_path);
    delwin(default_window);
    delwin(stdscr);
    endwin();
    return 0;
}
