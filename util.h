#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

struct Doc
{
	int id;
	int frequence;
	Doc *next;
};

struct Word
{
	string content;
	Word *next;
	Doc *doc;
};


#endif