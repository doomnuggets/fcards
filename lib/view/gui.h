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
    initscr();
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
    wrefresh(gui->navigation);
    wrefresh(gui->content);
    wrefresh(gui->footer);
    wrefresh(stdscr);
}

//// Resize all windows. Used when the terminal changes it's dynamics.
// Signal handler SIGWCH
void resize(GUI *gui) {
    // TODO
    wrefresh_all(gui);
}

/// Render all loaded decks in the navigation window.
void render_navigation(GUI *gui, Deck **decks) {
    if(decks == NULL) {
        wprintw(gui->navigation, 0, 0, "No decks found.");
        wrefresh(gui->navigation);
        return;
    }
}

/// Render a card in the content window.
void render_content(GUI *gui, Card *card) {
    if(card == NULL) {
        wprintw(gui->content, 0, 0, "Deck is empty.");
        wrefresh(gui->content);
        return;
    }
}

#endif
