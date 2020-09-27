PROGRAM_NAME = 2048

COMPILER = g++

FLAGS = -I./inc/ -lpthread -lncurses -Wall -Wextra -Wpedantic

all:
	$(COMPILER) $(FLAGS) *.cpp -o $(PROGRAM_NAME)

debug:
	$(COMPILER) $(FLAGS) *.cpp -o $(PROGRAM_NAME)_debug -g

install:
	cp $(PROGRAM_NAME)  /usr/local/bin

uninstall:
	rm /usr/local/bin/$(PROGRAM_NAME)
