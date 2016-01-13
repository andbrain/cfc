#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include "document.h"
#include "query.h"
#include "util.h"
#include "ireader.h"

#define STOPWORDS_FILE "source/resources/stopwords.dat"

using namespace std;

class Qprocessor
{
public:
	Qprocessor(vector<Document *>* base);
	~Qprocessor();
	void Initialize();
	int Process();

private:
	vector<Query *> queries;
	vector<Document *>* base;
	unordered_map<string,int> stopwords;
	Ireader *ir;
	//initializing processor
	void SelectRelDocs(unordered_map<int,int> *docs, string listDocs);
	void LoadStopWords();
	bool IsStopWords(string word);
	vector<string> SelectWords(string question);

	//proccess of queries
	int ProcessQuery(Query *query);
};

#endif