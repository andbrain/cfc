#include "ireader.h"

Ireader::Ireader(string filename)
{
	path_ref = filename + ".ref";
	path_dat = filename + ".dat";

	references = new unordered_map<string,double>();
}

Ireader::~Ireader()
{
	delete references;
}

vector<string> &split(const std::string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> Ireader::Split(const string &s,  char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void Ireader::Process()
{
	ReadReferences();
	ReadData();
}

void Ireader::ReadReferences()
{
	fstream fs(path_ref, ios::in);
	string line;
	vector<string> v;

	while(getline(fs,line))
	{
		v = Split(line, ' ');
		//TODO:: see how to convert string to double
		references->insert(pair<string,double>(v[0], atoi(v[1].c_str())));
	}
}

void Ireader::ReadData()
{

}

feature* Ireader::GetStruct(string term)
{

	unordered_map<string,double>::iterator it = references->find(term);
	feature *f = NULL;
	
	if(it != references->end())
	{
		fstream fs(path_dat, ios::in | ios::binary);
		double pos = it->second;

		// f = (struct feature*)malloc(sizeof(struct feature));
		f = new feature();
		fs.seekg(pos);
		fs.read((char*)f, sizeof(feature));
		//TODO:: try to use reinterpret_cast to convert strings
		fs.close();
		// free(f);
		return f;
	}

	return f;
}
