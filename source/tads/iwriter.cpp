#include "iwriter.h"

Iwriter::Iwriter(string filename, unordered_map<string,Term*> *terms)
{
	this->terms = terms;
	Initialize(filename);
}

Iwriter::~Iwriter()
{
}

void Iwriter::Initialize(string filename)
{
	path_ref = filename + ".ref";
	path_dat = filename + ".dat";
	InitFile(path_ref);
	InitFile(path_dat);
}

void Iwriter::Generate()
{
	unordered_map<string, Term*>::iterator it = terms->begin();
	fstream fs_ref(path_ref, ios::out | ios::app);
	fstream fs_dat(path_dat, ios::out | ios::app | ios::binary);
	unsigned long long int pos = 0;

	for (it; it != terms->end(); ++it)
	{
		fs_ref << it->first << " " << pos << endl;
		pos += sizeof(Term);
		fs_dat.write((char*)it->second, sizeof(Term));

		//write information cells
		Doc *infor = it->second->document;
		pos += sizeof(Doc);
		fs_dat.write((char*)infor, sizeof(Doc));	

		while(infor->next != NULL)
		{
			Doc *cur = infor->next;
			pos += sizeof(Doc);
			fs_dat.write((char*)cur, sizeof(Doc));
			infor = cur;
		}
	}

	fs_ref.close();
	fs_dat.close();
}