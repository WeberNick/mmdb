#include "exception.hh"

BaseException::BaseException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const std::string& aErrorMessage) : 
	_file(aFileName),
	_line(aLineNumber),
	_func(aFunctionName),
	_errMsg(aErrorMessage) 
{}

BaseException::BaseException(const BaseException& ex) : 
	_file(ex._file),
	_line(ex._line),
	_func(ex._func),
	_errMsg(ex._errMsg) 
{}

BaseException& BaseException::operator=(const BaseException& ex) 
{
  _file = ex._file;
  _line = ex._line;
  _func = ex._func;
  _errMsg  = ex._errMsg;
  return (*this);
}

void BaseException::print() const 
{
  std::cerr << "An error occured in " << _file << ", line " << _line  << ", '" << _func << "':\n\t\"" << _errMsg << "\"" << std::endl;
}

NoMemoryManagerException::NoMemoryManagerException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName) :
	BaseException(aFileName, aLineNumber, aFunctionName, "No MemoryManager created. Cannot allocate memory without a MemoryManager.")
{}

OutOfMemoryException::OutOfMemoryException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName) :
	BaseException(aFileName, aLineNumber, aFunctionName, "There was insufficient memory to fulfill the allocation request.")
{}

InvalidArguementException::InvalidArguementException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const std::string& aErrorMessage) :
	BaseException(aFileName, aLineNumber, aFunctionName, aErrorMessage)
{}

SwitchException::SwitchException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName) :
	BaseException(aFileName, aLineNumber, aFunctionName, "Default section in switch-statement accessed.")
{}

FileException::FileException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const char* aErrorFileName, const std::string& aErrorMessage) :
	BaseException(aFileName, aLineNumber, aFunctionName, "Problem in file " + std::string(aErrorFileName) + ": " + aErrorMessage)
{}

IOException::IOException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName) : 
	BaseException(aFileName, aLineNumber, aFunctionName, "IO exception occured.")
{}
