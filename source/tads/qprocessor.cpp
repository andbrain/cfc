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
	vector<Score *> ranking;
	std::vector<string> words = query->content;
	for (std::vector<string>::iterator w = words.begin(); w != words.end(); ++w)
	{
		Term* inv_list = ir->GetStruct(*w);
		if(inv_list == NULL)
			continue;
		
		MeasureSim(inv_list, &weight, &ranking);
		DeleteTerm(inv_list);
	}

	CalculateSimilarity( &weight, &ranking);
	CreateRanking(&ranking);
	GenerateMeasures(&ranking, &query->rel_docs);
	DeleteRanking(&ranking);
}

int Qprocessor::MeasureSim(Term *term, unordered_map<string,double> *weight, vector<Score *> *ranking)
{
	Doc *cur = term->document;
	double word_idf = term->idf;
	CalculateParcials(word_idf, cur, weight);

	while(cur->next != NULL)
	{
		cur = cur->next;
		CalculateParcials(word_idf, cur, weight);
	}	
}

int Qprocessor::CalculateParcials(double idf, Doc *doc, unordered_map<string,double> *weight)
{
	double wDoc = CalculateWeight2(doc->frequence, idf); // tf*idf of doc
	double wQuery = idf; //assuming weight of query as 1
	
	double weight_parc = wDoc * wQuery; 
		
	string id(doc->id);
	Trim(id);
	unordered_map<string,double>::iterator w = weight->find(id);
	
	if(w == weight->end())
	{
		weight->insert(pair<string,double>(id, weight_parc));
	}
	else
	{
		double aux = weight->at(id);
		w->second = aux + weight_parc;
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

void Qprocessor::CalculateSimilarity(unordered_map<string,double> *weight, vector<Score *> *ranking)
{
	unordered_map<string,double>::iterator n,w = weight->begin();
	double sim;
	string strDoc;
	Score *score;
	for (w; w != weight->end(); ++w)
	{
		strDoc = w->first;
		Trim(strDoc);
		n = norma.find(strDoc);
		sim = w->second / n->second;
		score = new Score();
		score->document = strDoc;
		score->similarity = sim;

		ranking->push_back(score);
	}
}
	
void Qprocessor::CreateRanking(vector<Score *> *ranking)
{
	sort(ranking->begin(), ranking->end(), sortBysim);

	// int counter = 1;

	// for (vector<Score *>::iterator i = ranking->begin(); i != ranking->end(); ++i)
	// {
	// 	cout << counter++ << ". " << (*i)->document << " -- " << (*i)->similarity << endl;
	// }
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
	cout << "******Final Results******" << endl;
	cout << "Total de Consultas: " << tam << endl;
	cout << "P@10" << ": " << res_pn*100 << "\%" << endl;
	cout << "MAP: "<< res_map*100 << "\%" << endl;
}
