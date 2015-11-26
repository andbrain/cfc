BIN_INDEXER = indexer
BIN_ENGINE_SEARCH = engine

OBJS_INDEXER = objs/parser.o objs/indexer.o objs/document.o objs/inverted_file.o
OBJS_ENGINE = objs/engine.o

CC = g++
DEBUG = -g
CFLAGS = -c -std=c++11
LFLAGS = -Wall

all: indexer engine 

indexer: parser.o
	$(CC) $(LFLAGS) $(OBJS_INDEXER) $(DEBUG) -o $(BIN_INDEXER)
engine: engine.o
	$(CC) $(LFLAGS) $(OBJS_ENGINE) $(DEBUG) -o $(BIN_ENGINE_SEARCH)

parser.o: indexer.o inverted_file.o document.o parser.cpp
	$(CC) $(CFLAGS) parser.cpp -o objs/parser.o
inverted_file.o: inverted_file.cpp
	$(CC) $(CFLAGS) inverted_file.cpp -o objs/inverted_file.o
document.o:
	$(CC) $(CFLAGS) document.cpp -o objs/document.o
indexer.o: indexer.cpp parser.cpp
	$(CC) $(CFLAGS) indexer.cpp -o objs/indexer.o

engine.o: engine.cpp
	$(CC) $(CFLAGS) engine.cpp -o objs/engine.o

clean:
	rm *.o