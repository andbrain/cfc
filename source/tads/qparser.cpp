#include "qparser.h"

Qparser::Qparser(string path)
{
	cout << "..Starting query parser" << endl;
	this->base_path = path;
	this->documents = new vector<Document *>();
	InitializeMapTag();
	InitializeMapSelTags();
	field = "";
}
Qparser::~Qparser()
{
	vector<Document *>::iterator it = documents->begin();

	for(it; it!= documents->end(); ++it)
		delete (*it);
	cout << "..Finishing query parser" << endl;	
}

void Qparser::InitializeMapTag()
{
	tag["QN"] = QN;
	tag["QU"] = QU;
	tag["NR"] = NR;
	tag["RD"] = RD;	
}

void Qparser::InitializeMapSelTags()
{
	sel_tags["QN"] = QN;
	sel_tags["QU"] = QU;
	sel_tags["NR"] = NR;
	sel_tags["RD"] = RD;
}

bool Qparser::InMapSelTags(string str_tag)
{
	unordered_map<string,Tag>::const_iterator it;
	
	if (sel_tags.find(str_tag) != sel_tags.end())
		return true;
	
	return false;
}


Tag Qparser::ConvertStringToTag(string str_tag)
{
	unordered_map<string,Tag>::const_iterator it;
	
	if (tag.find(str_tag) != tag.end())
		return tag[str_tag];
	else
		return NIL;
}

vector<Document *>* Qparser::GetCollection()
{
	return documents;
}

int Qparser::Process()
{
	string tag,line;

	fs.open(base_path, ios::in);
	ReadDocument();
	fs.close();

	return 0;
}

int Qparser::ReadDocument()
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

			if(ConvertStringToTag(str_tag) == QN)
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

string Qparser::ReadField()
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

void Qparser::Return1Line(long line_size)
{
	long tam = fs.tellg();
	long line = line_size + 1;
	fs.seekg(tam-line);
}