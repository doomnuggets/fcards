#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/card.h"
#include "lib/deck.h"
#include "lib/parser.h"
#include "lib/view/gui.h"


int main(int argc, char *argv[]) {
    char *deck_path = calloc(MAX_DECK_PATH, sizeof(char));
    strcpy(deck_path, "/home/me/Documents/code/ccards/decks/test/");
    Deck *deck = parse_deck(deck_path);

    GUI *gui = new_gui();


    free_gui(gui);
    free_deck(deck);
    free(deck_path);
    return 0;
}
