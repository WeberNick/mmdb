/**
 *	@file 	bulk_insert_sp.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	A bulk insert class to read data from an intermediate buffer and write it into the database
 *	@bugs 	Currently no bugs known
 *	@todos	Better exception handling
 *
 *	@section DESCRIPTION
 *	This class implements the bulk insert functionality, reading data from an intermediate buffer
 *	and write it into the in memory database. This class is dependent on the storage model in use
 *	and is specific for the slotted pages (NSM) page format.
 */
#ifndef BULK_INSERT_SP_HH
#define BULK_INSERT_SP_HH

#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/intermediate_buffer.hh"
#include "infra/webe/bulk_loader.hh"
#include "infra/moer/datejd.hh"
#include "relation/nsm_relation.hh"
#include "memory/segment.hh"
#include "page_interpreter_sp.hh"

#include <vector>

/**
 *	This class gets either the intermediate buffer directly or an bulk loader object to
 *	ask the bulk loader for its intermediate buffer. All the stored data from the intermediate
 *	buffer is written into the in memory database in a slotted page format.
 */
class BulkInsertSP
{
	public:
		/*constructs a bulk insert object for slotted pages */
		BulkInsertSP();
		/* destructs a bulk insert object */
		~BulkInsertSP();

	public:
		/**
		 *	@brief	starts the bulk insert process, reading the tuples from the bulk loaders 
		 *			intermediate buffer and writing them into slotted memory pages
		 *
		 *	@param	aBuffer  reference to the intermediate buffer to read the tuples from
		 *	@param	aRelation  reference to the nsm relation, needed for the physical layout 
		 */
		void bulk_insert(const IntermediateBuffer& aBuffer, NSM_Relation* aRelation);

		/**
		 *	@brief	starts the bulk insert process, reading the tuples from the bulk loaders 
		 *			intermediate buffer and writing them into slotted memory pages
		 *
		 *	@param	aBuffer  reference to the bulk loader object
		 *	@param	aRelation  reference to the nsm relation, needed for the physical layout 
		 */
		void bulk_insert(const BulkLoader& aBulkLoader, NSM_Relation* aRelation);

	private:
		/**
		 *	@brief	writes the tuple from the buffer into the slotted page
		 *
		 *	@param	aBufferPointer  reference to the buffer to read the tuple from
		 *	@param	aRecordPointer  a pointer to an address to store the tuple at
		 *	@param	aRelation  reference to the nsm relation, needed for the physical layout 
		 */
		void insertTuple(unval_pt& aBufferPointer, byte* aRecordPointer, NSM_Relation* aRelation);
};

#endif