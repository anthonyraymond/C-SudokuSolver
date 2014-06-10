CC=					gcc

DEBUG=				false

CFLAGS=				-Wall -Wextra

ifeq ($(DEBUG), true)
	CFLAGS+= -g
endif

INCLUDES=			-Iinclude

SRCS=				src/cell.c					\
					src/grid.c					\
					src/solver.c				\
					src/grid_integrity.c

OBJS=				$(patsubst src/%,obj/%,$(SRCS:.c=.o))

MAIN= sudokuSolver


.PHONY: clean

re: clean all

all : $(MAIN)
	@echo Compiled succefully

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(OBJS)

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS) $(MAIN)
