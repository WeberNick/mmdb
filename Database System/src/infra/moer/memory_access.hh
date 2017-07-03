#ifndef MEMORY_ACCESS_HH
#define MEMORY_ACCESS_HH

#include "measure.hh"
#include "infra/webe/types.hh"
#include "infra/webe/print_helper.hh"
#include "random_permutation.hh"

#include <iostream>
#include <vector>

class MemoryAccess
{
	public:
		MemoryAccess(const uint vector_length);

	public:
		void fRunTest(std::ostream& output);

	private:
		void fPrintResult(std::ostream& output);

	private:
		const int _n;
		double _x1;
		double _x2;
		int _r1;
		int _r2;

  	
};

#endif