/**
 *	@file 	types.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Definition of various additional data types
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This class defines various additional data types which are helpful for readability and abstraction.
*/
#ifndef INFRA_WEBE_TYPES_HH
#define INFRA_WEBE_TYPES_HH

#include <stdint.h>
#include <cstdint>
#include <vector>
#include <string>

typedef uint8_t byte;
typedef std::vector<byte*> byte_vpt;
typedef unsigned int uint;
typedef std::vector<int> int_vt;
typedef std::vector<uint> uint_vt;
typedef std::vector<uint_vt> uint_vvt;
typedef std::vector<float> float_vt;
typedef std::vector<double> double_vt;
typedef std::vector<std::string> string_vt;
typedef std::vector<string_vt> string_vvt;

const double MS = (1000.0L);
const double US = (1000.0L * 1000.0L);
const double NS = (1000.0L * 1000.0L * 1000.0L);

inline double secToMilliSec(const double aTime) 
{
	return aTime * MS;
}

inline double secToMilliSec(const double aTime, const double aNumber) 
{
	return secToMilliSec(aTime) / aNumber;
}

inline double secToMicroSec(const double aTime) 
{
	return aTime * US;
}

inline double secToMicroSec(const double aTime, const double aNumber) 
{
	return secToMicroSec(aTime) / aNumber;
}

inline double secToNanoSec(const double aTime) 
{
	return aTime * NS;
}

inline double secToNanoSec(const double aTime, const double aNumber) 
{
	return secToNanoSec(aTime) / aNumber;
}

/**
 * This enum is used to describe attribute types (e.g., in the logical schema of a relation)
 */
enum SchemaTypes_et
{
	kNoType = 0,
	kCHAR = 1,			//1B
	kUINT32 = 2,		//4B
	kUINT64 = 3,		//8B
	kINT32 = 4,			//4B
	kFLOAT32 = 5,		//4B
	kFLOAT64 = 6,		//8B
	kCHAR_STRING = 7,	//8B
	kDATEJD = 8,		//4B
	kSTR_SDICT = 9,		//4B
	kEndType = 10
};
typedef std::vector<SchemaTypes_et> schema_vt;

/**
 * This enum is used to describe container types. Container are used in relations in order to store complex data like strings.
 */
enum ContainerTypes_et
{
	kNoContainer = -1,
	kSimpleCharContainer = 0,
	kSimpleDictionary = 1,
	kSimpleOrderedDictionary = 2
};
typedef std::vector<ContainerTypes_et> container_vt;

/**
 * Union data type used to store multiple data types
 */
union unval_t
{
	const char* _cp64;	//Character Pointer (C-String)
	double _f64;		//Double
	uint64_t _u64;		//64Bit Unsigned Integer 
	float _f32;			//Float
	uint32_t _u32;		//32Bit Unsigned Integer
	uint32_t _d32;		//DateJD
	uint32_t _s32;		//String Dictionary
	int32_t _i32;		//32Bit Signed Integer
	char _c8;			//Char

	char c8(){return _c8;}
	uint32_t u32(){return _u32;}
	uint64_t u64(){return _u64;}
	int32_t i32(){return _i32;}
	float f32(){return _f32;}
	double f64(){return _f64;}
	const char* cp64(){return _cp64;}
	uint32_t d32(){return _d32;}
	uint32_t s32(){return _s32;}

	size_t _size;
	byte* _pointer;
	uint32_t _tid[2];
	unval_t* _unval_pt;
};
typedef unval_t* unval_pt;
typedef std::vector<unval_t> unval_vt;
typedef std::vector<unval_t*> unval_vpt;

#endif
