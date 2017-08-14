/**
 *	@file 	segment.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	This class represents a memory segment
 *	@bugs 	Currently no bugs known
 *	@todos	Think about exception handling
 *
 *	@section DESCRIPTION
 *	This class serves as layer between a relation and its allocated memory. Each relation has an associated segment as a member variable.
 *	If the relation's memory pages are full, the relation can use its segment in order to request a new (empty) memory page.
 *	When a new page is requested, the Segment will forward this request to the memory manager. At success (a new page could be allocated),
 *	the segment adds the address of the allocated memory page into its vector of previously allocated pages. The segment maintains all
 *	the allocated memory pages and implements access routines to these pages.
*/
#ifndef SEGMENT_HH
#define SEGMENT_HH

#include "infra/webe/types.hh"
#include "memory_manager.hh"

#include <vector>

/**
 *	This class implements a Segment which is used to access and allocate memory pages
 */
class Segment
{
	public:
		/* constructs a new segment */
		Segment();
		/* copies a segment */
		Segment(const Segment& aSegment);
		/* destructs a segment */
		~Segment();
		/* specifies the assign operator of a segment */
		Segment& operator= (const Segment& aSegment);

	public:
		/**
		 *	@brief	Access routine to the current number of allocated pages
		 *
		 *	@return	the current number of allocated pages in this segment
		 */
		inline const uint getNoPages() const;
		inline uint getNoPages();

		/**
		 *	@brief	Access routine to the current number of allocated pages
		 *
		 *	@param 	pid  the respective page number in the segment 
		 *	@return	address of the page inside the page vector (at the parameter specified index)
		 */
		inline const byte* getPage(uint pid) const;
		inline byte* getPage(uint pid);

		/**
		 *	@brief	This function is used to request new (empty) pages
		 *
		 *	@return	address to the requested new (empty) page
		 */
		byte* getNewPage();

	private:
		/* a vector maintaining addresses to all allocated memory pages */
		byte_vpt _pages;


};

const uint Segment::getNoPages() const
{
	return _pages.size();
}

uint Segment::getNoPages()
{
	return _pages.size();
}

const byte* Segment::getPage(uint pid) const
{
	return _pages[pid];
}

byte* Segment::getPage(uint pid)
{
	return _pages[pid];
}

#endif