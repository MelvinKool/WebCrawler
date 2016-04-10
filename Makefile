EXECUTABLE=webcrawler

CC=g++
CFLAGS=-c -Wall -std=c++11 -g
LDFLAGS=-g

DEPS=$(wildcard *.h)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

run:
	./$(EXECUTABLE)
