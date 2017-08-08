/**
 *	@file 	memory_manager.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Singleton class handling the allocation and deallocation of memory
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class manages all the memory allocations and deallocations. The memory pages are organized
 *	in chunks of memory (size of chunk is user defined). The MemoryManager maintains pointer to its
 *	memory chunks in an vector of pointer. As soon a chunk is full, the next request for memory leads
 *	to the allocation of a new chunk. This class is a singleton, so only one instance of it can be
 *	constructed (private constructor) by calling the function createInstance(). Multiple calls to
 *	createInstance() are possible but this will not create multipe MemoryManager instances. To access
 *	the MemoryManager instance, getInstance() must be called. This only works if the instance was
 *	previously constructed. By calling destroyInstance() the single instance is destroyed forever
 *	and all the allocated memory is freed.
 */
#ifndef MEMORY_MANAGER_HH
#define MEMORY_MANAGER_HH

#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"

#include <vector>
#include <iostream>

class MemoryManager
{
	private:
		/* private constructor so nobody but this class can construct an object */
		MemoryManager(const size_t aPageAlignment, const size_t aMemoryChunkSize, const size_t aPageSize);
		/* this is just to prevent compiler warnings */
		MemoryManager(const MemoryManager& aMemoryManager) = delete;
		/* this is just to prevent compiler warnings */
		MemoryManager& operator=(const MemoryManager& aMemoryManager) = delete;
		/* destructor to destroy the MemoryManager object */
		~MemoryManager();

	public:
		/**
		 *	@brief	This function is the only way to construct an MemoryManager object.
		 *			If there was already an MemoryManager object created this function
		 *			will print an error message
		 *
		 *	@param 	aPageAlignment  a page alignment to use for allocated memory pages
		 *	@param 	aMemoryChunkSize  the memory chunk size to use (i.e., how much pages to allocate together)
		 *	@param 	aPageSize  a page size to use for the allocated memory pages
		 *
		 */
		static void createInstance(const size_t aPageAlignment, const size_t aMemoryChunkSize, const size_t aPageSize);

		/**
		 *	@brief	Destroys the MemoryManager instance if it was previously constructed
		 */
		static void destroyInstance();

		/**
		 *	@brief	This function is the only way to get access to the MemoryManager instance
		 *
		 *	@return	pointer to the only MemoryManager instance
		 */
		static MemoryManager* getInstance();

	public:
		/**
		 *	@brief	This function will return a pointer to the first free memory page.
		 *			If there is no free memory page, this function will make a request
		 *			to allocate a new memory chunk
		 *
		 *	@return	address of the first free page in the current memory chunk
		 */
		byte* getPage();

		/**
		 *	@brief	deallocates a memory region pointed to by the parameter
		 *
		 *	@param 	aPointer  a pointer to a memory region which shall be deallocated
		 */
		void freeMemory(byte* aPointer);

		/**
		 *	@brief	frees all allocated memory by the MemoryManager
		 */
		void freeAll();

	private:
		/**
		 *	@brief	allocates a new memory chunk
		 *
		 *	@param 	aNumbOfPages  the size of the memory chunk to allocate
		 *	@return	a pointer to the allocated memory chunk
		 */
		byte* allocAlignedMemChunk(const size_t aNumbOfPages);

		/**
		 *	@brief	deallocates a memory chunk pointed to by the parameter
		 *
		 *	@param 	aPointer a pointer to the memory chunk which shall be deallocated
		 */
		void freeAlignedMemChunk(byte* aPointer);

	private:
		/* this variable indicates if a instance of the MemoryManager exists at the moment */
		static bool instance_created;
		/* the memory page alignment to use */
		size_t _alignment;
		/* the memory chunk size to use */
		size_t _chunkSize;
		/* the memory page size to use */
		size_t _pageSize;
		/* maintains pointer to all allocated memory chunks */
		byte_vpt _chunks;
		/* address of the first free memory page in the current memeory chunk */
		byte* _firstFreePage;
		/* address of the end in the current memory chunk */
		byte* _endOfChunk;
		/* pointer to the only MemoryManager instance (0 if not constructed) */
		static MemoryManager* mm_instance;
};



#endif