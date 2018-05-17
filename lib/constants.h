#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define MAX_DECKS 20
#define MAX_DECK_NAME 48
#define MAX_DECK_PATH 256
#define MAX_CARDS 200
#define MAX_CARD_PATH 256
#define MAX_CARD_QUESTION 4096
#define MAX_CARD_ANSWER 4096
#define MAX_CARD_CONTENT 81930

// lib/view.h
#define NAVIGATION_START_X 1
#define NAVIGATION_START_Y 1
#define NAVIGATION_MAX_WIDTH 50

#define CONTENT_START_X 52
#define CONTENT_START_Y 1
#define CONTENT_MAX_WIDTH 200

#define FOOTER_START_X 0
#define FOOTER_MAX_HEIGHT 2

// lib/navigation.h
#define REQ_PREV_CARD 101
#define REQ_NEXT_CARD 202
#define REQ_RAND_CARD 303
#define REQ_TOGGLE_ANSWER 404
#define HIDE_ANSWER 0
#define SHOW_ANSWER 1

const char ANSWER_MARKER[] = "-xxx-";
const char EXIT_CHAR = 'q';

#endif
