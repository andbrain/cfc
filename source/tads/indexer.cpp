#include "indexer.h"

Indexer::Indexer(vector<Document *>* base)
{
	cout << "Creating indexer" << endl;
	this->base = base;
	this->hash = new Hash_Table(base->size());
}

Indexer::~Indexer()
{
	delete this->hash;
	cout << "Deleting indexer" << endl;
}

void Indexer::Initialize()
{
	LoadStopWords();

	//hash of gains
	gains["MN"] = 0.05;
	gains["MJ"] = 0;
	gains["TI"] = 0;
}

void Indexer::LoadStopWords()
{
	fstream fwords(STOPWORDS_FILE, ios::in);

	string line;

	while(getline(fwords,line))
		stopwords[line] = 1;
	
	fwords.close();
}

bool Indexer::IsStopWords(string word)
{
	string word_lower = StrToLower(word);
	unordered_map<string,int>::const_iterator got = stopwords.find(word_lower);

	if(got != stopwords.end())
		return true;

	return false;
}

void Indexer::Process()
{
	cout << "Processing..." << endl;

	vector<Document *>::iterator it = base->begin();
	string doc_id;

	for(it; it!= base->end(); ++it)
	{
		doc_id = (*it)->GetAttribute("RN");

		map<string,string>::iterator attr = (*it)->Begin();

		for(attr; attr != (*it)->End(); ++attr)
		{
			SplitField(doc_id, attr->first, attr->second);
		}
	}

	hash->Calculate_IDF_Norma();
}


void Indexer::SplitField(string id, string field, string line)
{
	string::iterator it;
	string word = "";
	unordered_map<string,double>::iterator g;
	for(it = line.begin(); it != line.end(); ++it)
	{
		//TODO:: check if is necessary to evaluate symbol '-'. Eg: bottle-neck
		//limit of 40 characteres per word
		if (isalpha(*it) && word.size() < 40)
			word += *it;
		else
		{
			if(word != "" && !IsStopWords(word) && word.size() > 2)
			{
				g = gains.find(field);
				if(g == gains.end())
					hash->AddContent(word, id);
				else
					hash->AddContent(word, id, gains[field]);
			}
			
			word = "";
		}
	}
}

void Indexer::Print()
{
	hash->Print();
}

void Indexer::WriteIndexFile()
{
	//norma
	InitFile(NORMA);
	unordered_map<string,double>* norma = hash->GetNorma();
	fstream fs_ref(NORMA, ios::out | ios::app);
	unordered_map<string,double>::iterator it = norma->begin();	

	for (it; it != norma->end(); ++it)
	{
		fs_ref << it->first << " " << it->second << endl;
	}

	fs_ref.close();

	//index file
	Iwriter *iw = new Iwriter("base", hash->GetHash_Table());
	iw->Generate();
	delete iw;
}


