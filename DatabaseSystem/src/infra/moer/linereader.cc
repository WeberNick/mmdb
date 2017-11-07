#include "linereader.hh"

LineReader::LineReader(const char* aFileName, 
                       uint aLineBufferSize,
                       char aCommentChar) 
           : _streamgiven(false),
             _filename(aFileName),
             _linebuffersize(aLineBufferSize),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _buffer(0),
             _ifs(0),
             _is(std::cin),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _noSplit(0),
             _splits(),
             _splitsEnd(),
             _subsplitsBegin(),
             _subsplitsEnd() {
  _buffer = new char[linebuffersize() + 3];
}

LineReader::LineReader(std::istream& aIs,
                       uint aLineBufferSize,
                       char aCommentChar)
           : _streamgiven(true),
             _filename("no-file"),
             _linebuffersize(aLineBufferSize),
             _commentchar(aCommentChar), // for comments until end of line, must occur at beginning of line
             _buffer(0),
             _ifs(0),
             _is(aIs),
             _ok(false),
             _linesize(0),
             _linecount(0),
             _commentlinecount(0),
             _no_bytes_read(0),
             _noSplit(0),
             _splits(),
             _splitsEnd(),
            _subsplitsBegin(),
            _subsplitsEnd() {
  _buffer = new char[linebuffersize()];
}

LineReader::~LineReader() {
  if(0 != _ifs) {
    delete _ifs;
  }
  delete[] _buffer;
}

bool
LineReader::open() {
  if(_streamgiven) {
    if(_is) {
      _ok = true;
    } else {
      _ok = false;
    }
  } else {
    if(0 != _ifs) {
      throw SimpEx3("The file \'", filename(), "\' has already been opened");
    }
    _ifs = new std::ifstream(filename());
    if(!ifs()) {
      throw SimpEx3("Cannot open file \'", filename(), "\'");
    }
    _ok = true;
    _no_bytes_read = 0;
  }
  getNonCommentLine();
  return (_ok);
}

bool
LineReader::next() {
  getNonCommentLine();
  return ok();
}

void
LineReader::close() {
  if(0 != _ifs) {
    delete _ifs;
    _ifs = 0;
  }
}

void
LineReader::getNonCommentLine() {
  if(!ok()) { return; }
  while(!ifs().eof()) {
    ifs().getline(_buffer, linebuffersize(), '\n');
    size_t lGCount = ifs().gcount();
    _linesize = lGCount - 1; // '\n' also counts as one byte read
    _no_bytes_read += (uint64_t) (_linesize + 1);
    if(1 > ifs().gcount()) { // empty line
      continue;
    }
    if(lGCount >= (linebuffersize() - 1)) {
      throw SimpEx3("Buffer too small for file \'", filename(), "\'");
    }
    ++_linecount;
    const char* x = _buffer;
    if(skipws(x) && commentchar() == (*x)) {
      ++_commentlinecount;
      continue;
    }
    if(x == end()) { // line contains only blanks
      continue;
    }
    break;
  }
  if(ifs().eof()) { _ok = false; }
}

/* Utilities */

bool
LineReader::skipws(const char*& x) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while((x < e) && isspace(*x)) { ++x; }
  return (x < e);
}

bool
LineReader::skipuntil(const char*& x, const char aCharToStopAt) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  while(aCharToStopAt != (*x) && (x < e)) { ++x; }
  return (x == e) || (aCharToStopAt == *x);
}

bool
LineReader::skipuntil(const char*& x, const char aCharToStopAt, const char aCharEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return true; }
  while((aCharToStopAt != (*x)) && (aCharEnd != (*x)) && (x < e)) { ++x; }
  return (x >= e) && (aCharEnd != (*x));
}

bool
LineReader::read_string_set(const char*& x, string_st& aSetOut, const char aStringSep, const char aSep) {
  const char* e = end();
  const char* lBegin = 0;
  const char* lEnd = 0;
  if(x < begin() || e <= x) { return false; }
  while((aSep != *x) && (x < e)) {
    if(aStringSep == (*x)) { ++x; continue; }
    lBegin = x;
    skipuntil(x, aStringSep, aSep);
    lEnd = x;
    if(lBegin < lEnd) {
      aSetOut.insert(std::string(lBegin, lEnd));
    }
  }
  return true;
}



bool
LineReader::read_char(const char*& x, char& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  out = *x;
  ++x;
  return true;
}

