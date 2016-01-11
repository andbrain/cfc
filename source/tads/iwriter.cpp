#include "iwriter.h"

Iwriter::Iwriter(string filename, unordered_map<string,feature*> *terms)
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

void Iwriter::InitFile(string path)
{
	fstream fsin(path, ios::in);

	if(fsin.is_open())
		remove(path.c_str());

	fsin.close();
	fsin.open(path, ios::in | ios::out | ios::app);
	fsin.close();	
}

void Iwriter::Generate()
{
	unordered_map<string, feature*>::iterator it = terms->begin();
	fstream fs_ref(path_ref, ios::out | ios::app);
	fstream fs_dat(path_dat, ios::out | ios::app | ios::binary);
	double pos = 0;

	for (it; it != terms->end(); ++it)
	{
		fs_ref << it->first << " " << pos << endl;
		pos += sizeof(feature);

		fs_dat.write((char*)it->second, sizeof(feature));	
	}

	fs_ref.close();
	fs_dat.close();
}