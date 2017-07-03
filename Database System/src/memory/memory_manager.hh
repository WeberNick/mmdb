#ifndef MEMORY_MANAGER_HH
#define MEMORY_MANAGER_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "infra/webe/exception.hh"
#include <vector>

class MemoryManager
{
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