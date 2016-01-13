#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include "document.h"
#include "query.h"

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

	void SelectRelDocs(unordered_map<int,int> *docs, string listDocs);
};

#endif