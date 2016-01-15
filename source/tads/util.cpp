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

double CalculateWeight1(double tf, double idf)
{
	double result = tf * idf;

	return result;
}

double CalculateWeight2(double tf, double idf, double gain)
{
	double result = idf * (1 + log(tf * gain)/log(2)) ;

	return result;
}

double CalculateWeight3(double tf, double max_tf, double idf)
{
	double final_tf = 0.5 + 0.5*((double)tf / max_tf);
	double result = final_tf * idf;
	
	return result;
}


bool sortBysim( Score *lhs, Score *rhs) 
{ 
	return lhs->similarity > rhs->similarity; 
}

bool sortBydoc( Score *lhs, Score *rhs) 
{ 
	return lhs->document > rhs->document; 
}

void InitFile(string path)
{
	fstream fsin(path, ios::in);

	if(fsin.is_open())
		remove(path.c_str());

	fsin.close();
	fsin.open(path, ios::in | ios::out | ios::app);
	fsin.close();	
}

vector<string> &split(const std::string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> Split(const string &s,  char delim) {
    vector<string> elems;
    split(s, delim, elems);
    
    return elems;
}

// trim from begin
string &ltrim(string &s)
{
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    
    return s;
}

// trim from end
string &rtrim(string &s)
{
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    
    return s;
}

// trim from both ends
string Trim(string &s)
{
    return ltrim(rtrim(s));
}