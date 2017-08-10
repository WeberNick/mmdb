/**
 *	@file 	exception.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Provides a central class for exception handling
 *	@bugs 	Currently no bugs known
 *	@todos	Needs major rework and redesign
 *
 *	@section DESCRIPTION
 *	TODO
 *
 */
#ifndef INFRA_WEBE_EXCEPTION_HH
#define INFRA_WEBE_EXCEPTION_HH

#include <exception>
#include <string>
#include <iostream>
#include <errno.h>

class BaseException : public std::exception
{
	public:
		BaseException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const std::string& aErrorMessage); 
		BaseException(const BaseException&);
	    BaseException& operator=(const BaseException&);
		virtual const char* what() const throw() {return _errMsg.c_str();}
    	const std::string& msg() const {return _errMsg;}
	    void print() const;

	private:
		const char*  _file;
   		unsigned int _line;
    	const char*  _func;
    	std::string _errMsg;

};

class NoMemoryManagerException : public BaseException
{
	public:
		NoMemoryManagerException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName);
};

class OutOfMemoryException : public BaseException
{
	public:
		OutOfMemoryException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName);
};

class InvalidArguementException : public BaseException
{
	public:
		InvalidArguementException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const std::string& aErrorMessage);
};

class SwitchException : public BaseException
{
	public:
		SwitchException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName);
};

class FileException : public BaseException
{
	public:
		FileException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName, const char* aErrorFileName, const std::string& aErrorMessage); 
};

class IOException : public BaseException
{
	public:
		IOException(const char*  aFileName, unsigned int aLineNumber, const char* aFunctionName);
};

#endif