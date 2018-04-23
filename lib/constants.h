#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define MAX_DECK_NAME 48
#define MAX_DECK_PATH 256
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
#define CONTENT_MAX_WIDTH 190

#define FOOTER_START_X 0
#define FOOTER_MAX_HEIGHT 2

#define HIDE_ANSWER 0
#define SHOW_ANSWER 1

const char ANSWER_MARKER[] = "-xxx-";
const char EXIT_CHAR = 'q';
const char VERSION[] = "0.1";

#endif
