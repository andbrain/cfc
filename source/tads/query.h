#ifndef QUERY_H
#define QUERY_H
#include <unordered_map>
using namespace std;

struct Query
{
	int number;
	string content;
	int rel_total; //total of relevants
	unordered_map<int,int> rel_docs; //identification of each relevant document
};

#endif