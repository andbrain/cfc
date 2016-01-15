#include "qprocessor.h"

Qprocessor::Qprocessor(vector<Document *>* base)
{
	this->base = base;

	ir = new Ireader("base");
	ir->Process();
	RetrieveNorma();
}

Qprocessor::~Qprocessor()
{
	for (int i = 0; i < queries.size(); ++i)
	{
		delete queries[i];
	}

	delete ir;
}

void Qprocessor::RetrieveNorma()
{
	fstream fs(NORMA, ios::in);
	string line;
	vector<string> v;

	while(getline(fs,line))
	{
		v = Split(line, ' ');
		if(v.size() == 2)
			norma.insert(pair<string,double>(v[0], atof(v[1].c_str())));
		else if(v.size() == 3)
			norma.insert(pair<string,double>(v[0], atof(v[2].c_str())));
	}

	fs.close();
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
	int counter = 1;
	//Proccess each query
	for (std::vector<Query *>::iterator q = queries.begin(); q != queries.end(); ++q)
	// for (std::vector<Query *>::iterator q = queries.begin(); q != queries.begin()+1; ++q)
	{
		cout << endl;
		cout << "Query: " << counter++ << endl;
		ProcessQuery(*q);
	}

	PrintResults();
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
		if(inv_list == NULL)
			continue;
		
		MeasureSim(inv_list, &weight, &norma, &ranking);
		DeleteTerm(inv_list);
	}

	CalculateSimilarity( &weight, &norma, &ranking);
	CreateRanking(&ranking);
	GenerateMeasures(&ranking, &query->rel_docs);
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
}
	
void Qprocessor::DeleteRanking(vector<Score *> *ranking)
{
	for (int i = 0; i < ranking->size(); ++i)
	{
		delete ranking->at(i);
	}
}

void Qprocessor::GenerateMeasures(vector<Score *> *ranking, unordered_map<int,int> *rel_docs)
{
	double p10, map;
	p10 = PN(ranking, rel_docs);
	map = MAP(ranking, rel_docs);

	pn.push_back(p10);
	mapp.push_back(map);
}

double Qprocessor::PN(vector<Score *> *ranking, unordered_map<int,int> *rel_docs)
{
	int counter=0;
	int n = 10; //p@10
	double result;

	unordered_map<int,int>::iterator rel;
	for (vector<Score *>::iterator doc = ranking->begin(); doc != ranking->begin()+n; ++doc)
	{
		rel = rel_docs->find(atoi((*doc)->document.c_str()));

		if(rel != rel_docs->end())
			counter++;
	}

	result = (double)counter/n;
	cout << "Relevantes encontrados ate 10: " << counter << endl;
	cout << "P@" << n << ": " << result*100 << "\%" << endl;

	return result;
}

double Qprocessor::MAP(vector<Score *> *ranking, unordered_map<int,int> *rel_docs)
{
	double result, acum = 0;
	int counter = 0;
	int relevants = rel_docs->size();
	unordered_map<int,int>::iterator rel;

	for (int i = 0; i < ranking->size(); ++i)
	{
		rel = rel_docs->find(atoi(ranking->at(i)->document.c_str()));

		if(rel != rel_docs->end())
		{
			counter++;
			acum += (double)counter / (i+1); //start in 0
		}
	}

	result = (double)acum / relevants;
	cout << "Todos relevantes encontrados: " << counter << endl;
	cout << "MAP: "<< result*100 << "\%" << endl;

	return result;
}

void Qprocessor::PrintResults()
{
	cout << endl;
	cout << endl;

	double res_pn, ac_pn = 0;
	double res_map, ac_map = 0;
	int tam = pn.size();

	for (int i = 0; i < tam; ++i)
	{
		ac_pn += pn[i];
		ac_map += mapp[i];
	}

	res_pn = ac_pn / tam;
	res_map = ac_map / tam;
	cout << "Consultas: " << tam << endl;
	cout << "******Final Results******" << endl;
	cout << "P@10" << ": " << res_pn*100 << "\%" << endl;
	cout << "MAP: "<< res_map*100 << "\%" << endl;
}
