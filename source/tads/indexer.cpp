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
}

void Indexer::LoadStopWords()
{
	fstream fs(STOPWORDS_FILE);

	string line;

	while(getline(fs,line))
		stopwords[line] = 1;

	fs.close();
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
			SplitField(doc_id, attr->second);
		}
	}
}


void Indexer::SplitField(string id, string line)
{
	string::iterator it;
	string word = "";

	for(it = line.begin(); it != line.end(); ++it)
	{
		//TODO:: check if is necessary to evaluate symbol '-'. Eg: bottle-neck
		//limit of 40 characteres per word
		if (isalpha(*it) && word.size() < 40)
			word += *it;
		else
		{
			if(word != "" && !IsStopWords(word))
			{
				// word = MakeStemming(word);
				hash->AddContent(word, id);				
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
	Iwriter *iw = new Iwriter("base", hash->GetHash_Table());
	iw->Generate();

	delete iw;
}


