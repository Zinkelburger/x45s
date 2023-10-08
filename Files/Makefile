CC = g++
CFLAGS = --std=c++17 -Wall -Werror -Wextra -Wshadow -Wlogical-op -Wduplicated-branches -Wuseless-cast -Wduplicated-cond -pedantic -O3
LIB = -lboost_unit_test_framework

.PHONY: all clean lint tests

all: Frank lint tests

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

testFiles/%.o: testFiles/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

Frank: 45s.o card.o deck.o main.o computer.o player.o gameState.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

tests: 45s.o card.o deck.o player.o gameState.o testFiles/testCard.o testFiles/testDeck.o testFiles/testX45s.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

testCard.o: testFiles/testCard.cpp
	$(CC) $(CFLAGS) -c $< -o $@

testDeck.o: testFiles/testDeck.cpp
	$(CC) $(CFLAGS) -c $< -o $@

testX45s.o: testFiles/testX45s.cpp
	$(CC) $(CFLAGS) -c $< -o $@

concatenate:
	cat *.cpp > ../x45s.cpp
	cat *.hpp > ../x45s.hpp

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o Frank testFiles/*.o tests
