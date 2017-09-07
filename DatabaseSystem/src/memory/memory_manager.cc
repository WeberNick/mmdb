#include "memory_manager.hh"

bool MemoryManager::instance_created = false;
MemoryManager* MemoryManager::mm_instance = NULL;

MemoryManager::MemoryManager(const size_t aPageAlignment, const size_t aMemoryChunkSize, const size_t aPageSize) :
	_alignment(aPageAlignment),
	_chunkSize(aMemoryChunkSize),
	_pageSize(aPageSize),
	_chunks(),
	_firstFreePage(0),
	_endOfChunk(0)
{
	std::cout << "MemoryManager created." << std::endl;
}

MemoryManager::~MemoryManager()
{
	freeAll();
	MemoryManager::mm_instance = NULL;
	MemoryManager::instance_created = false;
	std::cout << "MemoryManager destroyed" << std::endl;
}

void MemoryManager::createInstance(const size_t aPageAlignment, const size_t aMemoryChunkSize, const size_t aPageSize)
{
	if(!MemoryManager::instance_created)
	{
		MemoryManager::instance_created = true;
		MemoryManager::mm_instance = new MemoryManager(aPageAlignment, aMemoryChunkSize, aPageSize);
	}
	else
	{
		std::cerr << "ERROR: Instance of MemoryManager already created" << std::endl;
	}
}

void MemoryManager::destroyInstance()
{
	if(MemoryManager::instance_created)
	{ 
		delete MemoryManager::mm_instance;
	}
	else
	{
		std::cerr << "ERROR: Instance of MemoryManager was never created" << std::endl;
	}
}


MemoryManager* MemoryManager::getInstance()
{
	if(MemoryManager::instance_created)
	{ 
		return MemoryManager::mm_instance;
	}
	else
	{
		std::cerr << "ERROR: Instance of MemoryManager was never created" << std::endl;
		return NULL;
	}
}



byte* MemoryManager::getPage()
{
	if(_firstFreePage >= _endOfChunk)
	{
		_firstFreePage = allocAlignedMemChunk(_chunkSize);
		_endOfChunk = _firstFreePage + _chunkSize * _pageSize;	
	}

	byte* lFreePage = _firstFreePage;
	_firstFreePage += _pageSize;
	return lFreePage;
}
		
void MemoryManager::freeMemory(byte* aPointer)
{
	freeAlignedMemChunk(aPointer);
}

void MemoryManager::freeAll()
{
    _firstFreePage = 0;
    _endOfChunk = 0;
	for (uint i = 0; i < _chunks.size(); ++i)
	{
		freeAlignedMemChunk(_chunks[i]);
	}
	_chunks.clear();
	std::cout << "All allocated memory now free" << std::endl;
}

byte* MemoryManager::allocAlignedMemChunk(const size_t aNumbOfPages)
{
	byte* lChunkPointer;
	switch(posix_memalign((void**)&lChunkPointer, _alignment, aNumbOfPages * _pageSize))
	{
		case 0:
			_chunks.push_back(lChunkPointer);
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


