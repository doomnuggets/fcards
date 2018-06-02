#ifndef __PARSER_H
#define __PARSER_H

#include <stdio.h>

#include "errors.h"
#include "constants.h"
#include "util.h"
#include "deck.h"
#include "card.h"



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

// Parse a directory structure into a deck of cards.
//
//  /path/to/deck/[001_card, 002_card, ...]
//
Deck *parse_deck(char *deck_path) {
    char **files = calloc(MAX_CARDS, sizeof(char *));
    for(int i = 0; i < MAX_CARDS; i++) {
        files[i] = calloc(PATH_MAX, sizeof(char));
    }
    files[MAX_CARDS] = NULL;

    file_walk(deck_path, files, MAX_CARDS);
    char *deck_name = filename_from_path(deck_path);
    Deck *deck = new_deck(deck_name);
    if(deck == NULL) {
        return NULL;
    }
    int filename_index = 0;
    while(files[filename_index] != NULL && filename_index < MAX_CARDS) {
        FILE *card_file = fopen(files[filename_index], "r");
        if(card_file == NULL) {
            break;
        }
        Card *c = parse_card(card_file);
        if(c != NULL) {
            push_card_bottom(c, deck);
        }
        fclose(card_file);
        filename_index++;
    }

    for(int i = 0; i < MAX_CARDS; i++) {
        free(files[i]);
    }
    free(files);

    return deck;
}

// Parse a directory of decks.
Deck **parse_decks(char *deck_root) {
    char **deck_dirs = calloc(MAX_DECKS, sizeof(char *));
    for(int i = 0; i < MAX_DECKS; i++) {
        deck_dirs[i] = calloc(PATH_MAX, sizeof(char));
    }
    deck_dirs[MAX_DECKS] = NULL;
    dir_walk(deck_root, deck_dirs, MAX_DECKS);
    Deck **decks = calloc(MAX_DECKS, sizeof(Deck *));
    for(int i = 0; i < MAX_DECKS; i++) {
        decks[i] = calloc(1, sizeof(Deck));
    }

    int deck_index = 0;
    while(deck_dirs[deck_index] != NULL && deck_index < MAX_DECKS) {
        Deck *deck = parse_deck(deck_dirs[deck_index]);
        if(deck == NULL) {
            fprintf(stderr, "Failed to parse deck: %s\n", deck_dirs[deck_index]);
            break;
        }
        decks[deck_index] = deck;
        deck_index++;
    }
    decks[deck_index] = NULL;
    for(int i = 0; i < MAX_DECKS; i++) {
        free(deck_dirs[i]);
    }
    free(deck_dirs);

    return decks;
}


#endif
