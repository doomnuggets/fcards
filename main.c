#include <stdio.h>
#include <string.h>

#include "lib/card.h"
#include "lib/deck.h"



void print_card(Card *c) {
    printf("Question:\n%s\n", c->question);
    printf("Answer:\n%s\n", c->answer);
    printf("\n-----------------------------\n");
}

int main(int argc, char *argv[]) {

    // Prepare a new deck.
    Deck *deck = new_deck("Test Deck");
    if(!deck) {
        perror("Unable to allocate deck :(");
        return -1;
    }

    // Populate the deck with cards.
    for(int i = 0; i < 10; i++) {
        Card *c = new_card("Question 1?", "Answer 1.");
        if(!c) {
            perror("Unable to allocate card :(");
            free(deck);
            return -1;
        }
        push_card_top(c, deck);
    }

    // Render the cards.
    Card *card = deck->top;
    print_card(card);
    while((card = card->next) != NULL) {
        print_card(card);
    }
    free_deck(deck);
    return 0;
}
