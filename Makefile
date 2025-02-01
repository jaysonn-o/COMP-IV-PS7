CC = g++
CFLAGS = -std=c++11 -Wall -Werror -pedantic
FLAGS = -lboost_regex -lboost_date_time

ps7: main.cpp
	$(CC) $(CFLAGS) main.cpp -o ps7 $(FLAGS)

all: ps7

clean:
	rm -f *.o ps7

lint:
	cpplint *.cpp *.hpp

.PHONY: all clean lint
