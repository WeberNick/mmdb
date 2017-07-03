#ifndef INFRA_WEBE_STRING_HELPER_HH
#define INFRA_WEBE_STRING_HELPER_HH

#include "types.hh"

#include <string>
#include <vector>

bool hasSuffix(const std::string aString, const std::string aSuffix);

string_vt tokenize(const char* aString, const char aCharToSplit);

void removeSuffix(std::string& aString, const char aChar = '.');





#endif