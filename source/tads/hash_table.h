#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <unordered_map>
#include <cstring>
#include <cmath>
#include <fstream>

#include "util.h"
#include "index.h"

#define VOCAB "vocabulary.txt"

using namespace std;


class Hash_Table
{
public:
	Hash_Table(int total_documents);
	~Hash_Table();
	void AddContent(string str, string doc_id, double gain = 0);
	void SetTotalDocuments(int total);
	void Print();
	unordered_map<string,Term *>* GetHash_Table();
	unordered_map<string,double>* GetNorma();
	void Calculate_IDF_Norma();

private:	
	unordered_map<string,Term *> *hash_map;
	unordered_map<string,double> *norma;
	unordered_map<string,int> max_freq;
	int total_documents;
	fstream fVocab;

	void CalculateNorma(Doc* doc, double idf, double gain);
	void ApplySqrt();

};


#endif