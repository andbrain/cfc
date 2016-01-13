#include <iostream>
#include "tads/ireader.h"
#include "tads/qparser.h"
using namespace std;

int main()
{
	cout << "Engine starting" << endl;

	Qparser *qp = new Qparser("base/cfquery");
	qp->Process();
	vector<Document *>*docs = qp->GetCollection();

	delete qp;
	cout << "Engine finishing" << endl;
}