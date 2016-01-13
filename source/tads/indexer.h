#ifndef INDEXER_H
#define INDEXER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "document.h"
#include "hash_table.h"
#include "util.h"
#include "iwriter.h"

#define STOPWORDS_FILE "source/resources/stopwords.dat"

using namespace std;

class Indexer
{
public:
	Indexer(vector<Document *>* base);
	~Indexer();
	void Initialize();
	void Process();
	void Print();
	void WriteIndexFile();
private:
	vector<Document *>* base;
	Hash_Table *hash;
	unordered_map<string,int> stopwords;
	void LoadStopWords();
	bool IsStopWords(string word);
	void SplitField(string id, string line);
};

#endif