#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include "document.h"
#include "util.h"

using namespace std;

enum Tag {PN, RN, AN, AU, TI, SO, MJ, MN, AB, RF, CT, NIL};

class Parser
{

private:
	string base_path, field; //path without id file
	fstream fs;
	Tag current_tag;
	unordered_map <string,Tag> tag;
	unordered_map <string, Word *> *hash; //hash map of words
	vector<Document *> *documents;

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