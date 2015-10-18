#include "hash_table.h"

Hash_Table::Hash_Table()
{
	cout << "Starting hash table" << endl;
	hash_map = new unordered_map<string,Term *>();
}

Hash_Table::~Hash_Table()
{

	unordered_map<string,Term *>::iterator it = hash_map->begin();
	Document *doc;
	Term *term, *aux;

	for (it; it != hash_map->end(); ++it)
	{
		//deleting all term into the hash_table
		term = it->second;

		do
		{
			aux = term->next;
			do
			{
				doc = term->document;
				term->document = doc->next;
				delete doc;
			}while(term->document != NULL);
		
			delete term;
			term = aux;
		}while(term != NULL);

	}
	cout << "Finishing hash table" << endl;	
}

void Hash_Table::AddContent(string str, string doc_id)
{
	unordered_map<string,Term *>::iterator it = hash_map->find(str);
	Document *doc;

	if (it == hash_map->end())
	{
		//new string term
		Document *doc = new Document();
		doc->id = doc_id;
		doc->frequence = 1;
		doc->next = NULL;

		Term *term = new Term();
		term->content = str;
		term->document = doc;
		term->next = NULL;

		hash_map->insert(pair<string,Term *>(str,term));
	}
	else
	{
		//already have string term inside the hash table
		Term *term = it->second;

		while(term != NULL)
		{
			if (term->content == str)
			{
				doc = term->document;

				if(doc->id == doc_id)
				{
					//add frequence to document
					doc->frequence++;
				}
				else
				{
					//add new document as first
					Document *new_doc = new Document();
					new_doc->id = doc_id;
					new_doc->frequence = 1;
					new_doc->next = doc;
					term->document = new_doc;
				}
				break;
			}

			//TODO:: Get know in how to resolve collides or maybe unordered map encapsule it for us...
			term = term->next;			
		}

	}
}

void Hash_Table::Print()
{
	unordered_map<string,Term *>::iterator it = hash_map->begin();
	Term *term;
	Document *doc;
	int bucket, size_bucket;

	cout << "$$$ Elements of hash table $$$" << endl;
	cout << endl;

	for (it; it != hash_map->end(); ++it)
	{
		cout << "[" << it->first << "]:" << endl;
		
		cout << "\t";
		bucket = hash_map->bucket(it->first);
		cout << "bucket #" << bucket << endl;

		cout << "\t";
		size_bucket = hash_map->bucket_size(bucket);
		cout << "bucket size:" << size_bucket << endl;

		term = it->second;
		cout << "\t";
		cout << "Content => "<< term->content << endl;

		doc = term->document;

		do{
			cout << "\t";
			cout << "\t";
			cout << "[Document]: " << endl;
			cout << "\t";
			cout << "\t";
			cout << "\t";
			cout << "id => " << doc->id << endl;
			cout << "\t";
			cout << "\t";
			cout << "\t";
			cout << "frequence => " << doc->frequence << endl;
			doc = doc->next;
		}while(doc != NULL);
	}
}

