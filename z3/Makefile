.SUFFIXES: .c .h .x .o

CO = gcc
FLAGS = -std=c99 -Wall -pedantic
TO_CLEAN = *.x

src = $(wildcard *.c)
obj = $(src:.c=.o)
exe = $(src:.c=.x)

SERVER = server
HANDLER = handler

EXEC_SERVER = $(SERVER).x

.PHONY: all clean compile server

all: $(exe)
	@echo Files: {$(exe)} created. 

#do uruchomienia z dodatkowymi flagami/programami
compile:
	@gcc $(FLAGS) $(SERVER).c $(HANDLER).c -o $(EXEC_SERVER) 

#uruchamia serwer
server: 
	@gcc $(FLAGS) $(SERVER).c $(HANDLER).c -o $(EXEC_SERVER) 
	@./$(EXEC_SERVER)

%.o: %.c
	@$(CO) $^ $(FLAGS) -c

%.x: %.c
	@$(CO) $< $(FLAGS) -o $@

clean:
	@rm $(TO_CLEAN)
	@echo Files: {$(exe)} removed.