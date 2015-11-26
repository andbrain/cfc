#include <iostream>
#include "parser.h"

using namespace std;

int main()
{
	Parser *parser = new Parser("base/");
	parser->Process();
	vector<Document *>* collection = parser->GetCollection();

	vector<Document *>::iterator it = collection->begin();

	for(it; it!= collection->end(); ++it)
	{
		cout << "************************" << endl;
		cout << "RN: " << (*it)->GetRN() << endl;
		cout << "AU: " << (*it)->GetAU() << endl;
		cout << "TI: " << (*it)->GetTI() << endl;
		cout << "AB: " << (*it)->GetAB() << endl;
		cout << "************************" << endl;
	}

	cout << "Size of hash table: " << collection->size() << endl;	

	delete parser;
}