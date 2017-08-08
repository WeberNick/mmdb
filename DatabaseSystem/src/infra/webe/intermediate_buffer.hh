/**
 *	@file 	intermediate_buffer.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Implementation of an intermediate buffer
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class implements a intermediate buffer (probably) mainly used by the bulk loader.
 *	The bulk loader reads data from a file and needs to store the data in an intermediate 
 *	buffer before the bulk insert routine stores the data in memory.
 */
#ifndef INFRA_WEBE_INTERMEDIATE_BUFFER_HH
#define INFRA_WEBE_INTERMEDIATE_BUFFER_HH

#include "types.hh"

class IntermediateBuffer
{
 	public:
 		/* standard constructor */
 		IntermediateBuffer();
 		/* constructor */
 		IntermediateBuffer(const size_t aBufferSize);
 		/* copy constructor */
 		IntermediateBuffer(const IntermediateBuffer& aBuffer);
 		/* destructor */
 		~IntermediateBuffer();
 		/* specifies the assign operator of a intermediate buffer */
		IntermediateBuffer& operator= (const IntermediateBuffer& aBuffer);

 	public:
 		/**
		 *	@brief	get access to the number of tuple stored in each buffer chunk
		 *
		 *	@return	the number of tuple stored in each buffer chunk
		 */
		inline const size_t getTuplePerChunk() const;

		/**
		 *	@brief	get access to the the total number of tuples stored in the buffer
		 *
		 *	@return	the total number of tuple stored in all chunks
		 */
		inline const size_t getTupleTotal() const;

		/**
		 *	@brief	sets the the total number of tuples stored in the buffer
		 *
		 *	@param	aTotalTupleNo  the total number of tuple stored in all chunks
		 */
		void setTupleTotal(const size_t aTotalTupleNo);

		/**
		 *	@brief	get access to the buffer
		 *
		 *	@return	reference to the buffer
		 */
		inline const unval_vpt& getBuffer() const;

		/**
		 *	@brief	adds a chunk pointer to the buffer
		 *
		 *	@param	aChunkPointer  a pointer to the chunk
		 */
		void addChunkToBuffer(const unval_pt aChunkPointer);

	private:
		size_t _tuplePerChunk;
		size_t _tupleTotal;
		unval_vpt _buffer;
};

const size_t IntermediateBuffer::getTuplePerChunk() const
{
	return _tuplePerChunk;
}

const size_t IntermediateBuffer::getTupleTotal() const
{
	return _tupleTotal;
}

const unval_vpt& IntermediateBuffer::getBuffer() const
{
	return _buffer;
}

#endif