#include "plot.hh"

int main(const int argc, const char* argv[])
{
	if(argc < 2)
	{
		startPlotProcess();
	}
	else
	{
		startPlotProcess(argv[1]);
	}
	
	return 0;
}