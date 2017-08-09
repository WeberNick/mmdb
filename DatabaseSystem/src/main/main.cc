/**
 *	@file 	main.cc
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Main method of the application
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This part of the application starts all the procedures. First of all a
 *	command line arguement parser is constructed and checks for certain
 *	commands like '--help' which prints all the possible command line arguements
 *	and then terminates the application. Secondly, if there are no problems 
 *	with the command line arguements (e.g., wrong or contradicting arguements),
 *	a system test is conducted if the application is running on a linux system.
 *	After the system test was conducted, several command line arguements are
 *	used to initialize the system (like the memory page size to use, and so on).
 *	In the end, several conditions check if certain flags were set by the user
 *	which indicate which tests are conducted.
 */

#include "infra/webe/types.hh"
#include "infra/webe/args.hh"
#include "memory/memory_manager.hh"
#include "modes.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include "queries/test_query.hh"
#include "app/tpc_h/tpc_h.hh"
#include "app/big_int_relation/big_int_relation.hh"


#ifdef __linux__
#include "infra/moer/system.hh"
#include "infra/moer/memory_access.hh"
#endif

#include <string>
#include <fstream>
#include <iostream>

int main(const int argc, const char* argv[])
{
	Args lArgs;
	argdesc_vt lArgDesc;
	construct_arg_desc(lArgDesc);

	if(!parse_args<Args>(1, argc, argv, lArgDesc, lArgs)) 
	{
		std::cerr << "error while parsing arguments." << std::endl;
		return -1;
	}

	if(lArgs.help()) 
	{
		print_usage(std::cout, argv[0], lArgDesc);
		return 0;
	}

	std::string storageModel = ((lArgs.nsm() != lArgs.pax()) ? true : false) ? (lArgs.nsm() ? "NSM" : "PAX") : "NO_STORAGE_MODEL_CHOSEN";

	if(lArgs.nsm() == lArgs.pax())
	{
		std::cerr << "ERROR: " << storageModel << std::endl;
		return -1;
	}

	#ifdef __linux__
	uint lHwThreadNo = lArgs.core();
	cpu_set_t lMask;
	CPU_ZERO(&lMask);
	CPU_SET(lHwThreadNo, &lMask);
	int lRc = sched_setaffinity(0, sizeof(cpu_set_t), &lMask);
	if(0 != lRc) 
	{
		std::cerr << "binding thread to " << lHwThreadNo << " failed: return code: " << lRc << std::endl;
	} 
	else 
	{
		std::cerr << "binding thread to " << lHwThreadNo << " succeeded." << std::endl;
	}

	/***************************************************************************************************
	** If the 'lArgs.measure()'-flag was set, the following prints an header in some output stream *****
	***************************************************************************************************/

	if(lArgs.measure())
	{
		GM::System lSystem(lHwThreadNo);
		std::string lSetting = lSystem.hostname() + "_" + std::to_string(lArgs.pageSize());
		DateJd jd;
		jd.set_to_current_date();
		std::string lHeader = storageModel + " DBS TEST (" + std::to_string(jd.day()) + "/" + std::to_string(jd.month()) + "/" + std::to_string(jd.year()) + ")";
		std::ostream* os = &std::cout;
		std::ofstream out;
		if(lArgs.print())
		{	
			std::string lPath = (lArgs.path() + storageModel + "/").c_str() + lSetting + ".txt";
			out.open(lPath.c_str(), std::ios::out | std::ios::app);
			os = &out;
		}

		if(out.is_open() == lArgs.print())
		{
			print_header(*os, lHeader);
			*os << std::setw(15) << "Page Size: " << lArgs.pageSize() << "    " << "Alignment: " << lArgs.alignment() << "    " << "#Runs: " << lArgs.runs() << "    " << "VectorizedSize: " << lArgs.vectorized() << std::endl;
			print_header(*os, "System");
			lSystem.print(*os);
			print_header(*os, "RANDOM vs. SEQUENTIAL MEMORY ACCESS");
			MemoryAccess memoryAccess(10 * 1000 * 1000);
			memoryAccess.fRunTest(*os);
			if(lArgs.print())
			{	
				out.close();
			}
		}
	}
	#endif


	MemoryManager::createInstance(lArgs.alignment(), lArgs.chunkSize(), lArgs.pageSize());
	PageInterpreterSP::setPageSize(lArgs.pageSize());
	PageInterpreterPAX::setPageSize(lArgs.pageSize());
	print_infra_t lPrintInfra = {(lArgs.path() + storageModel + "/").c_str(), lArgs.print(), lArgs.pageSize()};
	query_infra_t lQueryInfra = {lArgs.runs(), lArgs.measure(), lArgs.vectorized(), lPrintInfra};

	/***************************************************************************************************
	** If the 'NSM'- or 'PAX'-flag was set, the DBS loads the respective storage model mode ************
	***************************************************************************************************/


	if(lArgs.nsm())
	{
		if(lArgs.tpch())
		{
			TPC_H<NSM_Relation> tpc_h(true);
			if(lArgs.all())
			{
				tpc_h.init(lArgs.sf(), lArgs.delimiter(), lArgs.seperator(), lArgs.bufferSize(), lQueryInfra);
				// row_test_query(tpc_h.getTPC_H_Relations()[1], lQueryInfra);
				// row_test_tpch_projection(tpc_h.getTPC_H_Relations()[1], 6000000, MODE_TPCH_NO_ATTR, lQueryInfra);
				row_test_tpch_projection_optimized_switch(tpc_h.getTPC_H_Relations()[1], 6000000, MODE_TPCH_NO_ATTR, lQueryInfra);
			}
			else
			{
				if(lArgs.bl())
				{
					tpc_h.init(lArgs.sf(), lArgs.delimiter(), lArgs.seperator(), lArgs.bufferSize(), lQueryInfra);
				}
			}
		}
		else if(lArgs.intRelation())
		{
			BIG_INT_RELATION<NSM_Relation> b_i_r(true);
			b_i_r.init(MODE_BIR_NO_ATTR, MODE_BIR_NO_TUPLE, lArgs.bufferSize());
			// row_test_projection(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			// row_test_projection_optimized_switch(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			// row_test_projection_mat(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			row_test_projection_mat_optimized_switch(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
		}
		else
		{
			std::cerr << "ERROR: NO DATASET WAS CHOSEN" << std::endl;
			return -1;
		}
	}
	else if(lArgs.pax())
	{
		if(lArgs.tpch())
		{
			TPC_H<PAX_Relation> tpc_h(false);
			if(lArgs.all())
			{
				tpc_h.init(lArgs.sf(), lArgs.delimiter(), lArgs.seperator(), lArgs.bufferSize(), lQueryInfra);
				// col_test_query(tpc_h.getTPC_H_Relations()[1], lQueryInfra);
				col_test_tpch_projection(tpc_h.getTPC_H_Relations()[1], 6000000, MODE_TPCH_NO_ATTR, lQueryInfra);
				// col_test_tpch_projection_optimized_switch(tpc_h.getTPC_H_Relations()[1], 6000000, MODE_TPCH_NO_ATTR, lQueryInfra);
			}
			else
			{
				if(lArgs.bl())
				{
					tpc_h.init(lArgs.sf(), lArgs.delimiter(), lArgs.seperator(), lArgs.bufferSize(), lQueryInfra);
				}
			}
		}
		else if(lArgs.intRelation())
		{
			BIG_INT_RELATION<PAX_Relation> b_i_r(false);
			b_i_r.init(MODE_BIR_NO_ATTR, MODE_BIR_NO_TUPLE, lArgs.bufferSize());
			// col_test_projection(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			// col_test_projection_optimized_switch(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			// col_test_projection_mat(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
			col_test_projection_mat_optimized_switch(b_i_r.getRelation(), MODE_BIR_NO_TUPLE, MODE_BIR_NO_ATTR, lQueryInfra);
		}
		else
		{
			std::cerr << "ERROR: NO DATASET WAS CHOSEN" << std::endl;
			return -1;
		}
	}

	/***************************************************************************************************
	** The DBS finished its processing, all allocated memory is freed and the application terminates ***
	***************************************************************************************************/

	MemoryManager::destroyInstance();
	return 0;
}



