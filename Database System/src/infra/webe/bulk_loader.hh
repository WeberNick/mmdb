#ifndef INFRA_WEBE_BULK_LOADER_HH
#define INFRA_WEBE_BULK_LOADER_HH

#include <vector>
#include <string>
#include <cstring>
#include <stddef.h>

#include "types.hh"
#include "common.hh"
#include "exception.hh"
#include "relation/relation.hh"
#include "infra/moer/linereader.hh"
#include "infra/moer/datejd.hh"
#include "infra/moer/simplecharcontainer.hh"
#include "infra/moer/simpledictionary.hh"


class BulkLoader
{
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

	public:
		/*constructs a bulk loader object*/
		BulkLoader(const char* aFileName, Relation& aRelation, char aDelimiter, char aSeperator, size_t aBufferSize);
		BulkLoader(Relation& aRelation, size_t aBufferSize);
		~BulkLoader();
		
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
