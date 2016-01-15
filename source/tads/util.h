#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "query.h"

using namespace std;

string StrToLower(string str);
double CalculateIDF(int N, int n);
bool sortBysim( Score *lhs, Score *rhs);
bool sortBydoc( Score *lhs, Score *rhs);
void InitFile(string path);
vector<string> Split(const string &s,  char delim);
string Trim(string &s);

#endif