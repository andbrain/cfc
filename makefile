install: compile
compile:
	cd source && make
indexer:
	cd source && make indexer
engine:
	cd source && make engine
clean:
	rm engine indexer base.dat base.ref && cd source && make clean
init:
	mkdir base %% cd source && mkdir objs