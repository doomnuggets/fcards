#ifndef __CARD_H
#define __CARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"


typedef struct Card Card;

struct Card {
    char *question;
    char *answer;
    Card *prev;
    Card *next;
};

void free_card(Card *c) {
    free(c->question);
    free(c->answer);
    free(c);
}

Card *new_card(char *question, char *answer) {
    Card *c = malloc(sizeof(Card));
    if(!c) {
        return NULL;
    }
    c->question = calloc(MAX_CARD_QUESTION+1, sizeof(char));
    if(c->question == NULL) {
        free_card(c);
        perror("new_card: Failed to allocate space for question.");
        return NULL;
    }
    c->answer = calloc(MAX_CARD_ANSWER+1, sizeof(char));
    if(c->answer == NULL) {
        free_card(c);
        perror("new_card: Failed to allocate memory for answer.");
        return NULL;
    }
    strncpy(c->question, question, MAX_CARD_QUESTION);
    strncpy(c->answer, answer, MAX_CARD_ANSWER);
    c->prev = NULL;
    c->next = NULL;

    return c;
}

#endif
