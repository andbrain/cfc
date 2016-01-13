#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <unordered_map>
#include <cstring>

#include "util.h"
#include "index.h"

using namespace std;


class Hash_Table
{
public:
	Hash_Table(int total_documents);
	~Hash_Table();
	void AddContent(string str, string doc_id);
	void SetTotalDocuments(int total);
	void Print();
	unordered_map<string,Term *>* GetHash_Table();
	void ProcessIDF();

private:	
	unordered_map<string,Term *> *hash_map;
	int total_documents;
};


#endif