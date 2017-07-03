#include "string_helper.hh"

bool hasSuffix(const std::string aString, const std::string aSuffix)
{
	return aString.size() >= aSuffix.size() && aString.compare(aString.size() - aSuffix.size(), aSuffix.size(), aSuffix) == 0;
}

string_vt tokenize(const char* aString, const char aCharToSplit)
{
	string_vt result;
	do
	{
		const char *begin = aString;

		while(*aString != aCharToSplit && *aString)
			aString++;

		result.push_back(std::string(begin, aString));
	} while (0 != *aString++);
	return result;
}

void removeSuffix(std::string& aString, const char aChar)
{
	aString = aString.substr(0, aString.find_last_of(aChar)); 
}