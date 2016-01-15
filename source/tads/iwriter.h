#ifndef IWRITER_H
#define IWRITER_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "index.h"
#include "util.h"

using namespace std;


class Iwriter
{
public:
	Iwriter(string filename, unordered_map<string,Term*> *terms);
	~Iwriter();
	void Generate();

private:
	string path_ref, path_dat;
	unordered_map<string,Term*> *terms;

	void Initialize(string filename);
};



#endif