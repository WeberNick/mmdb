#include "plot.hh"

std::string G_FORMAT;

void getIndeces(uint_vt& nsmPlotIndeces, const string_vvt& nsmFileNameTokens, uint_vt& paxPlotIndeces, const string_vvt& paxFileNameTokens, const std::string aTestType)
{
	bool lFlag = true, iFlag = false, jFlag = false;
	uint i = 0, j = 0;
	while(lFlag)
	{
		if(i < nsmFileNameTokens.size())
		{
			if(nsmFileNameTokens[i][0] == aTestType)
			{
				nsmPlotIndeces.push_back(i);
			}
			++i;
			if(i >= nsmFileNameTokens.size())
			{
				iFlag = true;
			}
		}

		if(j < paxFileNameTokens.size())
		{
			if(paxFileNameTokens[j][0] == aTestType)
			{
				paxPlotIndeces.push_back(j);
			}
			++j;
			if(j >= paxFileNameTokens.size())
			{
				jFlag = true;
			}
		}

		if(iFlag && jFlag)
		{
			lFlag = false;
		}
	}
}

void plotBulkLoadInsert(const bool aIsLoad, const std::string nsmPath, const string_vt& nsmDataFilesToPlot, const string_vvt& nsmFileNameTokens, const uint_vt& nsmPlotIndeces, const std::string paxPath, const string_vt& paxDataFilesToPlot, const string_vvt& paxFileNameTokens, const uint_vt& paxPlotIndeces)
{
	const size_t numberOfFilesToPlot = nsmPlotIndeces.size() + paxPlotIndeces.size();

	const std::string lTestName = (aIsLoad) ? "Bulk Loading" : "Bulk Insert";

	std::string lFileName = (aIsLoad) ? "bulk_load" : "bulk_insert";

	if(numberOfFilesToPlot != 0)
	{
		Gnuplot gp;
		gp << ("set terminal " + G_FORMAT + "\n");
		gp << "set output '../results/" << lFileName << ("." + G_FORMAT + "'\n");
		gp << "set grid\n";
		gp << "set autoscale\n";
		gp << "set ylabel 'Average time in sec' font ', 8'\n";
		gp << "set xtics font ',8'\n";
		gp << "set title 'Average Time of " << lTestName << " each Relation' font ',12'\n";
		gp << "set boxwidth 0.7 relative\n";
		gp << "set style data histograms\n";
		gp << "set style histogram cluster\n";
		gp << "set style fill solid 1.0 border lt -1\n";
		gp << "set auto x\n";
		gp << "set auto y\n";
		gp << "set key top left font ', 6'\n";

		std::string command = "plot ";
		std::string title;
		uint count = 0;
		uint index;
		for(size_t i = 0; i < nsmPlotIndeces.size(); ++i)
		{
			index = nsmPlotIndeces[i];
			title = "NSM-" + nsmFileNameTokens[index][0] + "(" + nsmFileNameTokens[index][1] + "/" + nsmFileNameTokens[index][2] + "/" + nsmFileNameTokens[index][3] + ")";
			command += "'" + nsmPath + nsmDataFilesToPlot[index] + "' using 2:xticlabels(1) title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		for(size_t i = 0; i < paxPlotIndeces.size(); ++i)
		{
			index = paxPlotIndeces[i];
			title = "PAX-" + paxFileNameTokens[index][0] + "(" + paxFileNameTokens[index][1] + "/" + paxFileNameTokens[index][2] + "/" + paxFileNameTokens[index][3] + ")";
			command += "'" + paxPath + paxDataFilesToPlot[index] + "' using 2:xticlabels(1) title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		command += "\n";
		gp << command;
	}
}

void plotProjection(const std::string nsmPath, const string_vt& nsmDataFilesToPlot, const string_vvt& nsmFileNameTokens, const uint_vt& nsmPlotIndeces, const std::string paxPath, const string_vt& paxDataFilesToPlot, const string_vvt& paxFileNameTokens, const uint_vt& paxPlotIndeces)
{
	const size_t numberOfFilesToPlot = nsmPlotIndeces.size() + paxPlotIndeces.size();
	if(numberOfFilesToPlot != 0)
	{
		Gnuplot gp;
		gp << ("set terminal " + G_FORMAT + "\n");
		gp << ("set output '../results/projection." + G_FORMAT + "'\n");
		gp << "set grid\n";
		gp << "set autoscale\n";
		gp << "set ylabel 'Average projection time per tuple in ns' font ', 8'\n";
		gp << "set xlabel 'Number of attributes projected' font ', 8'\n";
		gp << "set title 'Projection Time for a different Number of Attributes' font ',12'\n";
		gp << "set style data linespoints\n";
		gp << "set pointsize 0.5\n";
		gp << "set auto x\n";
		// gp << "set xrange [0:20]\n";
		gp << "set yrange [0:800]\n";
		gp << "set key top left font ', 6'\n";

		std::string command = "plot ";
		std::string title;
		uint count = 0;
		uint index;
		for(size_t i = 0; i < nsmPlotIndeces.size(); ++i)
		{
			index = nsmPlotIndeces[i];
			title = "NSM-" + nsmFileNameTokens[index][0] + "(" + nsmFileNameTokens[index][1] + "/" + nsmFileNameTokens[index][2] + "/" + nsmFileNameTokens[index][3] + ")";
			command += "'" + nsmPath + nsmDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		for(size_t i = 0; i < paxPlotIndeces.size(); ++i)
		{
			index = paxPlotIndeces[i];
			title = "PAX-" + paxFileNameTokens[index][0] + "(" + paxFileNameTokens[index][1] + "/" + paxFileNameTokens[index][2] + "/" + paxFileNameTokens[index][3] + ")";
			command += "'" + paxPath + paxDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		command += "\n";
		gp << command;
	}
}

void plotScan(const std::string nsmPath, const string_vt& nsmDataFilesToPlot, const string_vvt& nsmFileNameTokens, const uint_vt& nsmPlotIndeces, const std::string paxPath, const string_vt& paxDataFilesToPlot, const string_vvt& paxFileNameTokens, const uint_vt& paxPlotIndeces)
{
	const size_t numberOfFilesToPlot = nsmPlotIndeces.size() + paxPlotIndeces.size();
	if(numberOfFilesToPlot != 0)
	{
		Gnuplot gp;
		gp << ("set terminal " + G_FORMAT + "\n");
		gp << ("set output '../results/scan." + G_FORMAT + "'\n");
		gp << "set grid\n";
		gp << "set autoscale\n";
		gp << "set ylabel 'Average time in ms' font ', 8'\n";
		gp << "set xlabel 'Attributes scanned' font ', 8'\n";
		gp << "set title 'Average Scan Time for a different Number of Attributes' font ',12'\n";
		gp << "set style data linespoints\n";
		gp << "set pointsize 0.5\n";
		gp << "set auto x\n";
		gp << "set yrange [0:200]\n";
		gp << "set key top left font ', 6'\n";

		std::string command = "plot ";
		std::string title;
		uint count = 0;
		uint index;
		for(size_t i = 0; i < nsmPlotIndeces.size(); ++i)
		{
			index = nsmPlotIndeces[i];
			title = "NSM-" + nsmFileNameTokens[index][0] + "(" + nsmFileNameTokens[index][1] + "/" + nsmFileNameTokens[index][2] + "/" + nsmFileNameTokens[index][3] + ")";
			command += "'" + nsmPath + nsmDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		for(size_t i = 0; i < paxPlotIndeces.size(); ++i)
		{
			index = paxPlotIndeces[i];
			title = "PAX-" + paxFileNameTokens[index][0] + "(" + paxFileNameTokens[index][1] + "/" + paxFileNameTokens[index][2] + "/" + paxFileNameTokens[index][3] + ")";
			command += "'" + paxPath + paxDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		command += "\n";
		gp << command;
	}
}

void plotSelection(const std::string nsmPath, const string_vt& nsmDataFilesToPlot, const string_vvt& nsmFileNameTokens, const uint_vt& nsmPlotIndeces, const std::string paxPath, const string_vt& paxDataFilesToPlot, const string_vvt& paxFileNameTokens, const uint_vt& paxPlotIndeces)
{
	const size_t numberOfFilesToPlot = nsmPlotIndeces.size() + paxPlotIndeces.size();
	if(numberOfFilesToPlot != 0)
	{
		Gnuplot gp;
		gp << ("set terminal " + G_FORMAT + "\n");
		gp << ("set output '../results/selection." + G_FORMAT + "'\n");
		gp << "set grid\n";
		gp << "set autoscale\n";
		gp << "set ylabel 'Average Time in ms' font ', 8'\n";
		gp << "set xlabel 'Selectivity in \%' font ', 8'\n";
		gp << "set title 'Average Selection Time for each Selectivity' font ',12'\n";
		gp << "set style data linespoints\n";
		gp << "set pointsize 0.5\n";
		gp << "set auto x\n";
		gp << "set yrange [0:150]\n";
		gp << "set key top left font ', 6'\n";

		std::string command = "plot ";
		std::string title;
		uint count = 0;
		uint index;
		for(size_t i = 0; i < nsmPlotIndeces.size(); ++i)
		{
			index = nsmPlotIndeces[i];
			title = "NSM-" + nsmFileNameTokens[index][0] + "(" + nsmFileNameTokens[index][1] + "/" + nsmFileNameTokens[index][2] + "/" + nsmFileNameTokens[index][3] + ")";
			command += "'" + nsmPath + nsmDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		for(size_t i = 0; i < paxPlotIndeces.size(); ++i)
		{
			index = paxPlotIndeces[i];
			title = "PAX-" + paxFileNameTokens[index][0] + "(" + paxFileNameTokens[index][1] + "/" + paxFileNameTokens[index][2] + "/" + paxFileNameTokens[index][3] + ")";
			command += "'" + paxPath + paxDataFilesToPlot[index] + "' using 1:2 title '" + title + "'";
			++count;
			if(count < numberOfFilesToPlot)
			{
				command += ", ";
			}
		}
		command += "\n";
		gp << command;
	}
}

void startPlotProcess(const char* aPlotG_FORMAT)
{
	G_FORMAT = std::string(aPlotG_FORMAT);
	/***************************************************************************************************
	** Path to the NSM and PAX data files **************************************************************
	***************************************************************************************************/
	const std::string nsmPath = "../data/NSM/raw/";
	const std::string paxPath = "../data/PAX/raw/";
	/***************************************************************************************************
	** Stores all .txt file names in a vector and print a list for the user ****************************
	***************************************************************************************************/
	const string_vt nsmDataFileNames = txtFilesInDirectory(nsmPath);
	const string_vt paxDataFileNames = txtFilesInDirectory(paxPath);
	listFilesInDirectory(nsmDataFileNames, paxDataFileNames);
	/***************************************************************************************************
	** The user can choose files to plot  **************************************************************
	***************************************************************************************************/
	string_vt nsmDataFilesToPlot;
	string_vt paxDataFilesToPlot;
	filesToPlot(nsmDataFileNames, paxDataFileNames, nsmDataFilesToPlot, paxDataFilesToPlot);
	/***************************************************************************************************
	** Tokenize the file names (tokens are needed later on) ********************************************
	***************************************************************************************************/
	string_vvt nsmFileNameTokens;
	for(size_t i = 0; i < nsmDataFilesToPlot.size(); ++i)
	{
		nsmFileNameTokens.push_back(tokenize(nsmDataFilesToPlot[i].c_str(), '_'));
		removeSuffix(nsmFileNameTokens[i][nsmFileNameTokens[i].size()-1]);
	}
	string_vvt paxFileNameTokens;
	for(size_t i = 0; i < paxDataFilesToPlot.size(); ++i)
	{
		paxFileNameTokens.push_back(tokenize(paxDataFilesToPlot[i].c_str(), '_'));
		removeSuffix(paxFileNameTokens[i][paxFileNameTokens[i].size()-1]);
	}
	/***************************************************************************************************
	** Stores all the indeces from '[...]DataFilesToPlot' which belong to the same test together  ******
	** [0][...] are bulk load test indeces, [1][...] are [PLACEHOLDER]                            ******
	***************************************************************************************************/
	uint_vvt nsmPlotIndeces(5);
	uint_vvt paxPlotIndeces(5);
	getIndeces(nsmPlotIndeces[0], nsmFileNameTokens, paxPlotIndeces[0], paxFileNameTokens, "load");
	plotBulkLoadInsert(true, nsmPath, nsmDataFilesToPlot, nsmFileNameTokens, nsmPlotIndeces[0], paxPath, paxDataFilesToPlot, paxFileNameTokens, paxPlotIndeces[0]);
	getIndeces(nsmPlotIndeces[1], nsmFileNameTokens, paxPlotIndeces[1], paxFileNameTokens, "insert");
	plotBulkLoadInsert(false, nsmPath, nsmDataFilesToPlot, nsmFileNameTokens, nsmPlotIndeces[1], paxPath, paxDataFilesToPlot, paxFileNameTokens, paxPlotIndeces[1]);
	getIndeces(nsmPlotIndeces[2], nsmFileNameTokens, paxPlotIndeces[2], paxFileNameTokens, "projection");
	plotProjection(nsmPath, nsmDataFilesToPlot, nsmFileNameTokens, nsmPlotIndeces[2], paxPath, paxDataFilesToPlot, paxFileNameTokens, paxPlotIndeces[2]);
	getIndeces(nsmPlotIndeces[3], nsmFileNameTokens, paxPlotIndeces[3], paxFileNameTokens, "scan");
	plotScan(nsmPath, nsmDataFilesToPlot, nsmFileNameTokens, nsmPlotIndeces[3], paxPath, paxDataFilesToPlot, paxFileNameTokens, paxPlotIndeces[3]);
	getIndeces(nsmPlotIndeces[4], nsmFileNameTokens, paxPlotIndeces[4], paxFileNameTokens, "selection");
	plotSelection(nsmPath, nsmDataFilesToPlot, nsmFileNameTokens, nsmPlotIndeces[4], paxPath, paxDataFilesToPlot, paxFileNameTokens, paxPlotIndeces[4]);
}