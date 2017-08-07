/**
 *	@file 	bulk_loader.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	A bulk loader class to read a data from a file and write it into a buffer
 *	@bugs 	Currently no bugs known
 *	@todos	Better exception handling
 *
 *	@section DESCRIPTION
 *	This class implements a bulk loader which allows to read data from a file and write it
 *	into a intermediate buffer. This buffer then can be passed to the bulk insert
 * 	(@see bulk_insert_nsm.hh and @see bulk_insert_pax.hh respectively) to write the data
 *	from the buffer into the (in memory) database.	
 */
#ifndef INFRA_WEBE_BULK_LOADER_HH
#define INFRA_WEBE_BULK_LOADER_HH

#include "types.hh"
#include "common.hh"
#include "exception.hh"
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
		BulkLoader(const char* aFileName, Relation& aRelation, char aDelimiter, char aSeperator, size_t aBufferSize);
		/* constructs a bulk loader object used for a specific artificial relation */
		BulkLoader(Relation& aRelation, size_t aBufferSize);
		/* destructs a bulk loader object */
		~BulkLoader();

	public:
		/*return reference to the bulk loaders buffer*/
		inline const unval_vpt& getBuffer() const;
		/*return referemce to the bulk loaders associated relation*/
		inline const Relation& getRelation() const;
		/*return number of tuple stored in each chunk*/
		inline const size_t getTuplePerChunk() const;
		/*return total number of tuple in all chunks (=linecount)*/
		inline const uint getTupleTotal() const;

	public:
		/*start the bulk loading process*/
		void bulk_load();
		void bulk_load_int_chunk(const size_t aSize);

	
		
	private:
		LineReader _lineReader;
		/*char 'NUL' (code 0) no delimiter*/
		char _delimiter;
		char _seperator;
		size_t _tuplePerChunk;
		uint _tupleTotal;
		Relation& _relation;
		unval_vpt _buffer;
		/*reads the next tuple in the file and writes it into the parameter specified chunk*/
		void readTuple(unval_t*& aChunk);


};

const unval_vpt& BulkLoader::getBuffer() const
{
	return _buffer;
}

const Relation& BulkLoader::getRelation() const
{
	return _relation;
}

const size_t BulkLoader::getTuplePerChunk() const
{
	return _tuplePerChunk;
}

const uint BulkLoader::getTupleTotal() const
{
	return _tupleTotal;
}


#endif
