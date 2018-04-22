#ifndef __VIEW_H
#define __VIEW_H

#include <ncurses.h>
#include <menu.h>
#include <signal.h>

#include "../constants.h"
#include "../deck.h"
#include "ncurses_util.h"

/* GUI layout:
**********************************
* NAV     | CONTENT              *
*         |                      *
* deck 1  |                      *
* deck 2  |                      *
* deck 3  |                      *
* ...     |                      *
*_________|______________________*
* FOOTER                         *
**********************************
 */

typedef struct GUI {
    WINDOW *navigation; // The sidebar which contains a list of available decks.
    WINDOW *content; // The main panel on which the cards are displayed.
    WINDOW *footer; // A small area at the bottom of navigation and content.
    int max_height;
    int max_width;
} GUI;

/// Deinitialize ncurses and free all windows and the created GUI structure.
void free_gui(GUI *gui) {
    destroy_win(gui->navigation);
    destroy_win(gui->content);
    destroy_win(gui->footer);
    free(gui);
    endwin();
}

/// Prepare the GUI structure and return a pointer to it.
GUI *new_gui() {
    GUI *gui = malloc(sizeof(GUI));
    if(gui == NULL) {
        perror("Failed to initialize the GUI. (malloc)");
        return NULL;
    }

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    gui->max_height = max_height;
    gui->max_width = max_width;
    gui->navigation = newwin(max_height - FOOTER_MAX_HEIGHT, NAVIGATION_MAX_WIDTH, 0, 0);
    if(gui->navigation == NULL) {
        perror("Failed to initialize navigation window.");
        free_gui(gui);
        return NULL;
    }

    gui->content = newwin(max_height - FOOTER_MAX_HEIGHT, CONTENT_MAX_WIDTH, 0, NAVIGATION_MAX_WIDTH + 1);
    if(gui->content == NULL) {
        perror("Failed to initialize content window.");
        free_gui(gui);
        return NULL;
    }

    gui->footer = newwin(FOOTER_MAX_HEIGHT, max_width, max_height-FOOTER_MAX_HEIGHT, 0);
    if(gui->footer == NULL) {
        perror("Failed to initialize footer window.");
        free_gui(gui);
        return NULL;
    }

    return gui;
}

/// Refresh all windows.
void wrefresh_all(GUI *gui) {
    wrefresh(stdscr);
    wrefresh(gui->navigation);
    wrefresh(gui->content);
    wrefresh(gui->footer);
}

void draw_gui(GUI *gui) {
    int x;
    int y;
    getyx(stdscr, y, x);

    init_pair(1, COLOR_BLUE, COLOR_BLACK); // vertical blue line.
    init_pair(2, COLOR_BLUE, COLOR_BLUE); // horizontal blue bar.
    init_pair(3, COLOR_WHITE, COLOR_BLUE); // white text with blue background.

    // Draw the top most horizontal bar.
    wmove(stdscr, 0, 0);
    attron(COLOR_PAIR(2));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(2));

    // Draw the title.
    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "C-Cards v%s", VERSION);
    attroff(COLOR_PAIR(3));

    // Draw the vertical bar which separates navigation from content.
    wmove(stdscr, 1, CONTENT_START_X);
    attron(COLOR_PAIR(1));
    vline(0, gui->max_height - FOOTER_MAX_HEIGHT);
    attroff(COLOR_PAIR(1));

    // Draw the horizontal footer bar which separates navigation and content
    // from the footer.
    wmove(stdscr, gui->max_height - FOOTER_MAX_HEIGHT, 0);
    attron(COLOR_PAIR(2));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(2));

    move(y, x);
}


//// Resize all windows. Used when the terminal changes it's dynamics.
// Signal handler SIGWCH
void resize(GUI *gui) {
    // TODO
    draw_gui(gui);
    wrefresh_all(gui);
}

/// Render all loaded decks in the navigation window.
void render_navigation(GUI *gui, Deck **decks) {
    move(0, 0);
    if(decks == NULL) {
        wprintw(gui->navigation, 0, 0, "No decks found.");
    }
    else {
        // TODO: Actually build the menu for the deck selection.
        for(int y_coord = 1; y_coord < gui->max_height; y_coord++) {
            wmove(gui->navigation, y_coord, 2);
            wprintw(gui->navigation, "Deck 112312312");
        }
    }
    wrefresh(gui->navigation);
}

/// Render a card in the content window.
void render_content(GUI *gui, Card *card) {
    if(card == NULL) {
        wprintw(gui->content, 0, 0, "Deck is empty.");
    }
    else {
    }
    wrefresh(gui->content);
}

void render_footer(GUI *gui) {
    wrefresh(gui->footer);
}

#endif
