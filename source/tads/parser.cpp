#include "parser.h"

Parser::Parser(string dataset_path)
{
	cout << "..Starting parsing" << endl;
	this->base_path = dataset_path + "cf";
	this->documents = new vector<Document *>();
	InitializeMapTag();
	InitializeMapSelTags();
	field = "";
}

Parser::~Parser()
{
	vector<Document *>::iterator it = documents->begin();

	for(it; it!= documents->end(); ++it)
		delete (*it);
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
	tag["EX"] = EX;
	tag["RF"] = RF;
	tag["CT"] = CT;
}

void Parser::InitializeMapSelTags()
{
	sel_tags["RN"] = RN;
	// sel_tags["AU"] = AU;
	sel_tags["TI"] = TI;
	sel_tags["AB"] = AB;
	sel_tags["EX"] = EX;
	sel_tags["MJ"] = MJ;
	sel_tags["MN"] = MN;
}

bool Parser::InMapSelTags(string str_tag)
{
	unordered_map<string,Tag>::const_iterator it;
	
	if (sel_tags.find(str_tag) != sel_tags.end())
		return true;
	
	return false;
}


Tag Parser::ConvertStringToTag(string str_tag)
{
	unordered_map<string,Tag>::const_iterator it;
	
	if (tag.find(str_tag) != tag.end())
		return tag[str_tag];
	else
		return NIL;
}

vector<Document *>* Parser::GetCollection()
{
	return documents;
}

int Parser::Process()
{
	for (int i = 74; i < 80; ++i)
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
	string str_result, str_tag, str_content;
	Document *document;

	while(!fs.eof())
	{
		str_result = ReadField();
		// pattern: Tag#content

		if(str_result != "")
		{
			str_tag = str_result.substr(0,2);
			str_content = str_result.substr(3); //start from 3 to leave out the character '#'

			if(ConvertStringToTag(str_tag) == PN)
			{
				document = new Document();
				document->SetAttribute(str_tag, str_content);
				documents->push_back(document);				
			}
			else if(InMapSelTags(str_tag))
				document->SetAttribute(str_tag, str_content);
		}
	}

	return 0;
}

string Parser::ReadField()
{
	string line, str_tag;
	Tag aux_tag;

	do{
		getline(fs,line);
		str_tag = line.substr(0,2);
		aux_tag = ConvertStringToTag(str_tag);

		if(aux_tag != NIL)
			break;		
	}while( !fs.eof());
	
	if(fs.eof())
		return "";

	field = str_tag + "#";
	//start from 3 character
	field += line.substr(3);

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
				field += " " + line;
		}

		if(!fs.eof())
			Return1Line(line.size());
	}
	else
		Return1Line(line.size());

	return field;
}

void Parser::Return1Line(long line_size)
{
	long tam = fs.tellg();
	long line = line_size + 1;
	fs.seekg(tam-line);
}