#include "document.h"

Document::Document()
{

}

Document::~Document()
{

}

string GetRN()
{
	return record_number;
}

string GetTI()
{
	return title;
}

string GetAB()
{
	return abstract;
}

void SetRN(string rn)
{
	record_number = rn;
}

void SetTI(string ti)
{
	title = ti;
}

void SetAB(string ab)
{
	abstract = ab;
}