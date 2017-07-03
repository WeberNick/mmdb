#ifndef SEGMENT_HH
#define SEGMENT_HH

#include <vector>
#include "memory_manager.hh"
#include "infra/webe/types.hh"

class Segment
{
	public:
		/*return the segment ID*/
		uint16_t getSegID();
		/*return address of the page (at the parameter specified index) inside the page vector*/
		byte* getPage(uint pid);
		/*return address to a new free page*/
		byte* getNewPage();
		uint getNoPages();

	public:
		/*constructs a Segment*/
		Segment();
		Segment& operator= (const Segment& aSegment);
		

	private:
		static uint16_t _IDcounter;
		uint16_t _segID;
		byte_vpt _pages;


};


#endif