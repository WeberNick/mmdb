#ifndef STRING_HELPER_HH
#define STRING_HELPER_HH

#include "types.hh"

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

void print_header(std::ostream& aOutput, std::string aHeaderName, int aWidth = 80);

void print_sep(std::ostream& aOutput, int aTot = 80);

bool hasSuffix(const std::string aString, const std::string aSuffix);

string_vt tokenize(const char* aString, const char aCharToSplit);

void removeSuffix(std::string& aString, const char aChar = '.');





#endif