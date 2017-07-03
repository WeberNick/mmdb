#include "memory_access.hh"

MemoryAccess::MemoryAccess(const uint vector_length) : _n(vector_length * 10), _x1(0), _x2(0), _r1(0), _r2(0){}

void MemoryAccess::fPrintResult(std::ostream& output)
{
	output << "sequential/random read: " << std::endl
		<< "   sequential read: " << ((_x1 / _n) * NS) << " [ns]" << std::endl
		<< "       random read: " << ((_x2 / _n) * NS) << " [ns]" << std::endl
		<< "            factor: " << (_x2 / _x1) << std::endl
		<< "             r1/r2: " << fFormatNumber(_r1) << " =?= " << fFormatNumber(_r2) <<std::endl;
}

void MemoryAccess::fRunTest(std::ostream& output)
{
	int* lId   = new int[_n];
	int* lPerm = new int[_n];
	int* lData = new int[_n];

	for(int i = 0; i < _n; ++i) 
	{
		lId[i] = i;
		lPerm[i] = i;
		lData[i] = i;
	}

	std::mt19937 lRng(17);

	random_permutation(lRng, lPerm,  _n);

    {
		Measure lMeasure;
		lMeasure.start();
		for(int i = 0; i < _n; ++i) 
		{
			_r1 += lData[lId[i]];
		}
		lMeasure.stop();
		_x1 = lMeasure.mTotalTime();
	}

	{
		Measure lMeasure;
		lMeasure.start();
		for(int i = 0; i < _n; ++i) 
		{
			_r2 += lData[lPerm[i]];
		}
		lMeasure.stop();
		_x2 = lMeasure.mTotalTime();
    }

	delete[] lData;
	delete[] lPerm;
	delete[] lId;

	fPrintResult(output);
}