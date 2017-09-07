#include "file_helper.hh"

void listFilesInDirectory(const string_vt& txtFilesInNSM, const string_vt& txtFilesInPAX)
{
	print_header(std::cout, "List of plottable files in the 'NSM' directory");
	size_t k = 1;
	if(txtFilesInNSM.size() == 0)
	{
		std::cout << "=== NO FILES FOUND ===" << std::endl;
	}
	for(size_t i = 0; i < txtFilesInNSM.size(); i++) 
	{
		std::cout << "(" << k << ") " << txtFilesInNSM[i] << ' ' << ' ';
		++k;
	}
	std::cout << std::endl << std::endl;

	print_header(std::cout, "List of plottable files in the 'PAX' directory");
	if(txtFilesInPAX.size() == 0)
	{
		std::cout << "=== NO FILES FOUND ===" << std::endl;
	}
	for(size_t i = 0; i < txtFilesInPAX.size(); i++) 
	{
		std::cout << "(" << k << ") " << txtFilesInPAX[i] << ' ' << ' ';
		++k;
	}
	std::cout << std::endl << std::endl;
}

const string_vt txtFilesInDirectory(const std::string aDirectory)
{
	string_vt lFiles;
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(aDirectory.c_str())) == NULL) 
	{
		std::cout << "Error(" << errno << ") opening " << aDirectory << std::endl;
	}
	else
	{
		while ((dirp = readdir(dp)) != NULL) 
		{
			std::string currFile = std::string(dirp->d_name);
			if(currFile[0] != '.' && hasSuffix(currFile, ".txt"))
			{
				lFiles.push_back(currFile);
			}
		}
		closedir(dp);
	}
	return lFiles;
}

void filesToPlot(const string_vt& aFileNamesVecNSM, const string_vt& aFileNamesVecPAX, string_vt& aFilesToPlotNSM, string_vt& aFilesToPlotPAX)
{
	std::string input = "";
	size_t nsmVecSize = aFileNamesVecNSM.size();
	size_t paxVecSize = aFileNamesVecPAX.size();
	uint fileNumber = 0;
	bool lFlag = true;
	while(lFlag)
	{
		print_header(std::cout, "Please enter the filenumber to plot and '0' to continue");
		getline(std::cin, input);
		std::stringstream stream(input);
		if(stream >> fileNumber && (fileNumber >= 0 && fileNumber <= (nsmVecSize + paxVecSize)))
		{
			if(fileNumber == 0)
			{
				lFlag = false;
			}
			else if(std::find(aFilesToPlotNSM.begin(), aFilesToPlotNSM.end(), aFileNamesVecNSM[fileNumber-1]) != aFilesToPlotNSM.end())
			{
				std::cout << "File '(" << fileNumber << ") " << aFileNamesVecNSM[fileNumber-1] << "' already selected for comparison, please select different files" << std::endl;
			}
			else if(std::find(aFilesToPlotPAX.begin(), aFilesToPlotPAX.end(), aFileNamesVecPAX[fileNumber-nsmVecSize-1]) != aFilesToPlotPAX.end())
			{
				std::cout << "File '(" << fileNumber << ") " << aFileNamesVecPAX[fileNumber-nsmVecSize-1] << "' already selected for comparison, please select different files" << std::endl;
			}
			else
			{
				if(fileNumber <= nsmVecSize)
				{
					aFilesToPlotNSM.push_back(aFileNamesVecNSM[fileNumber-1]);
				}
				else if(fileNumber > nsmVecSize)
				{
					aFilesToPlotPAX.push_back(aFileNamesVecPAX[fileNumber-nsmVecSize-1]);
				}
				else
				{
					std::cerr << "ERROR OCCURED (Insert files into plot vector)" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "File number '" << input << "' not found, make sure the file is in the following list: " << std::endl;
			size_t k = 1;
			std::cout << "=== NSM DATA FILES ===" << std::endl;
			for(size_t i = 0; i < aFileNamesVecNSM.size(); i++) 
			{
				std::cout << "(" << (k) << ") " << aFileNamesVecNSM[i] << ' ' << ' ';
				++k;
			}
			std::cout << std::endl;
			std::cout << "=== PAX DATA FILES ===" << std::endl;
			for(size_t i = 0; i < aFileNamesVecPAX.size(); i++) 
			{
				std::cout << "(" << (k) << ") " << aFileNamesVecPAX[i] << ' ' << ' ';
				++k;
			}
			std::cout << std::endl << std::endl;
		}
	}
}