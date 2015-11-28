#include "document.h"

Document::Document()
{

}

Document::~Document()
{

}

string Document::GetAttribute(string attr)
{
	return attributes[attr];
}

void Document::SetAttribute(string attr, string content)
{
	attributes[attr] = content;
}