#ifndef __DECK_H
#define __DECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "errors.h"
#include "constants.h"
#include "card.h"

typedef struct Deck {
    char *name;
    Card *top;
    Card *bottom;
    unsigned long num_of_cards;
} Deck;


// TODO:
// Pick a random card from the passed deck.
Card *get_random_card(Deck *d) {
    Card *c = NULL;
    return c;
}

// TODO: Check memory errors
Deck *new_deck(char *name) {
    Deck *d = malloc(sizeof(Deck));
    d->name = calloc(MAX_DECK_NAME, sizeof(char));
    strncpy(d->name, name, MAX_DECK_NAME);
    d->name[MAX_DECK_NAME-1] = '\0';
    return d;
}

bool is_deck_empty(Deck *d) {
    return (d->top != NULL || d->bottom != NULL);
}

bool is_top_card(Card *c, Deck *d) {
    if(d->top == c) {
        return true;
    } else if(c->next != NULL, c->prev == NULL) {
        return true;
    }
    return false;
}

bool is_bottom_card(Card *c, Deck *d) {
    if(d->bottom == c) {
        return true;
    } else if(c->next == NULL && c->prev != NULL) {
        return true;
    }
    return false;
}

int insert_card(Card *c, unsigned long index, Deck *d) {
    if(c == NULL) {
        return INVALID_CARD;
    }
    if(d == NULL) {
        return INVALID_DECK;
    }
    d->num_of_cards++;
}

// Get a card by index from the deck.
Card *get_card(unsigned long index, Deck *d) {
    if(index+1 > d->num_of_cards)
        return NULL;
    if(d == NULL)
        return NULL;

    unsigned long matched_index = 0;
    Card *c = d->top;
    while((c = c->next) != NULL && matched_index < index) {
        matched_index++;
    }
    return c;
}

// Add a card to the top of the deck.
int push_card_top(Card *c, Deck *d) {
    if(c == NULL)
        return INVALID_CARD;
    if(d == NULL)
        return INVALID_DECK;

    Card *old_top_card = NULL;
    if(d->top != NULL) {
        old_top_card = d->top;
        old_top_card->prev = c;
    }
    c->next = old_top_card;
    c->prev = NULL;
    d->top = c;
    d->num_of_cards++;
}

// Add a card to the bottom of the deck.
int push_card_bottom(Card *c, Deck *d) {
    if(c == NULL)
        return INVALID_CARD;
    if(d == NULL)
        return INVALID_DECK;

    Card *old_bottom_card = NULL;
    if(d->bottom != NULL) {
        old_bottom_card = d->bottom;
        old_bottom_card->next = c;
    }
    c->next = NULL;
    c->prev = old_bottom_card;
    d->bottom = c;
    d->num_of_cards++;
}

// Remove a card from the deck. This calls `free_card` and thus removes it.
int remove_card(unsigned long index, Deck *d) {
    if(index+1 > d->num_of_cards) {
        return INVALID_INDEX;
    }
    if(d == NULL) {
        return INVALID_DECK;
    }
    if(is_deck_empty(d)) {
        return CARD_NOT_FOUND;
    }

    // Find the card to delete.
    Card *c = d->top;
    unsigned long matched_index = 0;
    for(matched_index; matched_index < index; matched_index++) {
        if(c->next != NULL) {
            c = c->next;
        } else {
            return CARD_NOT_FOUND;
        }
    }

    // Restructure the doubly linked list of cards.
    if(is_top_card(c, d)) {
        Card *new_top_card = c->next;
        Card *next_top_card = new_top_card->next;
        new_top_card->next = next_top_card;
        next_top_card->prev = new_top_card;
    }
    else if(is_bottom_card(c, d)) {
        Card *new_bottom_card = c->prev;
        Card *prev_bottom_card = new_bottom_card->prev;
        prev_bottom_card->next = new_bottom_card;
        new_bottom_card->prev = prev_bottom_card;
    }
    else { // Some card below the top or above the bottom.
        Card *next_card = c->next;
        Card *prev_card = c->prev;
        prev_card->next = next_card;
        next_card->prev = prev_card;
    }
    free_card(c);
    d->num_of_cards--;
    return 0;
}

// Free a deck with all it's cards and information.
void free_deck(Deck *d) {
    // Free all cards.
    while(d->top != NULL) {
        Card *tmp = d->top;
        d->top = d->top->next;
        tmp->next = NULL;
        tmp->prev = NULL;
        free_card(tmp);
    }

    // Free the meta.
    free(d->name);
    d->top = NULL;
    d->bottom = NULL;
    d->num_of_cards = 0;
    free(d->top);
    free(d->bottom);

    // Free the deck.
    free(d);
}

#endif
