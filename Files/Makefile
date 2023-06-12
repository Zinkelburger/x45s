CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -O3
LIB = -lboost_unit_test_framework

.PHONY: all clean lint tests

all: Frank lint tests

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

Frank: 45s.o card.o deck.o main.o computer.o player.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

tests: 45s.o card.o deck.o tests.o player.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o Frank test
