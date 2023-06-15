CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -O3
LIB = -lboost_unit_test_framework

.PHONY: all clean lint tests

all: Frank lint tests

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

testCard.o: testFiles/testCard.cpp
	$(CC) $(CFLAGS) -c $< -o $@

testDeck.o: testFiles/testDeck.cpp
	$(CC) $(CFLAGS) -c $< -o $@

Frank: 45s.o card.o deck.o main.o computer.o player.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

tests: 45s.o card.o deck.o player.o testCard.o testDeck.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o Frank tests