bool
LineReader::read_uint(const char*& x, unsigned int& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || x < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_uint64(const char*& x, uint64_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int64_t lRead = strtoll(x, &lEndPtr, 10);
  if(lEndPtr == x || x < 0) {
    return false;
  }
  out = (uint64_t) lRead;
  x = lEndPtr;
  return true;
}



bool
LineReader::read_int(const char*& x, int& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_float(const char*& x, float& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  float lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_double(const char*& x, double& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  char* lEndPtr = 0;
  double lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_string(const char*& x, char aDelimiter, const char*& aBegin, const char*& aEnd) {
  const char* e = end();
  // std::cout << "LR 1" << std::endl;
  if(x < begin() || e <= x) { return false; }
  // std::cout << "LR 2" << std::endl;
  if(skipuntil(x, aDelimiter)) {
    // std::cout << "LR 3" << std::endl;
    ++x; // skip the delimiter
    const char* lBegin = x;
    if(skipuntil(x, aDelimiter)) {
      // std::cout << "LR 4" << std::endl;
      const char* lEnd = x;
      ++x; // do not skip the delimiter
      aBegin = lBegin;
      aEnd   = lEnd;
      return true;
    }
  }
  return false;
}

bool
LineReader::read_string_no_delim(const char*& x, 
                                 char aSep, 
                                 const char*& aBegin, 
                                 const char*& aEnd) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  const char* lBegin = x;
  if(skipuntil(x, aSep)) {
    const char* lEnd = x;
    // ++x; // do not skip the delimiter
    aBegin = lBegin;
    aEnd   = lEnd;
    return true;
  }
  return false;
}


bool
LineReader::read_ival(const char*& x, ival_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || x < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_uval(const char*& x, uval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  int lRead = strtol(x, &lEndPtr, 10);
  if(lEndPtr == x || x < 0) {
    return false;
  }
  out = (unsigned int) lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_fval(const char*& x, fval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  float lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_dval(const char*& x, dval_t& out) {
  const char* e = end();
  if(x < begin() || e <= x) { return false; }
  if(0 == strncmp("NULL", x, 4)) {
    out.setNull();
    x += 4;
    return true;
  }
  char* lEndPtr = 0;
  double lRead = strtof(x, &lEndPtr);
  if(lEndPtr == x) {
    return false;
  }
  out = lRead;
  x = lEndPtr;
  return true;
}

bool
LineReader::read_stimestamp(const char*& x, STimestamp& out, char sep) {
  return out.set(x, sep);
}

int
LineReader::split_line(const char aSep, const bool aStrip) {
  _splits.clear();
  const char* e = end();
  const char* x = _buffer;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    while(aSep != *x && x < e && '\0' != *x) ++x;
    // if(('\0' == *x) || (e <= x)) {
    //   break;
    // }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _splits.push_back((char*) lBegin);

    ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

int
LineReader::split_line(const char aSep, const char aDelim, const bool aStrip) {
  _splits.clear();
  _splitsEnd.clear();
  const char* e = end();
  const char* x = _buffer;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if(aDelim == (*x)) {
        lInString = !lInString;
        // std::cout << "TOGGLE" << std::endl;
      }
      ++x;
    }
    if(lInString) {
      std::cerr << "unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _splits.push_back((char*) lBegin);
    _splitsEnd.push_back((char*) lEnd);
     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

int
LineReader::split(const char* aBegin, const char* aEnd,
                  const char aSep, const char aDelim, const bool aStrip) {
  _subsplitsBegin.clear();
  _subsplitsEnd.clear();
  const char* e = aEnd;
  const char* x = aBegin;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if(aDelim == (*x)) {
        lInString = !lInString;
        // std::cout << "TOGGLE" << std::endl;
      }
      ++x;
    }
    if(lInString) {
      std::cerr << "unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _subsplitsBegin.push_back((char*) lBegin);
    _subsplitsEnd.push_back((char*) lEnd);

     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}



int
LineReader::splitDoubleDelim(const char* aBegin, const char* aEnd,
                             const char aSep, const char aDelim, const bool aStrip) {
  _subsplitsBegin.clear();
  _subsplitsEnd.clear();
  const char* e = aEnd;
  const char* x = aBegin;
  if(aStrip) {
    skipws(x);
  }
  while(x < e) {
    // std::cout << "  loop: " << x << std::endl;
    const char* lBegin = x;
    // std::cout << "  begin: " << lBegin << std::endl;
    bool lInString = false;
    while((lInString || aSep != *x) && x < e && '\0' != *x) {
      if(aDelim == (*x)) {
        if(aDelim == (*(x+1))) {
          // std::cout << "TOGGLE" << std::endl;
          lInString = !lInString;
          ++x;
        }
      }
      ++x;
    }
    if(lInString) {
      std::cerr << "unclosed string at line: " << linecount() << std::endl;
      throw SimpEx1("unclosed string");
    }
    char* lEnd = (char*) x;
    // std::cout << "  end: " << lEnd << std::endl;
    if(aStrip) {
      while(lEnd >= lBegin && isspace(*(lEnd - 1))) --lEnd;
    }
    // std::cout << "  end: " << lEnd << std::endl;
    *lEnd = '\0';
    _subsplitsBegin.push_back((char*) lBegin);
    _subsplitsEnd.push_back((char*) lEnd);

     ++x; // skip sep
    if(aStrip) {
      skipws(x);
    }
  }
  return splits().size();
}

