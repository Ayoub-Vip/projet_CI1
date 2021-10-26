SRCS = individual.c population.c tsp.c testga.c gifenc.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = testga
CFLAGS = -Wall -Wextra -Wmissing-prototypes --pedantic -std=c99 -g
LDFLAGS = -lm

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG)

gifenc.o: gifenc.c gifenc.h
individual.o: individual.c individual.h
population.o: population.c population.h individual.h
testga.o: testga.c individual.h population.h tsp.h
tsp.o: tsp.c gifenc.h tsp.h population.h individual.h
