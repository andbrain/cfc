#ifndef IREADER_H
#define IREADER_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "index.h"

#include <vector>
#include <sstream>
#include <string>

using namespace std;

class Ireader
{
public:
	Ireader(string filename);
	~Ireader();
	void Process();
	feature* GetStruct(string term);

private:
	string path_ref, path_dat;
	unordered_map<string, double> *references;
	void ReadReferences();
	void ReadData();
	vector<string> Split(const string &s,  char delim);

};

#endif