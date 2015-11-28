#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <unordered_map>

#include "util.h"

using namespace std;


class Hash_Table
{
public:
	Hash_Table(int total_documents);
	~Hash_Table();
	void AddContent(string str, string doc_id);
	void SetTotalDocuments(int total);
	void Print();

private:
	struct Doc
	{
		string id;
		int frequence = 1;
		Doc *next;
	};

	struct Term
	{
		string content;
		int frequence = 1;
		Doc *document;
	};
	unordered_map<string,Term *> *hash_map;
	int total_documents;
};


#endif