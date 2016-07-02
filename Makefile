EXECUTABLE=webcrawler
#gcc my_program.c `pkg-config --cflags --libs gumbo`
CC=g++
CFLAGS=-c -Wall -std=c++11 -pthread -g -I/usr/local/include  -L/usr/local/lib $(mysql_config --cflags)
LDFLAGS=-g -I/usr/local/include  -L/usr/local/lib
LIBS=-lcurl -lgumbo -lmysqlclient -lpthread -lmysqlcppconn
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
