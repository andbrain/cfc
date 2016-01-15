#ifndef IREADER_H
#define IREADER_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <string>

#include "index.h"
#include "util.h"

using namespace std;

class Ireader
{
public:
	Ireader(string filename);
	~Ireader();
	void Process();
	Term* GetStruct(string term);

private:
	string path_ref, path_dat;
	unordered_map<string, double> *references;
	void ReadReferences();

};

#endif