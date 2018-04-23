#ifndef __VIEW_H
#define __VIEW_H

#include <ncurses.h>
#include <menu.h>
#include <signal.h>

#include "../constants.h"
#include "../deck.h"
#include "gui_type.h"
#include "ncurses_util.h"
#include "menu_builder.h"

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


/// Deinitialize ncurses and free all windows and the created GUI structure.
void free_gui(GUI *gui) {
    destroy_win(gui->navigation);
    destroy_win(gui->content);
    destroy_win(gui->footer);
    unpost_menu(gui->menu);
    free_menu(gui->menu);
    for(int i = 0; i < gui->num_menu_items; i++) {
        free_item(gui->menu_items[i]);
    }
    free(gui);
}

/// Prepare the GUI structure and return a pointer to it.
GUI *new_gui(Deck **decks) {
    GUI *gui = malloc(sizeof(GUI));
    if(gui == NULL) {
        perror("new_gui: Failed to mallocate the GUI.");
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

    gui->num_menu_items = ARRAY_SIZE(decks);
    gui->menu_items = convert_to_menu_items(decks);
    gui->menu = build_menu(gui, gui->menu_items);

    return gui;
}


void draw_gui(GUI *gui) {
    int x;
    int y;
    getyx(stdscr, y, x);

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // horizontal blue bar.
    init_pair(2, COLOR_WHITE, COLOR_BLUE); // white text with blue background.
    init_pair(3, COLOR_BLUE, COLOR_BLACK); // vertical blue bar.

    // Draw the horizontal bar at the top.
    attron(COLOR_PAIR(1));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(1));

    // Draw the title.
    attron(COLOR_PAIR(2));
    mvwprintw(stdscr, 0, 1, "Decks");
    attroff(COLOR_PAIR(2));

    // Draw the vertical bar which separates navigation from content.
    move(1, CONTENT_START_X);
    attron(COLOR_PAIR(3));
    vline(0, gui->max_height - FOOTER_MAX_HEIGHT - 1);
    attroff(COLOR_PAIR(3));

    // Draw horizontal bar just above the footer.
    move(gui->max_height - FOOTER_MAX_HEIGHT - 1, 0);
    attron(COLOR_PAIR(1));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(1));

    move(y, x);
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
    draw_gui(gui);
    wrefresh_all(gui);
}

/// Render a card in the content window.
void render_content(GUI *gui, Card *card, int show_answer, char *content_buffer) {
    draw_gui(gui);
    if(card == NULL) {
        wprintw(gui->content, 0, 0, "Deck is empty.");
    }
    else {
        if(show_answer) {
            snprintf(content_buffer, MAX_CARD_CONTENT, "%s\n\n%s", card->question, card->answer);
            word_wrap(content_buffer, CONTENT_MAX_WIDTH-2);
        }
        else {
            strncpy(content_buffer, card->question, MAX_CARD_ANSWER);
            word_wrap(content_buffer, CONTENT_MAX_WIDTH - 2);
        }
        mvwprintw(gui->content, 1, NAVIGATION_MAX_WIDTH + 3, content_buffer);
    }
    wrefresh(gui->content);
}

#endif
