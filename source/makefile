##
## Executables
##

BIN_INDEXER = ../indexer
BIN_ENGINE_SEARCH = ../engine

##
## Compile and Link settings
##

TADS = tads
TEMP = objs
CC = g++
DEBUG = -g
CFLAGS = -c -std=c++11
LFLAGS = -Wall

##
## Objs
##

OBJS_INDEXER = $(TEMP)/indexation.o $(TEMP)/parser.o $(TEMP)/util.o $(TEMP)/indexer.o $(TEMP)/iwriter.o $(TEMP)/document.o $(TEMP)/hash_table.o
# OBJS_INDEXER := $(wildcard objs/*.o)
OBJS_ENGINE = $(TEMP)/engine.o $(TEMP)/ireader.o $(TEMP)/qparser.o $(TEMP)/document.o $(TEMP)/qprocessor.o $(TEMP)/util.o

##
## main target
##

all: indexer engine 

##
## indexer
##

indexer: parser.o hash_table.o indexer.o indexation.o
	$(CC) $(LFLAGS) $(OBJS_INDEXER) $(DEBUG) -o $(BIN_INDEXER)

document.o:
	$(CC) $(CFLAGS) $(TADS)/document.cpp -o $(TEMP)/document.o
parser.o: document.o
	$(CC) $(CFLAGS) $(TADS)/parser.cpp -o $(TEMP)/parser.o
hash_table.o: util.o
	$(CC) $(CFLAGS) $(TADS)/hash_table.cpp -o $(TEMP)/hash_table.o
util.o:
	$(CC) $(CFLAGS) $(TADS)/util.cpp -o $(TEMP)/util.o
iwriter.o:
	$(CC) $(CFLAGS) $(TADS)/iwriter.cpp -o $(TEMP)/iwriter.o
indexer.o: parser.o util.o iwriter.o
	$(CC) $(CFLAGS) $(TADS)/indexer.cpp -o $(TEMP)/indexer.o
indexation.o: parser.o indexer.o
	$(CC) $(CFLAGS) indexation.cpp -o $(TEMP)/indexation.o

##
## Engine
##

engine: engine.o
	$(CC) $(LFLAGS) $(OBJS_ENGINE) $(DEBUG) -o $(BIN_ENGINE_SEARCH)
engine.o: engine.cpp ireader.o qparser.o document.o qprocessor.o util.o
	$(CC) $(CFLAGS) engine.cpp -o $(TEMP)/engine.o
ireader.o:
	$(CC) $(CFLAGS) $(TADS)/ireader.cpp -o $(TEMP)/ireader.o
qparser.o:
	$(CC) $(CFLAGS) $(TADS)/qparser.cpp -o $(TEMP)/qparser.o
qprocessor.o:
	$(CC) $(CFLAGS) $(TADS)/qprocessor.cpp -o $(TEMP)/qprocessor.o


##
## Misc
##

clean:
	objs/*.o