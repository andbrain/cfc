#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <unordered_map>

using namespace std;


class Hash_Table
{
public:
	Hash_Table();
	~Hash_Table();
	void AddContent(string str, string doc_id);
	void Print();

private:
	struct Document
	{
		string id;
		int frequence;
		Document *next;
	};

	struct Term
	{
		string content;
		Term *next;
		Document *document;
	};
	unordered_map<string,Term *> *hash_map;
};


#endif