#ifndef __UTIL_H
#define __UTIL_H

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <dirent.h>


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


int _compare(const FTSENT **one, const FTSENT **two) {
    return strcmp((*one)->fts_name, (*two)->fts_name);
}

void file_walk(char *path, char *output_paths[], int max_paths) {
    FTS *file_system = NULL;
    FTSENT *node = NULL;
    char *p[] = {path, NULL};
    file_system = fts_open(p, FTS_COMFOLLOW | FTS_NOCHDIR, &_compare);
    if(file_system == NULL) {
        return;
    }

    int index = 0;
    while((node = fts_read(file_system)) != NULL && index < max_paths) {
        if(node->fts_info == FTS_F) {
            strncpy(output_paths[index], node->fts_path, PATH_MAX-1);
            index++;
        }
    }

    fts_close(file_system);
}

void dir_walk(char *path, char *dirs[], int max_dirs) {

    DIR *d = opendir(path);
    if(d == NULL) {
        return;
    }

    int i = 0;
    while(i < max_dirs) {
        struct dirent *entry;
        entry = readdir(d);
        if(entry == NULL) {
            dirs[i] = NULL;
            break;
        }
        if(entry->d_type & DT_DIR) {
            if(strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                strncpy(dirs[i], entry->d_name, sizeof(char) * PATH_MAX);
                i++;
            }
        }
    }
}

#endif
