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
*--------------------------------*
| NAV     | CONTENT              |
|         |                      |
| deck 1  |                      |
| deck 2  |                      |
| deck 3  |                      |
| ...     |                      |
|_________|______________________|
| FOOTER                         |
*--------------------------------*
*/


/// Deinitialize ncurses and free all windows and the created GUI structure.
void free_gui(GUI *gui) {
    free(gui->content_buffer);
    destroy_win(gui->navigation);
    destroy_win(gui->content);
    destroy_win(gui->footer);
    unpost_menu(gui->menu);
    for(int i = 0; i < gui->num_menu_items; i++) {
        free_item(gui->menu_items[i]);
    }
    free(gui->menu_items);
    free_menu(gui->menu);
    free(gui);
}


WINDOW *_new_content_window(GUI *gui) {
    return newwin(gui->max_height - FOOTER_MAX_HEIGHT - 2, CONTENT_MAX_WIDTH, 0, NAVIGATION_MAX_WIDTH + 2);
}

WINDOW *_new_navigation_window(GUI *gui) {
    return newwin(gui->max_height - FOOTER_MAX_HEIGHT - 2, NAVIGATION_MAX_WIDTH, 0, 0);
}

WINDOW *_new_footer_window(GUI *gui) {
    return newwin(gui->max_height, gui->max_width, gui->max_height - FOOTER_MAX_HEIGHT, 0);
}

/// Prepare the GUI structure and return a pointer to it.
GUI *new_gui(Deck **decks) {
    GUI *gui = calloc(1, sizeof(GUI));
    if(gui == NULL) {
        perror("new_gui: Failed to mallocate the GUI.");
        return NULL;
    }

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);
    gui->max_height = max_height;
    gui->max_width = max_width;
    gui->navigation = NULL;
    gui->content = NULL;
    gui->footer = NULL;
    gui->active_deck = NULL;
    gui->active_card = NULL;
    gui->active_window = NULL;
    gui->answer_state = HIDE_ANSWER;
    gui->navigation = _new_navigation_window(gui);

    gui->content_buffer = calloc(MAX_CARD_CONTENT, sizeof(char));
    if(gui->content_buffer == NULL) {
        perror("Failed to allocate content buffer.");
        free_gui(gui);
        return NULL;
    }

    if(gui->navigation == NULL) {
        perror("Failed to initialize navigation window.");
        free_gui(gui);
        return NULL;
    }

    gui->content = _new_content_window(gui);
    if(gui->content == NULL) {
        perror("Failed to initialize content window.");
        free_gui(gui);
        return NULL;
    }

    gui->footer = _new_footer_window(gui);
    if(gui->footer == NULL) {
        perror("Failed to initialize footer window.");
        free_gui(gui);
        return NULL;
    }

    gui->num_menu_items = ARRAY_SIZE(decks);
    gui->menu_items = calloc(gui->num_menu_items, sizeof(ITEM *));
    if(gui->menu_items == NULL) {
        perror("Failed to allocate memory for menu entries.");
        free_gui(gui);
        return NULL;
    }
    convert_to_menu_items(decks, gui->menu_items);
    build_menu(gui, gui->menu_items);

    return gui;
}

/// Refresh all windows.
void wrefresh_all(GUI *gui) {
    wrefresh(gui->navigation);
    wrefresh(gui->footer);
    refresh();
    wrefresh(gui->content);
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

    // Draw the titles.
    attron(COLOR_PAIR(2));
    mvwprintw(stdscr, 0, 1, "Decks");
    mvwprintw(stdscr, 0, CONTENT_START_X + 1, "Cards");
    attroff(COLOR_PAIR(2));

    // Draw the vertical bar which separates navigation from content.
    wmove(stdscr, 1, CONTENT_START_X);
    attron(COLOR_PAIR(3));
    vline(0, gui->max_height - FOOTER_MAX_HEIGHT - 1);
    attroff(COLOR_PAIR(3));

    // Draw horizontal bar just above the footer.
    wmove(stdscr, gui->max_height - FOOTER_MAX_HEIGHT - 1, 0);
    attron(COLOR_PAIR(1));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(1));

    move(y, x);
}

//// TODO: Resize all windows. Used when the terminal changes it's dynamics.
// Signal handler SIGWCH
void resize(GUI *gui) {
    draw_gui(gui);
    wrefresh_all(gui);
}

// Render the navigation.
void render_navigation(GUI *gui) {
    post_menu(gui->menu);
}

/// Render a card in the content window.
void render_content(GUI *gui, Card *card, const int show_answer, char *content_buffer) {
    destroy_win(gui->content);
    gui->content = _new_content_window(gui);
    if(gui->content == NULL) {
        perror("render_content: Failed to allocate a new content window.");
        free_gui(gui);
        return;
    }
    if(card == NULL) {
        mvwprintw(gui->content, 4, 4, "Deck is empty.");
    }
    else {
        if(show_answer) {
            strncat(content_buffer, card->question, MAX_CARD_QUESTION);
            strcat(content_buffer, "\n \n \n");
            strncat(content_buffer, card->answer, MAX_CARD_ANSWER-6);
            word_wrap(content_buffer, CONTENT_MAX_WIDTH - 3);
        }
        else {
            strncpy(content_buffer, card->question, MAX_CARD_ANSWER);
            word_wrap(content_buffer, CONTENT_MAX_WIDTH - 3);
        }
        print_wrapped_lines(gui->content, content_buffer, 4, 4);
    }
}

#endif
