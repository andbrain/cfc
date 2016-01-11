#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <map>

using namespace std;

class Document
{

public:
	Document();
	~Document();
	void SetAttribute(string attr, string content);
	string GetAttribute(string attr);
	map<string,string>::iterator Begin();
	map<string,string>::iterator End();
private:
	map<string,string> attributes;	
};

#endif