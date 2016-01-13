#include <iostream>
#include "tads/qparser.h"
#include "tads/qprocessor.h"
#include "tads/ireader.h"
using namespace std;

int main()
{
	cout << "Engine starting" << endl;

	Qparser *parser = new Qparser("base/cfquery");
	parser->Process();
	Qprocessor *proc = new Qprocessor(parser->GetCollection()); 
	proc->Initialize();
	proc->Process();

	delete proc;
	delete parser;
	cout << "Engine finishing" << endl;
}