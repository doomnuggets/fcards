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

    initscr();
	clear();
	noecho();
	cbreak();
    start_color();

    GUI *gui = new_gui();
    draw_gui(gui);
    render_navigation(gui, decks);
    render_content(gui, NULL);
    render_footer(gui);
    wrefresh_all(gui);

    char input_char = ' ';
    WINDOW *active_window = gui->navigation;
    wmove(active_window, 1, 1);
    while(input_char != EXIT_CHAR) {
        // Switch focus from navigation <-> content window.
        input_char = wgetch(active_window);
        if(input_char == '\t') {
            if(active_window == gui->navigation) {
                active_window = gui->content;
                wmove(active_window, 1, 1);
            }
            else {
                active_window = gui->navigation;
                wmove(active_window, 1, CONTENT_START_X+1);
            }
            wrefresh(active_window);
            continue;
        }
        else if(input_char == 'q') {
            break;
        }

        // Handle menu navigation mapping
        if(active_window == gui->navigation) {

        }
        else { // Handle content.

        }
    }


    endwin();
    free_gui(gui);
    free_deck(deck);
    free(deck_path);
    return 0;
}
