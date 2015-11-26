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
	string GetAU();
	string GetTI();
	string GetAB();
	void SetRN(string rn);
	void SetAU(string au);
	void SetTI(string ti);
	void SetAB(string ab);

private:
	string record_number;
	string title;
	string abstract;
	string author;
};

#endif