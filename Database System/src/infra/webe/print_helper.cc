#include "print_helper.hh"


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

double round(double d)
{
  return floor(d + 0.5);
}

std::string fFormatNumber(int number)
{
	std::string numberWithFormat = std::to_string(number);
	int insertPosition = numberWithFormat.length() - 3;
	while (insertPosition > 0)
	{
		numberWithFormat.insert(insertPosition, ",");
		insertPosition -= 3;
	}

	return numberWithFormat;
}

std::string getEnumAsString(int aValue, bool aFlag)
{
	std::string enumName;
	if(aFlag)
	{
		switch(aValue)
		{
			case 0: 
				enumName =  "kNoType";
				break;
			case 1: 
				enumName =  "kCHAR";
				break;
			case 2: 
				enumName =  "kUINT32";
				break;
			case 3: 
				enumName =  "kUINT64";
				break;
			case 4: 
				enumName =  "kINT32";
				break;
			case 5: 
				enumName =  "kFLOAT32";
				break;
			case 6: 
				enumName =  "kFLOAT64";
				break;
			case 7: 
				enumName =  "kCHAR_STRING";
				break;
			case 8: 
				enumName =  "kDATEJD";
				break;
			case 9: 
				enumName =  "kSTR_SDICT";
				break;
			case 10: 
				enumName =  "kEndType";
				break;
			default:
				enumName =  "SwitchError";
				break;
		}
	}
	else
	{
		switch(aValue)
		{
			case -1: 
				enumName =  "kNoContainer";
				break;
			case 0: 
				enumName =  "kSimpleCharContainer";
				break;
			case 1: 
				enumName =  "kSimpleDictionary";
				break;
			case 2: 
				enumName =  "kSimpleOrderedDictionary";
				break;
			default:
				enumName =  "SwitchError";
				break;
		}
	}

	return enumName;
}



void print_bulk_load_insert_result(const string_vt& aRelationNames, const double_vt& aMeasurementData, const bool aFlag)
{
	std::ofstream raw;
	std::string lTestName = aFlag ? "load" : "insert";
	GM::System lSystem;
	std::string lSetting = lSystem.hostname() + "_" + std::to_string(PAGE_SIZE_GLOBAL);
	std::string lPath = std::string(PATH_GLOBAL) + "raw/" + lTestName + "_" + lSetting + ".txt";
	raw.open(lPath.c_str(), std::ios::out | std::ios::trunc);
	if(raw.is_open())
	{
		for(size_t i = 0; i < aRelationNames.size(); ++i)
		{
			raw << aRelationNames[i] << ' ' << std::setprecision(3) << std::fixed << aMeasurementData[i] << std::endl;
		}
		raw.close();
	}
	
	std::ostream* os = &std::cout;
	std::ofstream out;
	if(PRINT_GLOBAL)
	{	
		lPath = PATH_GLOBAL + lSetting + ".txt";
		out.open(lPath.c_str(), std::ios::out | std::ios::app);
		os = &out;
	}

	if(out.is_open() == PRINT_GLOBAL)
	{
		print_header(*os, "bulk-" + lTestName);
		for(size_t i = 0; i < aRelationNames.size(); ++i)
		{
			*os << std::setw(10) << aRelationNames[i] << ": " << std::setprecision(3) << std::fixed << aMeasurementData[i] << "s" << std::endl;
		}
		if(PRINT_GLOBAL)
		{	
			out.close();
		}
	}
}

void print_scan_result(const size_t aAttrNo, const double_vt& aMeasurementData)
{
	std::ofstream raw;
	std::string lTestName = "scan";
	GM::System lSystem;
	std::string lSetting = lSystem.hostname() + "_" + std::to_string(PAGE_SIZE_GLOBAL);
	std::string lPath = std::string(PATH_GLOBAL) + "raw/" + lTestName + "_" + lSetting + ".txt";
	raw.open(lPath.c_str(), std::ios::out | std::ios::trunc);
	if(raw.is_open())
	{
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			raw << (i+1) << ' ' << std::setprecision(3) << std::fixed << aMeasurementData[i] << std::endl;
		}
		raw.close();
	}
	
	std::ostream* os = &std::cout;
	std::ofstream out;
	if(PRINT_GLOBAL)
	{	
		lPath = PATH_GLOBAL + lSetting + ".txt";
		out.open(lPath.c_str(), std::ios::out | std::ios::app);
		os = &out;
	}

	if(out.is_open() == PRINT_GLOBAL)
	{
		print_header(*os, lTestName);
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			*os << std::setw(10) << (i+1) << " attributes" << ": " << std::setprecision(3) << std::fixed << aMeasurementData[i] << "ms" << std::endl;
		}
		if(PRINT_GLOBAL)
		{	
			out.close();
		}
	}
}

void print_int_projection_result(const size_t aAttrNo, const double_vt& aMeasurementData)
{
	std::ofstream raw;
	std::string lTestName = "int-projection";
	GM::System lSystem;
	std::string lSetting = lSystem.hostname() + "_" + std::to_string(PAGE_SIZE_GLOBAL);
	std::string lPath = std::string(PATH_GLOBAL) + "raw/" + lTestName + "_" + lSetting + ".txt";
	raw.open(lPath.c_str(), std::ios::out | std::ios::trunc);
	if(raw.is_open())
	{
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			raw << (i+1) << ' ' << std::setprecision(3) << std::fixed << aMeasurementData[i] << std::endl;
		}
		raw.close();
	}
	
	std::ostream* os = &std::cout;
	std::ofstream out;
	if(PRINT_GLOBAL)
	{	
		lPath = PATH_GLOBAL + lSetting + ".txt";
		out.open(lPath.c_str(), std::ios::out | std::ios::app);
		os = &out;
	}

	if(out.is_open() == PRINT_GLOBAL)
	{
		print_header(*os, lTestName);
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			*os << std::setw(10) << (i+1) << " attributes" << ": " << std::setprecision(3) << std::fixed << aMeasurementData[i] << "ms" << std::endl;
		}
		if(PRINT_GLOBAL)
		{	
			out.close();
		}
	}
}

void print_tq1_result(const char* aPath, int aSelectivity, double aTime)
{
	aTime *= 1000; 
	std::string fileName(aPath + std::to_string(PAGE_SIZE_GLOBAL) + ".txt");
	std::ofstream os;
	os.open (fileName.c_str(), std::ios::out | std::ios::app);
	os << aSelectivity << '\t' << std::setprecision(3) << std::fixed << aTime << std::endl;
	os.close();
}

void print_tu1_result(const char* aPath, int aAttrNo, double aTime)
{
	aTime *= 1000;
	std::string fileName(aPath + std::to_string(PAGE_SIZE_GLOBAL) + ".txt");
	std::ofstream os;
	os.open (fileName.c_str(), std::ios::out | std::ios::app);
	os << aAttrNo << '\t' << std::setprecision(3) << std::fixed << aTime << std::endl;
	os.close();
}