#include <iostream>
#include "tads/ireader.h"

using namespace std;

int main()
{
	cout << "Engine starting" << endl;

	Ireader *ir = new Ireader("base");
	ir->Process();

	string element = "stehr";
	Term *f = ir->GetStruct(element);
	cout << f->frequence << endl;
	// cout << f->content << endl;
	free(f);
	delete ir;
	cout << "Engine finishing" << endl;
}