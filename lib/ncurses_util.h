#ifndef __NCURSES_UTIL_H
#define __NCURSES_UTIL_H

#include <string.h>

#include <ncurses.h>

void destroy_win(WINDOW *local_win) {
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	/* The parameters taken are
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window
	 * 3. rs: character to be used for the right side of the window
	 * 4. ts: character to be used for the top side of the window
	 * 5. bs: character to be used for the bottom side of the window
	 * 6. tl: character to be used for the top left corner of the window
	 * 7. tr: character to be used for the top right corner of the window
	 * 8. bl: character to be used for the bottom left corner of the window
	 * 9. br: character to be used for the bottom right corner of the window
	 */
    wclear(local_win);
	wrefresh(local_win);
	delwin(local_win);
}

// Print the given string in the center of the passed window.
void print_center(WINDOW *win, int starty, int startx, int width, char *string) {
    int length, x, y;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	x = startx + (int)((width - length) / 2);
	mvwprintw(win, y, x, "%s", string);
}

int wordlen(const char *str) {
    int tempindex = 0;
    while(str[tempindex] != ' ' && str[tempindex] != 0 && str[tempindex] != '\n') {
        ++tempindex;
    }

    return tempindex;
}

// Wrap a paragraph to a given max width.
void word_wrap(char *s, const int wrapline) {
    int index = 0;
    int curlinelen = 0;
    while(s[index] != '\0') {
        if(s[index] == '\n') {
            curlinelen = 0;
        }
        else if(s[index] == ' ') {
            if(curlinelen + wordlen(&s[index+1]) >= wrapline) {
                s[index] = '\n';
                curlinelen = 0;
            }
        }
        curlinelen++;
        index++;
    }
}

// Print multiplie lines which were previously wrapped with `word_wrap`.
void print_wrapped_lines(WINDOW *window, char *s, const int start_y, const int start_x) {
    char *token = NULL;
    char *current_haystack = s;
    int current_y = start_y;
    while((token = strtok(current_haystack, "\n")) != NULL) {
        mvwprintw(window, current_y, start_x, "%s", token);
        current_y++;
        current_haystack = NULL;
    }
}

#endif
