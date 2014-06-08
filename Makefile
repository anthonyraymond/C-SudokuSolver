CC=					gcc

CFLAGS=				-Wall -Wextra -g

INCLUDES=			-Iinclude

SRCS=				src/cell.c					\
					src/grid.c					\
					src/solver.c				\
					src/grid_integrity.c

OBJS=				$(SRCS:.c=.o)
NOBJS:=				$(subst src/,obj/,$(OBJS))

MAIN = sudokuSolver


.PHONY: depend clean

all : $(MAIN)
	@echo Compiled succefully

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(NOBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $(subst src/,obj/,$@)

clean:
	rm obj/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepends $(INCLUDES) $^
