#ifndef QPARSER_H
#define QPARSER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include "document.h"

using namespace std;

enum Tag {QN, QU, NR, RD, NIL};

class Qparser
{
public:
	Qparser(string path);
	~Qparser();
	int Process();
	vector<Document *>* GetCollection();
private:
	string base_path, field;
	vector<Document *> *documents;
	unordered_map <string,Tag> tag, sel_tags;
	fstream fs;

	void InitializeMapTag();
	void InitializeMapSelTags();
	bool InMapSelTags(string str_tag);
	Tag ConvertStringToTag(string str_tag);
	int ReadDocument();
	string ReadField();
	void Return1Line(long line_size);
};

#endif