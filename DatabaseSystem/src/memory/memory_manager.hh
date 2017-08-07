/**
 *	@file 	memory_manager.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Handles the allocation and deallocation of memory
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class manages all the memory allocations and deallocations. The memory pages are organized
 *	in chunks of memory (size of chunk is user defined). The MemoryManager maintains pointer to its
 *	memory chunks in an vector of pointer. As soon a chunk is full, the next request for memory leads
 *	to the allocation of a new chunk.
 */
#ifndef MEMORY_MANAGER_HH
#define MEMORY_MANAGER_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "infra/webe/exception.hh"
#include <vector>

class MemoryManager
{
	// public:
	// 	static MemoryManager& getInstance() 
	// 	{
	// 		static MemoryManager mm;
	// 		return mm;
	// 	}

	// private:
	//     MemoryManager();
	//     ~MemoryManager();

	// private:
	// 	static MemoryManager mm_instance;

	public:
		/*return address of the first free page in the current chunk*/
		static byte* getPage();
		/*free the dynamic allocated memory chunk at the parameter address*/
		static void freeMemory(byte* aPointer);
		/*free all dynamic allocated memory chunks*/
		static void freeAll();

	private:
		/*handles allocation of memory. Is called by getPage()*/
		static byte* allocAlignedMemChunk(uint aNumbOfPages);
		/*handles deallocation of previously dynamic allocated memory. Is called by freeMemory(byte*)*/
		static void freeAlignedMemChunk(byte* aPointer);

	private:
		static byte_vpt _chunks;
		static byte* _firstFreePage;
		static byte* _endOfChunk;

};



#endif