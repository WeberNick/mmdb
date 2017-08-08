#ifndef TPC_H_HH
#define TPC_H_HH

#include "tpc_h_data.hh"
#include "infra/moer/measure.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/print_helper.hh"
#include "infra/webe/types.hh"
#include "infra/webe/bulk_loader.hh"
#include "nsm/bulk_insert_sp.hh"
#include "pax/bulk_insert_pax.hh"
#include "memory/memory_manager.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include "queries/test_query.hh"

#include <string>
#include <vector>
#include <fstream>



template<typename T_Relation>
class TPC_H
{
	public:
		typedef std::vector<T_Relation> relation_vt;

	public:
		//bulk load and bulk insert
		void init(const std::string aScaleFactor, const char aDelimiter, const char aSeperator, const size_t aBufferSize, const query_infra_t& aQueryInfra);
		relation_vt& getTPC_H_Relations();

	public:
		TPC_H(const bool aFlag);
		// ~TPC_H();

		
	private:
		bool _flag;
		relation_vt _relation_vector;

};

#endif