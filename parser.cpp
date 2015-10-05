#include "parser.h"

Parser::Parser(string dataset_path)
{
	cout << "..Starting parsing" << endl;
	this->base_path = dataset_path + "cf";
	this->hash = new unordered_map<string,Word *>();
	InitializeMapTag();
}

Parser::~Parser()
{
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
	string str_tag, line;
	Tag aux_tag;

	while(getline(fs, line))
	{
		str_tag = line.substr(0,2);
		aux_tag = ConvertStringToTag(str_tag);

		if(aux_tag != NIL)
			SwitchField(aux_tag, line.substr(2)); //get line withou tag
	}

	return 0;
}

void Parser::SwitchField(Tag aux_tag, string line)
{
	switch(aux_tag)
	{
		case PN:
			cout << "PN: " << line << endl;
		break;
		case AU:

		break;
		case TI:

		break;
		case AB:

		break;
	}
}





