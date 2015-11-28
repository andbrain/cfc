BIN_INDEXER = indexer
BIN_ENGINE_SEARCH = engine

OBJS_INDEXER = objs/indexation.o objs/parser.o objs/util.o objs/indexer.o objs/document.o objs/hash_table.o
OBJS_ENGINE = objs/engine.o

CC = g++
DEBUG = -g
CFLAGS = -c -std=c++11
LFLAGS = -Wall

all: indexer engine 

indexer: parser.o hash_table.o indexer.o
	$(CC) $(LFLAGS) $(OBJS_INDEXER) $(DEBUG) -o $(BIN_INDEXER)
engine: engine.o
	$(CC) $(LFLAGS) $(OBJS_ENGINE) $(DEBUG) -o $(BIN_ENGINE_SEARCH)
document.o: document.cpp
	$(CC) $(CFLAGS) document.cpp -o objs/document.o
hash_table.o: hash_table.cpp
	$(CC) $(CFLAGS) hash_table.cpp -o objs/hash_table.o
indexer.o: parser.o util.o indexer.cpp
	$(CC) $(CFLAGS) indexer.cpp -o objs/indexer.o
util.o: util.cpp
	$(CC) $(CFLAGS) util.cpp -o objs/util.o
parser.o: indexation.o document.o parser.cpp
	$(CC) $(CFLAGS) parser.cpp -o objs/parser.o
indexation.o: indexation.cpp parser.cpp indexer.cpp
	$(CC) $(CFLAGS) indexation.cpp -o objs/indexation.o
engine.o: engine.cpp
	$(CC) $(CFLAGS) engine.cpp -o objs/engine.o

clean:
	rm *.o objs/*.o