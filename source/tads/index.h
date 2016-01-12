#ifndef INDEX_H
#define INDEX_H

using namespace std;

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

#endif