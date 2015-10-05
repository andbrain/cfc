#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

struct Document
{
	int id;
	int frequence;
	Document *next;
};

struct Word
{
	string content;
	Word *next;
	Document *document;
};


#endif