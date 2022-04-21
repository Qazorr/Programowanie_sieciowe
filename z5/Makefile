.SUFFIXES: .c .h .x .o

CO 	= gcc
FLAGS = -std=c99 -Wall -pedantic
DEBUG = -d
INFO = -v
HELP = -h
TO_CLEAN = *.x

src = $(wildcard *.c)
obj = $(src:.c=.o)

MAIN = main

EXEC = $(MAIN).x # entry point

.PHONY: run help info debug info-debug ncat tests timeout clean

%.o: %.c
	@$(CO) $^ $(FLAGS) -c

%.x: %.c
	@$(CO) $< $(FLAGS) -o $@

all: $(exe)
	@echo Files: {$(exe)} created. 

compile:
	@gcc $(FLAGS) $(src) -o $(EXEC) 

#uruchamia serwer
run: compile
	@clear
	@./$(EXEC)

help: compile
	@clear
	@./$(EXEC) $(HELP)

info: compile
	@clear
	@./$(EXEC) $(INFO)

debug: compile
	@clear
	@./$(EXEC) $(DEBUG)

info-debug: compile
	@clear
	@./$(EXEC) $(INFO) $(DEBUG)

ncat:
	@clear
	ncat -vC localhost 2020

tests:
	@clear
	@bash tester/runtests.sh

timeout:
	@clear
	@./tester/tester.x

clean:
	@rm $(TO_CLEAN)
	@echo Files: {$(exe)} removed.