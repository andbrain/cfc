#include "hash_table.h"

Hash_Table::Hash_Table(int total_documents)
{
	cout << "Starting hash table" << endl;
	hash_map = new unordered_map<string,Term *>();
	norma = new unordered_map<string,double>();
	this->total_documents = total_documents;

	InitFile(VOCAB);
	fVocab.open(VOCAB, ios::out | ios::app);
}

Hash_Table::~Hash_Table()
{

	unordered_map<string,Term *>::iterator it = hash_map->begin();
	Doc *doc;
	Term *term;

	for (it; it != hash_map->end(); ++it)
	{
		//deleting all term into the hash_table
		term = it->second;
		
		do
		{
			doc = term->document;
			term->document = doc->next;
			delete doc;
		}while(term->document != NULL);
	
		delete term;	
	}
	delete norma;
	fVocab.close();
	cout << "Finishing hash table" << endl;	
}

void Hash_Table::AddContent(string str, string doc_id, double gain)
{
	string word_lower = StrToLower(str);
	unordered_map<string,Term *>::iterator it = hash_map->find(word_lower);
	Doc *doc;

	Trim(doc_id);

	if (it == hash_map->end())
	{
		//new string term
		doc = new Doc();
		// doc->id = doc_id;
		strcpy(doc->id, doc_id.c_str());
		doc->next = NULL;

		Term *term = new Term();
		// term->content = word_lower;
		strcpy(term->content, word_lower.c_str());
		term->document = doc;
		term->gain += gain;
		hash_map->insert(pair<string,Term *>(word_lower,term));
	}
	else
	{
		Term *term = it->second;		
		doc = term->document;
		term->gain += gain;

		if(strcmp(doc->id,doc_id.c_str()) == 0)
		{
			//add frequence to document
			doc->frequence++;
		}
		else
		{
			//add new document as first
			Doc *new_doc = new Doc();
			// new_doc->id = doc_id;
			strcpy(new_doc->id, doc_id.c_str());
			new_doc->next = doc;
			term->document = new_doc;
			term->frequence++;
		}
	}
}

void Hash_Table::Print()
{
	unordered_map<string,Term *>::iterator it = hash_map->begin();
	Term *term;
	Doc *doc;
	int bucket, size_bucket;

	fVocab << "$$$ Elements of hash table $$$" << endl;
	fVocab << endl;

	for (it; it != hash_map->end(); ++it)
	{
		fVocab << "[" << it->first << "]:" << endl;
		
		// cout << "\t";
		// bucket = hash_map->bucket(it->first);
		// cout << "bucket #" << bucket << endl;

		// cout << "\t";
		// size_bucket = hash_map->bucket_size(bucket);
		// cout << "bucket size:" << size_bucket << endl;

		term = it->second;
		fVocab << "\t";
		fVocab << "Content => " << term->content << endl;
		fVocab << "\t";
		fVocab << "IDF => " << CalculateIDF(total_documents, term->frequence) << endl;
		fVocab << "\t";
		fVocab << "Gains => " << term->gain << endl;
		fVocab << "\t";
		fVocab << "Repetitions in docs => " << term->frequence << endl;
		
		doc = term->document;

		do{
			fVocab << "\t";
			fVocab << "\t";
			fVocab << "[Document]: " << endl;
			fVocab << "\t";
			fVocab << "\t";
			fVocab << "\t";
			fVocab << "id => " << doc->id << endl;
			fVocab << "\t";
			fVocab << "\t";
			fVocab << "\t";
			fVocab << "frequence => " << doc->frequence << endl;
			doc = doc->next;
		}while(doc != NULL);
	}
}

unordered_map<string,Term *>* Hash_Table::GetHash_Table()
{
	return hash_map;
}

void Hash_Table::Calculate_IDF_Norma()
{
	unordered_map<string,Term *>::iterator it;
	Term *term;

	it = hash_map->begin();
	for (it; it != hash_map->end(); ++it)
	{
		term = it->second;
		term->idf = CalculateIDF(total_documents, term->frequence);
		CalculateNorma(term->document, term->idf, term->gain);

		Doc* cur = term->document;
		while(cur->next != NULL)
		{
			cur = cur->next;
			CalculateNorma(cur, term->idf, term->gain);
		}

	}

	ApplySqrt();
}

void Hash_Table::CalculateNorma(Doc* doc, double idf, double gain)
{
	string id(doc->id);
	Trim(id);	

	unordered_map<string,double>::iterator it = norma->find(id);
	double wei = CalculateWeight2(doc->frequence, idf, gain);
	double wd = pow(wei, 2);

	if(it == norma->end())
	{
		norma->insert(pair<string,double>(id,wd));
	}
	else
	{
		double aux = norma->at(id);
		it->second = aux + wd;
	}
}

void Hash_Table::ApplySqrt()
{
	unordered_map<string,double>::iterator it = norma->begin();	

	for (it; it != norma->end(); ++it)
	{
		it->second = sqrt(it->second);
	}
}

unordered_map<string,double>* Hash_Table::GetNorma()
{
	return norma;
}


