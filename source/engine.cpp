#include <iostream>
#include "tads/ireader.h"
#include "tads/qparser.h"
#include "tads/qprocessor.h"
using namespace std;

int main()
{
	cout << "Engine starting" << endl;

	Qparser *parser = new Qparser("base/cfquery");
	parser->Process();
	Qprocessor *proc = new Qprocessor(parser->GetCollection()); 
	proc->Initialize();
	
	delete proc;
	delete parser;
	cout << "Engine finishing" << endl;
}