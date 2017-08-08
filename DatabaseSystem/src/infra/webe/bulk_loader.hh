/**
 *	@file 	bulk_loader.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	A bulk loader class to read a data from a file and write it into a buffer
 *	@bugs 	Currently no bugs known
 *	@todos	Better exception handling
 *
 *	@section DESCRIPTION
 *	This class implements a bulk loader which allows to read data from a file and write it
 *	into a intermediate buffer (@see intermediate_buffer.hh). This buffer then can be 
 * 	passed to the bulk insert (@see bulk_insert_nsm.hh and @see bulk_insert_pax.hh respectively) 
 *	to write the data from the buffer into the (in memory) database.	
 */
#ifndef INFRA_WEBE_BULK_LOADER_HH
#define INFRA_WEBE_BULK_LOADER_HH

#include "types.hh"
#include "exception.hh"
#include "intermediate_buffer.hh"
#include "relation/relation.hh"
#include "infra/moer/linereader.hh"
#include "infra/moer/datejd.hh"
#include "infra/moer/simplecharcontainer.hh"
#include "infra/moer/simpledictionary.hh"

#include <vector>
#include <string>
#include <cstring>
#include <stddef.h>

/**
 *	Implements the bulk loading functionality. First a BulkLoader object has to be constructed
 *	with the respective parameter and then the bulk load functionality is called on this object.
 */
class BulkLoader
{
	public:
		/* constructs a bulk loader object */
		BulkLoader(const char* aFileName, const char aDelimiter, const char aSeperator, const size_t aBufferSize, Relation& aRelation);
		/* constructs a bulk loader object used for a specific artificial relation */
		BulkLoader(const size_t aBufferSize, Relation& aRelation);
		/* destructs a bulk loader object */
		~BulkLoader();

	public:
		inline const IntermediateBuffer& getIntermediateBuffer() const;
		/*start the bulk loading process*/
		void bulk_load();
		void bulk_load_int_chunk(const size_t aSize);

	private:
		/*reads the next tuple in the file and writes it into the parameter specified chunk*/
		void readTuple(unval_t*& aChunk);

	private:
		LineReader _lineReader;
		/*char 'NUL' (code 0) no delimiter*/
		char _delimiter;
		char _seperator;
		IntermediateBuffer _intermediate_buffer;
		Relation& _relation;


};

const IntermediateBuffer& BulkLoader::getIntermediateBuffer() const
{
	return _intermediate_buffer;
}

#endif
