#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <unordered_map>
#include <cstring>
#include <cmath>

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
	unordered_map<string,double>* GetNorma();
	void Calculate_IDF_Norma();

private:	
	unordered_map<string,Term *> *hash_map;
	unordered_map<string,double> *norma;
	int total_documents;

	void CalculateNorma(Doc* doc, double idf);
	void ApplySqrt();
};


#endif