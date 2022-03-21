CO = gcc
FLAGS = -std=c99 -Wall -pedantic
TO_CLEAN = *.x

src = $(wildcard *.c)
obj = $(src:.c=.o)
exe = $(src:.c=.x)

.PHONY: all clean

all: $(exe)
	@echo Files: {$(exe)} created. 

#%.o: %.c
#	@$(CO) $^ $(FLAGS) -c

%.x: %.c
	@$(CO) $< $(FLAGS) -o $@

clean:
	@rm $(TO_CLEAN)
	@echo Files: {$(exe)} removed.