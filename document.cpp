#include "document.h"

Document::Document()
{

}

Document::~Document()
{

}

string Document::GetRN()
{
	return record_number;
}

string Document::GetTI()
{
	return title;
}

string Document::GetAB()
{
	return abstract;
}

void Document::SetRN(string rn)
{
	record_number = rn;
}

void Document::SetTI(string ti)
{
	title = ti;
}

void Document::SetAB(string ab)
{
	abstract = ab;
}