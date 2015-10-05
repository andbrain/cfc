#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include <string>
#include "util.h"

using namespace std;

enum Tag {PN, RN, AN, AU, TI, SO, MJ, MN, AB, RF, CT, NIL};

class Parser
{

private:
	string base_path; //path without id file
	fstream fs;
	unordered_map <string,Tag> tag;
	unordered_map <string, Word *> *hash; //hash map of words

	int ReadCollection(int doc);
	int ReadDocument();
	void SwitchField(Tag aux_tag, string line);
	void InitializeMapTag();
	Tag ConvertStringToTag(string tag);

public:
	Parser(string dataset_path);
	~Parser();
	int Process();
};

#endif