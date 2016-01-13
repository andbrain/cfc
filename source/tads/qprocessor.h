#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include "document.h"
#include "query.h"
#include "util.h"

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
	vector<Document *>* base;
	vector<Query *> queries;
	unordered_map<string,int> stopwords;

	void SelectRelDocs(unordered_map<int,int> *docs, string listDocs);
	void LoadStopWords();
	bool IsStopWords(string word);
	vector<string> SelectWords(string question);
};

#endif