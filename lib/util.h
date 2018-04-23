#ifndef __UTIL_H
#define __UTIL_H

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

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

#endif
