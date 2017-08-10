/**
 *	@file 	print_helper.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Provides procedures to print test results in files
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	TODO
 *
 */
#ifndef INFRA_WEBE_PRINT_HEADER_HH
#define INFRA_WEBE_PRINT_HEADER_HH

#include "types.hh"
#include "infra/moer/system.hh"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>

struct print_infra_t
{
	const char*		_path;
	const bool		_print;
	const size_t	_pageSize;

	const char*		path() const { return _path; }
	const bool		print() const { return _print; }
	const size_t	pageSize() const { return _pageSize; }
};

void print_header(std::ostream& aOutput, std::string aHeaderName, int aWidth = 80);

void print_sep(std::ostream& aOutput, int aTot = 80);

double round(double d);

std::string fFormatNumber(int number);

std::string getEnumAsString(int aValue, bool aFlag);

void print_bulk_load_insert_result(const string_vt& aRelationNames, const double_vt& aMeasurementData, const bool aFlag, const print_infra_t& aPrintInfra, const std::string aTestType = "Unknown");

void print_scan_result(const size_t aAttrNo, const double_vt& aMeasurementData, const print_infra_t& aPrintInfra, const std::string aTestType = "Unknown");

void print_projection_result(const size_t aAttrNo, const double_vt& aMeasurementData, const print_infra_t& aPrintInfra, const std::string aTestType = "Unknown");

#endif
