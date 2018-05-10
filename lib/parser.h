#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>

#include "errors.h"
#include "constants.h"
#include "util.h"
#include "deck.h"
#include "card.h"


int _compare(const FTSENT **one, const FTSENT **two) {
    return strcmp((*one)->fts_name, (*two)->fts_name);
}

// Parse a file into a card.
Card *parse_card(FILE *card_file) {
    char content[MAX_CARD_CONTENT] = {'\0'};
    size_t bytes_read = fread(content, sizeof(char), MAX_CARD_CONTENT-1, card_file);
    if(bytes_read < 1) {
        perror("parse_card: empty card file.");
        return NULL;
    }

    char *question = content;
    char *answer = strstr(content, ANSWER_MARKER);
    int position = answer - content;
    question[position] = '\0';
    answer += strlen(ANSWER_MARKER)+2;
    return new_card(question, answer);
}

// Parses a directory structure into a deck of cards.
//
//  /path/to/deck/[001_card, 002_card, ...]
//
Deck *parse_deck(char *deck_path) {
    FTS *file_system = NULL;
    FTSENT *node = NULL;

    char *dp[] = {deck_path, NULL};
    file_system = fts_open(dp, FTS_COMFOLLOW | FTS_NOCHDIR, &_compare);
    if(file_system == NULL) {
        perror("parse_deck: Unable to parse deck.");
        return NULL;
    }

    char *deck_name = filename_from_path(deck_path);
    Deck *deck = new_deck(deck_name);
    if(deck == NULL) {
        return NULL;
    }

    while((node = fts_read(file_system)) != NULL) {
        // Only parse files inside the deck directory.
        if(node->fts_info == FTS_F) {
            FILE *card_file = fopen(node->fts_path, "r");
            if(card_file == NULL) {
                continue;
            }
            Card *c = parse_card(card_file);
            if(c != NULL) {
                push_card_bottom(c, deck);
            }
            fclose(card_file);
        }
    }

    fts_close(file_system);
    return deck;
}

#endif
