#include "memory_manager.hh"

std::vector<byte*> MemoryManager::_chunks = {};
byte* MemoryManager::_firstFreePage = 0;
byte* MemoryManager::_endOfChunk = 0;

byte* MemoryManager::allocAlignedMemChunk(const uint aNumbOfPages)
{
	byte* lChunkPointer;
	switch(posix_memalign((void**)&lChunkPointer, ALIGNMENT_GLOBAL, aNumbOfPages * PAGE_SIZE_GLOBAL))
	{
		case 0:
			MemoryManager::_chunks.push_back(lChunkPointer);
			break;
		case ENOMEM /*12*/:
			throw OutOfMemoryException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			break;
		case EINVAL /*22*/:
			throw InvalidArguementException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "The alignment argument was not a power of two, or was not a multiple of sizeof(void *).");
			break;
		default:
			throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			break;
	}
	return lChunkPointer;
}

void MemoryManager::freeAlignedMemChunk(byte* aPointer)
{
	free(aPointer);
}

byte* MemoryManager::getPage()
{
	if(MemoryManager::_firstFreePage >= MemoryManager::_endOfChunk)
	{
		MemoryManager::_firstFreePage = MemoryManager::allocAlignedMemChunk(MEMCHUNK_SIZE_GLOBAL);
		MemoryManager::_endOfChunk = _firstFreePage + MEMCHUNK_SIZE_GLOBAL * PAGE_SIZE_GLOBAL;	
	}

	byte* lFreePage = MemoryManager::_firstFreePage;
	MemoryManager::_firstFreePage += PAGE_SIZE_GLOBAL;
	return lFreePage;
}
		
void MemoryManager::freeMemory(byte* aPointer)
{
	MemoryManager::freeAlignedMemChunk(aPointer);
}

void MemoryManager::freeAll()
{
    _firstFreePage = 0;
    _endOfChunk = 0;
	for (uint i = 0; i < _chunks.size(); ++i)
	{
		MemoryManager::freeAlignedMemChunk(_chunks[i]);
	}
	_chunks.clear();
}
