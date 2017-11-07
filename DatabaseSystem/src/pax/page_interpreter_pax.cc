#include "page_interpreter_pax.hh"

bool PageInterpreterPAX::_pageSizeSet = false;
size_t PageInterpreterPAX::_pageSize = 0;

PageInterpreterPAX::PageInterpreterPAX() : _pp(NULL), _header(NULL), _slot(NULL) {
}

void PageInterpreterPAX::setPageSize(const size_t aPageSize)
{
  if(!_pageSizeSet)
  {
    _pageSizeSet = !_pageSizeSet;
    _pageSize = aPageSize;
  }
}

void 
PageInterpreterPAX::initNewPage(byte* aPP, const uint_vt& aPartitionData) {
  if(aPP)
  {
    attach(aPP);
    header()->_noAttributes   = 0;
    header()->_noRecords   = 0;
    header()->_freeSpace   = (_pageSize - sizeof(header_t));
    header()->_maxRecords = 0;
    pagePartition(aPartitionData);
  }
}

int
PageInterpreterPAX::addNewRecord(const uint aRecordSize) 
{
  bool flag = (aRecordSize <= freeSpace()) && (header()->_noRecords < header()->_maxRecords);
  if(flag) {
    int index = header()->_noRecords;
    header()->_freeSpace -= aRecordSize;
    header()->_noRecords += 1;
    return index;
  }
  return -1;
}

void PageInterpreterPAX::pagePartition(const uint_vt& aAttrSizeVec)
{
  header()->_noAttributes = aAttrSizeVec.size() - 1;
  uint lFreeSpace = (_pageSize - sizeof(header_t) - (header()->_noAttributes * sizeof(slot_t)));
  uint lTotalAttrSize = aAttrSizeVec[header()->_noAttributes];

  uint lNoRec = (lFreeSpace / lTotalAttrSize) + 1;
  uint lSizeWithAlignment;
  do
  {
    --lNoRec;
    lSizeWithAlignment = 0;
    for(int i = 0; i < get_hdr_ptr()->_noAttributes; ++i)
    {
      slot(i)._miniPageOffset = lSizeWithAlignment;
      slot(i)._miniPageAttrSize = aAttrSizeVec[i];
      lSizeWithAlignment += ((lNoRec * aAttrSizeVec[i] + 7) & ~(uint) 0x07); //8byte alignment
    }
  }
  while(!(lSizeWithAlignment <= lFreeSpace));
  header()->_freeSpace -= (header()->_noAttributes * sizeof(slot_t));
  header()->_maxRecords = lNoRec;
}