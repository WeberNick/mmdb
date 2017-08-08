#ifndef BIG_INT_RELATION_HH
#define BIG_INT_RELATION_HH

#include "big_int_relation_data.hh"
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

#include <string>
#include <vector>
#include <fstream>

template<typename T_Relation>
class BIG_INT_RELATION
{
	public:
		//bulk load and bulk insert
		void init(const size_t aAttrNo, const size_t aSize, const size_t aBufferSize);
		T_Relation& getRelation();

	public:
		BIG_INT_RELATION(const bool aFlag);
		// ~BIG_INT_RELATION();

		
	private:
		bool _flag;
		T_Relation _relation;

};

#endif