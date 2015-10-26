#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>

using namespace std;

class Document
{

public:
	Document();
	~Document();

	string GetRN();
	string GetTI();
	string GetAB();
	void SetRN(string rn);
	void SetTI(string ti);
	void SetAB(string ab);

private:
	string record_number;
	string title;
	string abstract;
};

#endif