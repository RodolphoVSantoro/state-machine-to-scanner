CC=gcc
ExecPath=./bin/main
TestExecPath=./bin/test
Src=main.c state_machine.c my_string.c
TestSrc=state_machine.c testes.c my_string.c
Warning=-Wall -Wextra -Werror -pedantic
StdC=-std=c99

build:
	$(CC) -o $(ExecPath) $(Src) $(Warning) $(StdC)

test:
	$(CC) -o $(TestExecPath) $(TestSrc) $(Warning) $(StdC) -DTEST
	./bin/test

run:
	./bin/main