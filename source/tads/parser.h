#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include "document.h"

using namespace std;

enum Tag {PN, RN, AN, AU, TI, SO, MJ, MN, AB, EX, RF, CT, NIL};

class Parser
{
private:
	string base_path;//path without id file
	string field; //content of each field
	fstream fs;
	unordered_map <string,Tag> tag;
	unordered_map <string,Tag> sel_tags;
	vector<Document *> *documents;

	int ReadCollection(int doc);
	int ReadDocument();
	string ReadField();
	void InitializeMapTag();
	void InitializeMapSelTags();
	bool InMapSelTags(string str_tag);
	Tag ConvertStringToTag(string tag);
	void Return1Line(long line_size);

public:
	Parser(string dataset_path);
	~Parser();
	int Process();
	vector<Document *>* GetCollection();
};

#endif