#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "document.h"
#include "query.h"
#include "util.h"
#include "ireader.h"
#include "index.h"

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
	Ireader *ir;
	vector<Query *> queries;
	vector<Document *>* base;
	unordered_map<string,int> stopwords;	

	//initializing processor
	void SelectRelDocs(unordered_map<int,int> *docs, string listDocs);
	void LoadStopWords();
	bool IsStopWords(string word);
	vector<string> SelectWords(string question);

	//proccess of queries
	int ProcessQuery(Query *query);
	int MeasureSim(Term *term);
	void DeleteTerm(Term *term);
	int CalculateParcials(double idf, Doc *doc, unordered_map<string,double> *weight, unordered_map<string,double> *norma);
	void IncreaseParcial(unordered_map<string,double> *hash, string id, double value);
	void CalculateSimilarity(unordered_map<string,double> *weight, unordered_map<string,double> *norma, 
		vector<Score *> *ranking);
	void CreateRanking(vector<Score *> *ranking);
	void DeleteRanking(vector<Score *> *ranking);
};

#endif