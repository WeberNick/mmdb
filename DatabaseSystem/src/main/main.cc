/**
 *	@file 	main.cc
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Main method of the application
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	
 */

#include "memory/memory_manager.hh"
#include "infra/webe/types.hh"
#include "infra/webe/args.hh"
#include "infra/webe/print_helper.hh"
#include "infra/moer/system.hh"
#include "infra/moer/memory_access.hh"
#include "infra/moer/stime.hh"
#include "infra/moer/datejd.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include "modes.hh"
#include "queries/test_query.hh"

#include "app/tpc_h/tpc_h.hh"
#include "app/big_int_relation/big_int_relation.hh"

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
	#endif

	

	/***************************************************************************************************
	** All global variables are defined with either the respective user input or default values ********
	***************************************************************************************************/

	// std::string tmp 		= lArgs.path() + storageModel + "/";
	// PATH_GLOBAL 			= tmp.c_str();

	/***************************************************************************************************
	** If the 'lArgs.measure()'-flag was set, the following prints an header in some output stream *************
	***************************************************************************************************/

	#ifdef __linux__
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
				// row_test_tpch_projection(tpc_h.getTPC_H_Relations()[1], 6000000, 16, lQueryInfra);
				row_test_tpch_projection_optimized_switch(tpc_h.getTPC_H_Relations()[1], 6000000, 16, lQueryInfra);


				// ints_bulk_load_and_update<NSM_Relation, true>(intAttrNo, intChunkSize, lArgs.measure(), lArgs.runs(), r_test_updateInt_path);
				// bulk_load_insert<initRelationNSM_vt, NSM_Relation, true>(functionsNSM, nsm_rel_vec, lArgs.measure(), 1, r_bulk_load_path);
				// row_test_query1(nsm_rel_vec[1], lArgs.vectorized(), selectivity, lArgs.measure(), lArgs.runs(), r_test_query1_path);
				// row_test_update1(nsm_rel_vec[1], attrUpdates, lArgs.measure(), lArgs.runs(), r_test_update1_path);
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
			const size_t lNumberOfTuples = 1000000;
			const size_t lNumberOfAttributes = 100;
			BIG_INT_RELATION<NSM_Relation> b_i_r(true);
			b_i_r.init(lNumberOfAttributes, lNumberOfTuples, lArgs.bufferSize());
			// row_test_projection(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			// row_test_projection_optimized_switch(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			// row_test_projection_mat(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			row_test_projection_mat_optimized_switch(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
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
				col_test_tpch_projection(tpc_h.getTPC_H_Relations()[1], 6000000, 16, lQueryInfra);
				// col_test_tpch_projection_optimized_switch(tpc_h.getTPC_H_Relations()[1], 6000000, 16, lQueryInfra);


				// ints_bulk_load_and_update<NSM_Relation, true>(intAttrNo, intChunkSize, lArgs.measure(), lArgs.runs(), r_test_updateInt_path);
				// bulk_load_insert<initRelationNSM_vt, NSM_Relation, true>(functionsNSM, nsm_rel_vec, lArgs.measure(), 1, r_bulk_load_path);
				// row_test_query1(nsm_rel_vec[1], lArgs.vectorized(), selectivity, lArgs.measure(), lArgs.runs(), r_test_query1_path);
				// row_test_update1(nsm_rel_vec[1], attrUpdates, lArgs.measure(), lArgs.runs(), r_test_update1_path);
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
			const size_t lNumberOfTuples = 1000000;
			const size_t lNumberOfAttributes = 100;
			BIG_INT_RELATION<PAX_Relation> b_i_r(false);
			b_i_r.init(lNumberOfAttributes, lNumberOfTuples, lArgs.bufferSize());
			// col_test_projection(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			// col_test_projection_optimized_switch(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			// col_test_projection_mat(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
			col_test_projection_mat_optimized_switch(b_i_r.getRelation(), lNumberOfTuples, lNumberOfAttributes, lQueryInfra);
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

	MemoryManager::getInstance()->freeAll();
	return 0;
}



