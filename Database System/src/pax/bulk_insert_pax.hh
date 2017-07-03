#ifndef BULK_INSERT_PAX_HH
#define BULK_INSERT_PAX_HH

#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/bulk_loader.hh"
#include "infra/moer/datejd.hh"
#include "memory/segment.hh"
#include "relation/pax_relation.hh"
#include "page_interpreter_pax.hh"

#include <vector>

#include <sys/mman.h>
#include <errno.h>

class BulkInsertPAX
{
	public:
		void bulk_insert(const std::vector<unval_t*>& aBuffer, const uint aTuplePerChunk, const uint aTupleTotal, PAX_Relation* aRelation);
		void bulk_insert(const BulkLoader& aBulkLoader, PAX_Relation* aRelation);

	public:
		/*constructs a bulk inserter object for pax pages*/
		BulkInsertPAX();

	private:
		void insertTuple(PAX_Relation* aRelation, unval_t*& aBufferPointer, PageInterpreterPAX& aPageInterpreter, int aIndex);
};

#endif