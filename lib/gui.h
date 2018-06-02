#ifndef __VIEW_H
#define __VIEW_H

#include <ncurses.h>
#include <menu.h>
#include <signal.h>

#include "constants.h"
#include "deck.h"
#include "gui_type.h"
#include "ncurses_util.h"
#include "menu_builder.h"
#include "dimensions.h"

/* GUI layout:
*--------------------------------*
| NAV     | CONTENT              |
|         |                      |
| deck 1  |                      |
| deck 2  |                      |
| deck 3  |                      |
| ...     |                      |
*--------------------------------*
*/


volatile static sig_atomic_t TERMINAL_RESIZED = 0;

void termina_resize_handler(int sig) {
    TERMINAL_RESIZED = 1;
}

// Deinitialize ncurses and free all windows and the created GUI structure.
void free_gui(GUI *gui) {
    free(gui->content_buffer);
    destroy_win(gui->navigation);
    destroy_win(gui->content);
    unpost_menu(gui->menu);
    free_menu(gui->menu);
    for(int i = 0; i < gui->num_menu_items; i++) {
        free_item(gui->menu_items[i]);
    }
    free(gui->menu_items);
    free(gui);
}

WINDOW *_new_content_window(GUI *gui) {
    return newwin(CONTENT_HEIGHT, CONTENT_WIDTH, 1, NAVIGATION_WIDTH + 1);
}

WINDOW *_new_navigation_window(GUI *gui) {
    return newwin(NAVIGATION_HEIGHT, NAVIGATION_WIDTH, 1, 0);
}

// Prepare the GUI structure and return a pointer to it.
GUI *new_gui(Deck **decks) {
    GUI *gui = (GUI *)calloc(1, sizeof(GUI));
    if(gui == NULL) {
        perror("new_gui: Failed to mallocate the GUI.");
        return NULL;
    }

    gui->max_height = 0;
    gui->max_width = 0;
    gui->navigation = NULL;
    gui->content = NULL;
    gui->decks = decks;
    gui->active_deck = NULL;
    gui->active_card = NULL;
    gui->active_window = NULL;
    gui->answer_state = HIDE_ANSWER;
    gui->menu = NULL;
    gui->menu_items = NULL;

    if(decks != NULL) {
        gui->active_deck = decks[0];
        gui->active_card = gui->active_deck->top;
    }

    gui->content_buffer = (char *)calloc(MAX_CARD_CONTENT, sizeof(char));
    if(gui->content_buffer == NULL) {
        perror("Failed to allocate content buffer.");
        free_gui(gui);
        return NULL;
    }

    recalculate_dimensions(gui);

    keypad(stdscr, TRUE);
    gui->content = _new_content_window(gui);
    if(gui->content == NULL) {
        perror("Failed to initialize content window.");
        free_gui(gui);
        return NULL;
    }
    keypad(gui->content, TRUE);

    gui->navigation = _new_navigation_window(gui);
    if(gui->navigation == NULL) {
        perror("Failed to initialize navigation window.");
        free_gui(gui);
        return NULL;
    }
    keypad(gui->navigation, TRUE);
    int num_menu_items = 0;
    while(gui->decks[num_menu_items] != NULL) {
        num_menu_items++;
    }
    gui->num_menu_items = num_menu_items;
    gui->menu_items = (ITEM **)calloc(gui->num_menu_items + 1, sizeof(ITEM *));
    if(gui->menu_items == NULL) {
        perror("Failed to allocate memory for menu entries.");
        free_gui(gui);
        return NULL;
    }
    convert_to_menu_items(gui->decks, gui->menu_items, gui->num_menu_items);
    build_menu(gui, gui->menu_items);

    return gui;
}

/// Refresh all windows.
void wrefresh_all(GUI *gui) {
    refresh();
    wrefresh(gui->content);
    wrefresh(gui->navigation);
}

void render_gui(GUI *gui) {
    int x;
    int y;
    // Store the initial cursor position.
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
    mvwprintw(stdscr, 0, NAVIGATION_WIDTH + 4, "Cards");
    attroff(COLOR_PAIR(2));

    // Draw the vertical bar which separates the navigation from the content.
    wmove(stdscr, 1, NAVIGATION_WIDTH);
    attron(COLOR_PAIR(3));
    vline(0, gui->max_height - 1);
    attroff(COLOR_PAIR(3));

    // Draw the horizontal bar just above the end of the window.
    wmove(stdscr, gui->max_height - 1, 0);
    attron(COLOR_PAIR(1));
    hline(0, gui->max_width);
    attroff(COLOR_PAIR(1));

    // Restore the initial cursor position.
    move(y, x);
}

/// Render a card in the content window.
void render_content(GUI *gui, Card *card, const int show_answer, char *content_buffer) {
    destroy_win(gui->content);
    gui->content = NULL;
    gui->content = _new_content_window(gui);
    if(gui->content == NULL) {
        perror("render_content: Failed to allocate a new content window.");
        free_gui(gui);
        return;
    }
    if(card == NULL) {
        mvwprintw(gui->content, CONTENT_PADDING_Y, CONTENT_PADDING_X, "Deck is empty.");
    }
    else {
        if(show_answer == HIDE_ANSWER) { // Only render the question.
            strncpy(content_buffer, card->question, MAX_CARD_ANSWER);
        }
        else { // Render both the question and the answer.
            strncat(content_buffer, card->question, MAX_CARD_QUESTION);
            strcat(content_buffer, "\n \n \n");
            strncat(content_buffer, card->answer, MAX_CARD_ANSWER-6);
        }
        word_wrap(content_buffer, CONTENT_WIDTH - CONTENT_PADDING_X);
        print_wrapped_lines(gui->content, content_buffer, CONTENT_PADDING_Y, CONTENT_PADDING_X);
    }
}

void handle_terminal_resize(GUI *gui) {
    recalculate_dimensions(gui);
    render_gui(gui);
    render_content(gui, gui->active_card, gui->answer_state, gui->content_buffer);
    TERMINAL_RESIZED = 0;
    wrefresh_all(gui);
}

#endif
