#include "intermediate_buffer.hh"

IntermediateBuffer::IntermediateBuffer() :
	_tuplePerChunk(0),
	_tupleTotal(0),
	_buffer()
{

}

IntermediateBuffer::IntermediateBuffer(const size_t aBufferSize) :
	_tuplePerChunk(aBufferSize),
	_tupleTotal(0),
	_buffer()
{

}

IntermediateBuffer::IntermediateBuffer(const IntermediateBuffer& aBuffer) :
	_tuplePerChunk(aBuffer._tuplePerChunk),
	_tupleTotal(aBuffer._tupleTotal),
	_buffer(aBuffer._buffer)
{

}

IntermediateBuffer::~IntermediateBuffer()
{
	for(size_t i = 0; i < _buffer.size(); ++i)
	{
		delete[] _buffer[i];
	}
}

IntermediateBuffer& IntermediateBuffer::operator= (const IntermediateBuffer& aBuffer)
{
	if(this != &aBuffer)
	{
		_tuplePerChunk 	= aBuffer._tuplePerChunk;
		_tupleTotal 	= aBuffer._tupleTotal;
		_buffer 		= aBuffer._buffer;
	}
	return *this;
}

void IntermediateBuffer::setTupleTotal(const size_t aTotalTupleNo)
{
	_tupleTotal = aTotalTupleNo;
}

void IntermediateBuffer::addChunkToBuffer(const unval_pt aChunkPointer)
{
	_buffer.push_back(aChunkPointer);
}