#ifndef IWRITER_H
#define IWRITER_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "index.h"

using namespace std;


class Iwriter
{
public:
	Iwriter(string filename, unordered_map<string,feature*> *terms);
	~Iwriter();
	void Generate();

private:
	string path_ref, path_dat;
	unordered_map<string,feature*> *terms;

	void Initialize(string filename);
	void InitFile(string path);
};



#endif