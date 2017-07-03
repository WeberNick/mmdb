#ifndef PAGE_INTERPRETER_SP_HH
#define PAGE_INTERPRETER_SP_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"


class PageInterpreterSP {
  public:
    struct header_t {
        uint16_t _noRecords;      // number of records stored on this page
        uint16_t _freeSpace;      // total number of free bytes
        uint16_t _nextFreeRecord; // 8 byte aligned offset to next free record
        uint16_t _placeholder;
    };
    struct slot_t {
      uint16_t _offset; // offset to record
    };
  public:
    PageInterpreterSP();
  public:
    void  attach(byte* aPP);
    void  detach();
  public:
    void  initNewPage(byte* aPP); // combines initialization of fresh page with attach
    byte* addNewRecord(const uint aRecordSize); // returns 0 if page is full
  public:
    inline const byte*     pagePtr() const { return _pp; }
    inline       byte*     pagePtr()       { return _pp; }
    inline const header_t* header() const { return _header; }
    inline       header_t* header()       { return _header; }
    inline uint  freeSpace() const { return header()->_freeSpace; }
    inline uint  noRecords() const { return header()->_noRecords; }
  
    inline const byte* getRecord(const uint aRecordNo) const;
    inline       byte* getRecord(const uint aRecordNo);
    inline const slot_t& slot(const uint i) const { return _slots[- (int) i]; }
    inline       slot_t& slot(const uint i)       { return _slots[- (int) i]; }
  private:
    inline header_t* get_hdr_ptr() { return ((header_t*) (_pp + PAGE_SIZE - sizeof(header_t))); }
    inline slot_t*   get_slot_base_ptr() { return ((slot_t*) (_pp + PAGE_SIZE - sizeof(header_t) - sizeof(slot_t))); }
  public:
    byte*     _pp;
    header_t* _header;
    slot_t*   _slots;  // new
};

const byte* 
PageInterpreterSP::getRecord(const uint aRecordNo) const { 
  if(aRecordNo >= noRecords()) { 
    return 0; 
  }
  return (_pp + slot(aRecordNo)._offset); 
}

byte* 
PageInterpreterSP::getRecord(const uint aRecordNo) { 
  if(aRecordNo >= noRecords()) { 
    return 0; 
  }
  return (_pp + slot(aRecordNo)._offset); 
}

#endif
