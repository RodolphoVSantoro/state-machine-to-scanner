CC=gcc

ExecPath=./bin/main
TestExecPath=./bin/test

StaticLibs=./lib/state_machine.c ./lib/my_string.c

Src=./src/main.c $(StaticLibs)
TestSrc=./src/testes.c $(StaticLibs)

Warning=-Wall -Wextra -Werror -pedantic
Include=-Iinclude
StdC=-std=c99

build:
	$(CC) -o $(ExecPath) $(Warning) $(Include) $(StdC) $(Src)

test:
	$(CC) -o $(TestExecPath) $(Warning) $(Include) $(StdC) $(TestSrc)
	./bin/test

run:
	./bin/main