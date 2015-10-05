#include <iostream>
#include "parser.h"

using namespace std;

int main()
{
	Parser *parser = new Parser("base/");
	parser->Process();
	delete parser;
}