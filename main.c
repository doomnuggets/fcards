#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/card.h"
#include "lib/deck.h"
#include "lib/parser.h"


int main(int argc, char *argv[]) {
    char *deck_path = calloc(MAX_DECK_PATH, sizeof(char));
    strcpy(deck_path, "/home/me/Documents/code/ccards/decks/test/");
    Deck *deck = parse_deck(deck_path);
    puts("printing cards...");
    for(int i = 0; i < deck->num_of_cards; i++) {
        print_card(get_card(i, deck));
        puts("--------------------------------------");
    }

    puts("Picking a random card...");
    srand(time(NULL));
    print_card(pick_random_card(deck));
    puts("--------------------------------------");

    free_deck(deck);
    free(deck_path);

    return 0;
}
