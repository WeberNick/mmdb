#ifndef BULK_INSERT_SP_HH
#define BULK_INSERT_SP_HH

#include <vector>

#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/bulk_loader.hh"
#include "infra/moer/datejd.hh"
#include "memory/segment.hh"
#include "relation/nsm_relation.hh"
#include "page_interpreter_sp.hh"

class BulkInsertSP
{
	public:
		/*starts the bulk insert process which writes the tuples from the bulk loaders buffer into memory pages*/
		void bulk_insert(const std::vector<unval_t*>& aBuffer, const uint aTuplePerChunk, const uint aTupleTotal, NSM_Relation* aRelation);
		/*starts the bulk insert process which writes the tuples from the bulk loaders buffer into memory pages*/
		void bulk_insert(const BulkLoader& aBulkLoader, NSM_Relation* aRelation);

	public:
		/*constructs a bulk inserter object for slotted pages*/
		BulkInsertSP();

	private:
		/*writes the tuple at 'aBufferPointer' location into the slotted page at address 'aRecordPointer' + some offset*/
		void insertTuple(NSM_Relation* aRelation, byte* aRecordPointer, unval_t*& aBufferPointer);
};

#endif