.SUFFIXES: .c .h .x .o

CO 	= gcc
FLAGS = -std=c99 -Wall -pedantic
DEBUG = -d
INFO = -v
HELP = -h
TO_CLEAN = *.x

src = $(wildcard *.c)
obj = $(src:.c=.o)
exe = $(src:.c=.x)

MAIN = main
HANDLER = handler

EXEC = $(MAIN).x

.PHONY: all clean compile server test ncat

%.o: %.c
	@$(CO) $^ $(FLAGS) -c

%.x: %.c
	@$(CO) $< $(FLAGS) -o $@

all: $(exe)
	@echo Files: {$(exe)} created. 

compile:
	@gcc $(FLAGS) $(src) -o $(EXEC) 

#uruchamia serwer
server: compile
	@clear
	@./$(EXEC)

help: compile
	@clear
	@./$(EXEC) -h

info: compile
	@clear
	@./$(EXEC) -v

debug: compile
	@clear
	@./$(EXEC) -d

info-debug: compile
	@clear
	@./$(EXEC) -v -d

tests:
	@clear
	@bash tester/runtests.sh

timeout:
	@clear
	@./tester/tester.x

ncat:
	@clear
	ncat -vC localhost 2020

clean:
	@rm $(TO_CLEAN)
	@echo Files: {$(exe)} removed.