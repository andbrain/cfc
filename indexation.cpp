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
	// vector<Document *>::iterator it = collection->begin();

	// int rn, aux = 0, counter=0;
	// for(it; it!= collection->end(); ++it)
	// {
	// 	rn = atoi((*it)->GetAttribute("RN").c_str());

	// 	if(rn != aux+1){
	// 		cout << "[GAP]" << endl;
	// 		counter++;
	// 	}
	// 	aux = rn;

	// 	cout << "RN: " << rn << endl;
	// }

	cout << "Size of vector: " << collection->size() << endl;	
	// cout << "Quantity of gaps: " << counter << endl;
	delete indexer;
	delete parser;
}