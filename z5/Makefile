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

.PHONY: all clean compile server test ncat

all: $(exe)
	@echo Files: {$(exe)} created. 

#do uruchomienia z dodatkowymi flagami/programami
compile:
	@gcc $(FLAGS) $(SERVER).c -o $(EXEC_SERVER) 

#uruchamia serwer
server: 
	@gcc $(FLAGS) $(SERVER).c -o $(EXEC_SERVER) 
	@clear
	@./$(EXEC_SERVER)

client: 
	@gcc $(FLAGS) client.c -o client.x 
	@clear
	@./client.x

valgrind: 
	@gcc $(FLAGS) $(SERVER).c -o $(EXEC_SERVER) 
	@clear
	@valgrind ./$(EXEC_SERVER)

ncat:
	@clear
	ncat -v -C localhost 2020 

sockat:
	@clear
	printf "3 429496729\r\n1 2 3 4\r\n" | socat stdio TCP:127.0.0.1:2020

sockat1:
	@clear
	printf "1 2 3 4 5\r\n6 7 8 9\r\n" | socat stdio TCP:127.0.0.1:2020

spaces:
	@clear
	printf " 1 2 3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2  3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 3 4 \r\n" | socat stdio TCP:127.0.0.1:2020
	printf "\r\n" | socat stdio TCP:127.0.0.1:2020
	printf " \r\n" | socat stdio TCP:127.0.0.1:2020
	printf "  \r\n" | socat stdio TCP:127.0.0.1:2020


letters:
	@clear
	printf "1 2 e\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1r3 14 15\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 e\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 e\r\n" | socat stdio TCP:127.0.0.1:2020
	
test:
	@clear
	printf "1 2 3 4 5 6 7 8 9 10\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 3 4 5 6 r 8 9 10\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "100 200 300 400 5OO\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 3 4\r\n1 2 3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2\n 3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2\r 3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf " 1 2 3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2  3 4\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "1 2 3 4 \r\n" | socat stdio TCP:127.0.0.1:2020
	printf "\r\n" | socat stdio TCP:127.0.0.1:2020
	printf " \r\n" | socat stdio TCP:127.0.0.1:2020
	printf "  \r\n" | socat stdio TCP:127.0.0.1:2020
	printf "18446744073709551615\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "18446744073709551615 1 1\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "18446744073709551616\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "4611686018427387904 4611686018427387904 4611686018427387904 4611686018427387904\r\n" | socat stdio TCP:127.0.0.1:2020
	printf "4611686018427387904 4611686018427387904 4611686018427387904 4611686018427387903\r\n" | socat stdio TCP:127.0.0.1:2020

%.o: %.c
	@$(CO) $^ $(FLAGS) -c

%.x: %.c
	@$(CO) $< $(FLAGS) -o $@

clean:
	@rm $(TO_CLEAN)
	@echo Files: {$(exe)} removed.