#include "qprocessor.h"

Qprocessor::Qprocessor(vector<Document *>* base)
{
	this->base = base;

	ir = new Ireader("base");
	ir->Process();
}

Qprocessor::~Qprocessor()
{
	for (int i = 0; i < queries.size(); ++i)
	{
		delete queries[i];
	}

	delete ir;
}

void Qprocessor::Initialize()
{
	LoadStopWords();

	Query *query;

	for (vector<Document *>::iterator i = base->begin(); i != base->end(); ++i)
	{
		query = new Query();
		query->number = atoi((*i)->GetAttribute("QN").c_str());
		query->content = SelectWords((*i)->GetAttribute("QU"));
		query->rel_total = atoi((*i)->GetAttribute("NR").c_str());
		unordered_map<int,int> docs;
		SelectRelDocs(&docs, (*i)->GetAttribute("RD"));
		query->rel_docs = docs;

		queries.push_back(query);
	}
}

void Qprocessor::SelectRelDocs(unordered_map<int,int> *docs, string listDocs)
{
	string::iterator it;
	string number = "";
	bool get = true;

	for(it = listDocs.begin(); it != listDocs.end(); ++it)
	{
		if (isdigit(*it) && number.size() < 10)
			number += *it;
		else
		{
			if(number != "")
			{
				if(get)
				{
					docs->insert(pair<int,int>(atoi(number.c_str()), 1));
					get = false;
				}
				else 
					get = true;

			}

			number = "";
		}
	}
}

void Qprocessor::LoadStopWords()
{
	fstream fwords(STOPWORDS_FILE, ios::in);

	string line;

	while(getline(fwords,line))
		stopwords[line] = 1;
	
	fwords.close();
}

bool Qprocessor::IsStopWords(string word)
{
	string word_lower = StrToLower(word);
	unordered_map<string,int>::const_iterator got = stopwords.find(word_lower);

	if(got != stopwords.end())
		return true;

	return false;
}

vector<string> Qprocessor::SelectWords(string question)
{
	string::iterator it;
	string word = "";
	vector<string> terms;

	for(it = question.begin(); it != question.end(); ++it)
	{
		if (isalpha(*it) && word.size() < 40)
			word += *it;
		else
		{
			if(word != "" && !IsStopWords(word))
				terms.push_back(StrToLower(word));			
			word = "";			
		}
	}

	return terms;
}

int Qprocessor::Process()
{
	//Proccess each query
	// for (std::vector<Query *>::iterator q = queries.begin(); q != queries.end(); ++q)
	for (std::vector<Query *>::iterator q = queries.begin(); q != queries.begin()+1; ++q)
	{
		ProcessQuery(*q);
	}
}

int Qprocessor::ProcessQuery(Query *query)
{
	unordered_map<string,double> weight;
	unordered_map<string,double> norma;
	vector<Score *> ranking;

	std::vector<string> words = query->content;
	for (std::vector<string>::iterator w = words.begin(); w != words.end(); ++w)
	{
		Term* inv_list = ir->GetStruct(*w);
		MeasureSim(inv_list, &weight, &norma, &ranking);
		DeleteTerm(inv_list);
	}

	CalculateSimilarity( &weight, &norma, &ranking);
	CreateRanking(&ranking);
	DeleteRanking(&ranking);
}

int Qprocessor::MeasureSim(Term *term, unordered_map<string,double> *weight, unordered_map<string,double> *norma, vector<Score *> *ranking)
{
	Doc *cur = term->document;
	double word_idf = term->idf;
	CalculateParcials(word_idf, cur, weight, norma);

	while(cur->next != NULL)
	{
		cur = cur->next;
		CalculateParcials(word_idf, cur, weight, norma);
	}	
}

int Qprocessor::CalculateParcials(double idf, Doc *doc, unordered_map<string,double> *weight, unordered_map<string,double> *norma)
{
	double wDoc = idf*doc->frequence; // tf*idf of doc
	double weight_parc = wDoc * idf; // idf of query, assuming tf of query = 1. weight parcial for sum
	double norma_parc = pow(wDoc,2); // norma parcial
	
	string id(doc->id);
	IncreaseParcial(weight, id, weight_parc);
	IncreaseParcial(norma, id, norma_parc);
}

void Qprocessor::IncreaseParcial(unordered_map<string,double> *hash, string id, double value)
{
	unordered_map<string,double>::iterator w = hash->find(id);
	
	if(w == hash->end())
	{
		hash->insert(pair<string,double>(id, value));
	}
	else
	{
		double aux = hash->at(id);
		hash->insert(pair<string,double>(id, aux + value));
	}
}

void Qprocessor::DeleteTerm(Term *term)
{
	Doc *aux;
	while(term->document->next != NULL)
	{
		aux = term->document;
		term->document = term->document->next;
		delete aux;
	}

	delete term->document;
	delete term;	
}

void Qprocessor::CalculateSimilarity(unordered_map<string,double> *weight, unordered_map<string,double> *norma, vector<Score *> *ranking)
{
	unordered_map<string,double>::iterator n,w = weight->begin();
	double norma_sqrt, sim;
	Score *score;
	for (w; w != weight->end(); ++w)
	{
		n = norma->find(w->first);
		norma_sqrt = sqrt(n->second);
		sim = w->second / norma_sqrt;
		
		score = new Score();
		score->document = w->first;
		score->similarity = sim;

		ranking->push_back(score);
	}
}
	
void Qprocessor::CreateRanking(vector<Score *> *ranking)
{
	sort(ranking->begin(), ranking->end(), sortBysim);

	for (vector<Score*>::iterator i = ranking->begin(); i != ranking->end(); ++i)
	{
		cout << (*i)->document << " " << (*i)->similarity << endl;
	}
}
	
void Qprocessor::DeleteRanking(vector<Score *> *ranking)
{
	for (int i = 0; i < ranking->size(); ++i)
	{
		delete ranking->at(i);
	}
}
