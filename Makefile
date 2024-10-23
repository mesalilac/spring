CC=gcc
CFLAGS=-Wall -Wextra -Wformat -pedantic -ggdb
IDIR=include
INCLUDE=-I$(IDIR)/
LIBS= -lSDL2 -lSDL2_ttf
INCLUDE_SRCS=$(IDIR)/global.c
GAME_SRCS=game/main.c $(INCLUDE_SRCS)
LEVEL_DESIGNER_SRCS=level-designer/main.c $(INCLUDE_SRCS)

build: build-game build-level-designer

build-game:
	$(CC) $(CFLAGS) $(INCLUDE) -o game.out $(GAME_SRCS) $(LIBS)

build-level-designer:
	$(CC) $(CFLAGS) $(INCLUDE) -o level-designer.out $(LEVEL_DESIGNER_SRCS) $(LIBS)
