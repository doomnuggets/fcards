

all: main.c lib/*.h
	gcc -ggdb main.c lib/*.h -o ccards
