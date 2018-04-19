#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <string.h>


/// Return the last part of a directory or file path.
//   /var/log/sec returns sec.
char *filename_from_path(char *path) {
    if(path && *path) {
        if(path[strlen(path) - 1] == '/') {
            path[strlen(path) - 1] = '\0';
        }
    }
    char *ptr = strrchr(path, '/');
    return ++ptr;
}

// Count the number of characters in a word (separated by space).
int wordlen(const char *str) {
    int index = 0;
    while(str[index] != ' ' && str[index] != 0 && str[index] != '\n') {
        ++index;
    }

    return index;
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

#endif
