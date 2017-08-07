#include "string_helper.hh"

void print_header(std::ostream& aOutput, std::string aHeaderName, int aWidth)
{
	const int lLen = aHeaderName.length() + 4;
	const int lWidth = aWidth;
	const int lLeft = ((lWidth - lLen) / 2);
	const int lRight = lWidth - lLeft - lLen;

	aOutput << "\n" << std::string(lWidth, '=') << std::endl
		<< std::string(lLeft, '=') << "  " << aHeaderName << "  " << std::string(lRight, '=') << std::endl
		<< std::string(lWidth, '=') << "\n" << std::endl;
}

void print_sep(std::ostream& aOutput, int aTot)
{
	aOutput << std::string(aTot, '=') << std::endl;
}

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

