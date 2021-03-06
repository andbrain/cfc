#ifndef QPROCESSOR_H
#define QPROCESSOR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <sys/time.h>

#include "document.h"
#include "query.h"
#include "util.h"
#include "ireader.h"
#include "index.h"

#define STOPWORDS_FILE "source/resources/stopwords.dat"
#define NORMA "norma.ref"
#define RESULT "result.txt"
#define RANKING "ranking.txt"

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
	vector<double> pn,mapp;
	fstream fResult,fRanking;
	unordered_map<string,int> stopwords;
	unordered_map<string,double> norma;

	//initializing processor
	void RetrieveNorma();
	void SelectRelDocs(unordered_map<int,int> *docs, string listDocs);
	void LoadStopWords();
	bool IsStopWords(string word);
	vector<string> SelectWords(string question);

	//proccess of queries
	int ProcessQuery(Query *query);
	int MeasureSim(Term *term, unordered_map<string,double> *weight, vector<Score *> *ranking);
	void DeleteTerm(Term *term);
	int CalculateParcials(double idf, double gain, Doc *doc, unordered_map<string,double> *weight);
	void CalculateSimilarity(unordered_map<string,double> *weight, vector<Score *> *ranking);
	void CreateRanking(vector<Score *> *ranking);
	void DeleteRanking(vector<Score *> *ranking);
	void GenerateMeasures(vector<Score *> *ranking, unordered_map<int,int> *rel_docs);
	double PN(vector<Score *> *ranking, unordered_map<int,int> *rel_docs);
	double MAP(vector<Score *> *ranking, unordered_map<int,int> *rel_docs);
	void PrintResults();
};

#endif