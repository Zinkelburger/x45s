CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -O3

.PHONY: all clean lint

all: Frank lint

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

Frank: x45s.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o Frank
