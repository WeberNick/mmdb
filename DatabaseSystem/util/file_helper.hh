#ifndef INFRA_WEBE_FILE_HELPER_HH
#define INFRA_WEBE_FILE_HELPER_HH

#include "types.hh"
#include "string_helper.hh"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void listFilesInDirectory(const string_vt& txtFilesInNSM, const string_vt& txtFilesInPAX);

const string_vt txtFilesInDirectory(const std::string aDirectory);

void filesToPlot(const string_vt& aFileNamesVecNSM, const string_vt& aFileNamesVecPAX, string_vt& aFilesToPlotNSM, string_vt& aFilesToPlotPAX);





#endif