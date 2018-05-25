all: main.c lib/*.h
	gcc -Wall -ggdb main.c lib/*.h -lmenu -lncursesw -o ccards
