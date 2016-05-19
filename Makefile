EXECUTABLE=webcrawler
#gcc my_program.c `pkg-config --cflags --libs gumbo`
CC=g++
CFLAGS=-c -Wall -std=c++11 -g #-I/usr/local/include
LDFLAGS=-g # -L/usr/local/lib -lgumbo
LIBS=-lcurl
DEPS=$(wildcard *.h)
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean run

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

run:
	./$(EXECUTABLE)
