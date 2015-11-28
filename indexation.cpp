#include <iostream>
#include <cstdlib>
#include "parser.h"
#include "indexer.h"

using namespace std;

int main()
{
	Parser *parser = new Parser("base/");
	parser->Process();
	vector<Document *>* collection = parser->GetCollection();
	Indexer *indexer = new Indexer(collection);
	indexer->Initialize();
	indexer->Process();


	delete indexer;
	delete parser;
}