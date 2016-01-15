#ifndef INDEX_H
#define INDEX_H

using namespace std;

struct Doc
{
	char id[6]; //record number with length max of 6
	int frequence = 1;
	Doc *next;
};

struct Term
{
	char content[30]; //terms in length max of 30 characters
	int frequence = 1;
	double idf;
	double gain = 1;
	Doc *document;
};

#endif