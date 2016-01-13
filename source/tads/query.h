#ifndef QUERY_H
#define QUERY_H
#include <unordered_map>
#include <vector>
using namespace std;

struct Query
{
	int number;
	vector<string> content; //terms of query without stopwords
	int rel_total; //total of relevants
	unordered_map<int,int> rel_docs; //identification of each relevant document
};

struct Score
{
	string document;
	double similarity;
};

#endif