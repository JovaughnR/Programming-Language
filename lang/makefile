CC = gcc
CFLAGS = -fsanitize=address -g -Wall -I./include -I./lib
LIBS = -lm

SOURCES = ./parse/y.tab.c ./parse/lex.yy.c eval.c exec.c utils.c list.c set.c \
          dict.c str.c maloc.c daloc.c error.c ./builtin/build.c ./builtin/methods.c \
          class.c arena.c module.c format.c

OBJECTS = $(SOURCES:.c=.o)
TARGET = interpreter

all: ./parse/y.tab.c ./parse/lex.yy.c $(TARGET)

./parse/y.tab.c ./parse/y.tab.h: ./parse/parser.y
	yacc -d ./parse/parser.y
	mv y.tab.c ./parse/y.tab.c
	mv y.tab.h ./parse/y.tab.h

./parse/lex.yy.c: ./parse/tokens.l ./parse/y.tab.h
	lex ./parse/tokens.l
	mv lex.yy.c ./parse/lex.yy.c

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

./parse/y.tab.o: ./parse/y.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

./parse/lex.yy.o: ./parse/lex.yy.c
	$(CC) $(CFLAGS) -c $< -o $@

eval.o: eval.c
	$(CC) $(CFLAGS) -c $< -o $@

exec.o: exec.c
	$(CC) $(CFLAGS) -c $< -o $@

format.o: format.c
	$(CC) $(CFLAGS) -c $< -o $@

list.o: list.c
	$(CC) $(CFLAGS) -c $< -o $@

dict.o: dict.c
	$(CC) $(CFLAGS) -c $< -o $@

set.o: set.c
	$(CC) $(CFLAGS) -c $< -o $@

str.o: str.c
	$(CC) $(CFLAGS) -c $< -o $@

utils.o: utils.c
	$(CC) $(CFLAGS) -c $< -o $@

maloc.o: maloc.c
	$(CC) $(CFLAGS) -c $< -o $@

daloc.o: daloc.c
	$(CC) $(CFLAGS) -c $< -o $@

error.o: error.c
	$(CC) $(CFLAGS) -c $< -o $@

class.o: class.c
	$(CC) $(CFLAGS) -c $< -o $@

arena.o: arena.c
	$(CC) $(CFLAGS) -c $< -o $@

module.o: module.c
	$(CC) $(CFLAGS) -c $< -o $@

./builtin/build.o: ./builtin/build.c
	$(CC) $(CFLAGS) -c $< -o $@

./builtin/methods.o: ./builtin/methods.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) ./parse/y.tab.c ./parse/y.tab.h ./parse/lex.yy.c

.PHONY: all clean