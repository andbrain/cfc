#include "util.h"

string StrToLower(string str)
{
	for (unsigned i = 0; i < str.size(); i++)
	{
		char b=str[i];
		if ((b>='A')&&(b<='Z'))
			str[i]=b-'A'+'a';
	}

	return str;
}

double CalculateIDF(int N, int n)
{
	//N => number of documents
	//n => number of documents in which term occurred
	double number = (double)N/n;
	return log(number)/log(2);
}
