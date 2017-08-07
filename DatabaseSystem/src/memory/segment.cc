#include "segment.hh"


Segment::Segment() : _pages()
{}

Segment::Segment(const Segment& aSegment) : _pages(aSegment._pages)
{}

Segment::~Segment()
{}

Segment& Segment::operator=(const Segment& aSegment)
{
	if(this != &aSegment)
	{
		_pages = aSegment._pages;
	}
	return *this;
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