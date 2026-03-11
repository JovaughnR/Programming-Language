CC = gcc
CURL_PREFIX = $(shell brew --prefix curl)
CFLAGS = -g -Wall -I./include -I./lib -I/opt/homebrew/include -I$(CURL_PREFIX)/include
LIBS = -lm -L/opt/homebrew/lib -lgmp -L$(CURL_PREFIX)/lib -lcurl -lsqlite3 -lpthread

SOURCES = ./parse/y.tab.c ./parse/lex.yy.c \
          ./core/class.c ./core/arena.c \
          ./core/eval.c ./core/exec.c \
          ./core/utils.c ./core/list.c \
          ./core/set.c ./core/dict.c \
          ./core/str.c ./core/maloc.c \
          ./core/daloc.c ./core/error.c \
          ./core/format.c ./core/module.c \
          ./builtin/build.c ./builtin/methods.c \
          ./builtin/maths.c ./builtin/time.c \
          ./builtin/os.c ./builtin/io.c \
          ./builtin/random.c ./builtin/json.c \
          ./builtin/sys.c ./builtin/path.c \
          ./builtin/regex.c ./builtin/hash.c \
          ./builtin/csv.c ./builtin/stats.c \
          ./builtin/decimal.c ./builtin/env.c \
          ./builtin/log.c ./builtin/args.c \
          ./builtin/net.c ./builtin/socket.c \
          ./builtin/db.c ./builtin/thread.c \
          ./builtin/async.c ./builtin/http.c \
	   ./vm/vm.c ./vm/disasm.c \
			 ./vm/chunk.c ./vm/compiler.c \
			 ./vm/serial.c \
	  		 main.c

OBJ_DIR = ./build
OBJECTS = $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))
TARGET  = cxlang

# ---------------------------------------------------------------------------
# Top-level targets
# ---------------------------------------------------------------------------

all: ./parse/y.tab.c ./parse/lex.yy.c $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBS)

# ---------------------------------------------------------------------------
# Parser / lexer generation
# ---------------------------------------------------------------------------

./parse/y.tab.c ./parse/y.tab.h: ./parse/parser.y
	yacc -d ./parse/parser.y
	mv y.tab.c ./parse/y.tab.c
	mv y.tab.h ./parse/y.tab.h

./parse/lex.yy.c: ./parse/tokens.l ./parse/y.tab.h
	lex ./parse/tokens.l
	mv lex.yy.c ./parse/lex.yy.c

# ---------------------------------------------------------------------------
# Compile rules — one pattern per source directory
# ---------------------------------------------------------------------------

$(OBJ_DIR)/%.o: ./parse/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./core/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./builtin/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./vm/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# ---------------------------------------------------------------------------
# Housekeeping
# ---------------------------------------------------------------------------

clean:
	rm -rf $(OBJ_DIR) $(TARGET) \
	       ./parse/y.tab.c ./parse/y.tab.h ./parse/lex.yy.c

.PHONY: all clean