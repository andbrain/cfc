#include "parser.h"

Parser::Parser(string dataset_path)
{
	cout << "..Starting parsing" << endl;
	this->base_path = dataset_path + "cf";
	this->hash = new unordered_map<string,Word *>();
	this->documents = new vector<Document *>();
	InitializeMapTag();
}

Parser::~Parser()
{
	vector<Document *>::iterator it = documents->begin();

	for(it; it!= documents->end(); ++it)
		delete (*it);

	delete hash;
	cout << "..Finishing parsing" << endl;
}

void Parser::InitializeMapTag()
{
	tag["PN"] = PN;
	tag["RN"] = RN;
	tag["AN"] = AN;
	tag["AU"] = AU;
	tag["TI"] = TI;
	tag["SO"] = SO;
	tag["MJ"] = MJ;
	tag["MN"] = MN;
	tag["AB"] = AB;
	tag["RF"] = RF;
	tag["CT"] = CT;
}

Tag Parser::ConvertStringToTag(string str_tag)
{
	unordered_map<string,Tag>::const_iterator it;
	
	if (tag.find(str_tag) != tag.end())
		return tag[str_tag];
	else
		return NIL;
}

int Parser::Process()
{
	for (int i = 74; i < 75; ++i)
	{
		ReadCollection(i);
	}

	vector<Document *>::iterator it = documents->begin();

	for(it; it!= documents->end(); ++it)
	{
		cout << "************************" << endl;
		cout << "RN: " << (*it)->GetRN() << endl;
		cout << "TI: " << (*it)->GetTI() << endl;
		cout << "AB: " << (*it)->GetAB() << endl;
		cout << "************************" << endl;
	}	

	return 0;
}

int Parser::ReadCollection(int doc)
{
	string tag,line;

	fs.open(base_path + to_string(doc), ios::in);
	ReadDocument();
	fs.close();

	return 0;
}


int Parser::ReadDocument()
{	
	string result_str;
	Document *document;

	while(!fs.eof())
	{
		result_str = ReadField(RN);
		if(result_str != "")
		{
			document = new Document();
			document->SetRN(result_str);
			result_str = ReadField(TI);
			document->SetTI(result_str);
			result_str = ReadField(AB);
			document->SetAB(result_str);
			documents->push_back(document);
		}
	}

	return 0;
}

string Parser::ReadField(Tag tag_field)
{
	string line, str_tag;
	Tag aux_tag = tag_field;

	do{
		getline(fs,line);
		str_tag = line.substr(0,2);
		aux_tag = ConvertStringToTag(str_tag);
	}while(!fs.eof() && aux_tag != tag_field);
	
	if(fs.eof())
		return "";
	field = line.substr(2);

	getline(fs,line);
	str_tag = line.substr(0,2);
	aux_tag = ConvertStringToTag(str_tag);

	if(aux_tag == NIL)
	{
		if(!fs.eof())
			Return1Line(line.size());		
		
		while(!fs.eof() && aux_tag == NIL)
		{
			getline(fs,line);
			str_tag = line.substr(0,2);
			aux_tag = ConvertStringToTag(str_tag);
			
			if(aux_tag == NIL)
				field += " " + line.substr(2);
		}

		if(!fs.eof())
			Return1Line(line.size());
	}

	return field;
}

void Parser::Return1Line(long line_size)
{
	long tam = fs.tellg();
	long line = line_size + 1;
	fs.seekg(tam-line);
}