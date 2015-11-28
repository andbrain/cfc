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
