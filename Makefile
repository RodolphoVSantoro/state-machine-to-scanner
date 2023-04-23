CC=gcc

ExecPath=./bin/main.exe
TestExecPath=./bin/test.exe

StaticLibs=./lib/state_machine.c ./lib/my_string.c ./lib/vecs.c

Src=./src/main.c $(StaticLibs)
TestSrc=./src/testes.c $(StaticLibs)

Warning=-Wall -Wextra -Werror -pedantic
Include=-Iinclude
StdC=-std=c99

build:
	$(CC) -o $(ExecPath) $(Warning) $(Include) $(StdC) $(Src)

test:
	$(CC) -o $(TestExecPath) $(Warning) $(Include) $(StdC) $(TestSrc)
	$(TestExecPath)

run:
	$(ExecPath)