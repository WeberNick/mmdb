#include "segment.hh"

uint16_t Segment::_IDcounter = 0;

Segment::Segment() : _segID(_IDcounter++), _pages()
{
}

Segment& Segment::operator=(const Segment& aSegment)
{
	if(this != &aSegment)
	{
		_segID = aSegment._segID;
		_pages = aSegment._pages;
	}
	return *this;
}

uint16_t Segment::getSegID()
{
	return _segID;
}

byte* Segment::getNewPage()
{
	try 
	{
		byte* lPage = MemoryManager::getPage();
		_pages.push_back(lPage);
		return lPage;
	}
	catch(BaseException& ex)
	{
		ex.print();
		return 0;
	}
}

byte* Segment::getPage(uint pid)
{
	return _pages[pid];
}

uint Segment::getNoPages()
{
	return _pages.size();
}