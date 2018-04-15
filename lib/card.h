#ifndef __CARD_H
#define __CARD_H

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
    c->prev = NULL;
    c->next = NULL;
    free(c);
}

// TODO: Check memory errors
Card *new_card(char *question, char *answer) {
    Card *c = malloc(sizeof(Card));
    if(!c) {
        return NULL;
    }
    c->question = calloc(MAX_CARD_QUESTION, sizeof(char));
    c->answer = calloc(MAX_CARD_ANSWER, sizeof(char));
    strncpy(c->question, question, MAX_CARD_QUESTION);
    c->question[MAX_CARD_QUESTION-1] = '\0';
    strncpy(c->answer, answer, MAX_CARD_ANSWER);
    c->answer[MAX_CARD_ANSWER-1] = '\0';

    return c;
}

#endif
