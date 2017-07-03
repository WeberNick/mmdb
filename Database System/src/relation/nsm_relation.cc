#include "nsm_relation.hh"

NSM_Relation::NSM_Relation() :
	Relation(),
	_offset()
{}

NSM_Relation::NSM_Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer) :
	Relation(aRelName, aAttrDesc, aLogSchema, aContDesc, aSegment, aDictEntryPointer),
	_offset()
{
	calcPhysSchemaAndOffset();
}

NSM_Relation::NSM_Relation(const NSM_Relation& aRelation) :
	Relation(aRelation),
	_offset(aRelation._offset)
{}

NSM_Relation::~NSM_Relation()
{
}



void NSM_Relation::initSubRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	initRelation(aRelName, aAttrDesc, aLogSchema, aContDesc, aSegment, aDictEntryPointer);
	calcPhysSchemaAndOffset();
}

void NSM_Relation::calcPhysSchemaAndOffset()
{
	uint_vt lPhysOffset;
	uint_vt lPhysSchema;
	lPhysOffset.push_back(0);
	uint index = 0;
	for(uint i = 8; i > 0; i /= 2)
	{
		for(uint j = 0; j < _logSchemaVec.size(); ++j)
		{
			if((_logSchemaVec[j] == kUINT64 || _logSchemaVec[j] == kFLOAT64 || _logSchemaVec[j] == kCHAR_STRING) && i == 8)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if((_logSchemaVec[j] == kUINT32 || _logSchemaVec[j] == kINT32 || _logSchemaVec[j] == kFLOAT32 || _logSchemaVec[j] == kDATEJD || _logSchemaVec[j] == kSTR_SDICT) && i == 4)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if(_logSchemaVec[j] == kCHAR && i == 1)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if(_logSchemaVec[j] <= kNoType || _logSchemaVec[j] >= kEndType)
			{
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			}
		}
	}
	for(uint i = 0; i < _logSchemaVec.size(); ++i)
	{
		uint physSchemaIndex = 0;
		while(i != lPhysSchema[physSchemaIndex])
		{
			++physSchemaIndex;
		}
		_offset.push_back(lPhysOffset[physSchemaIndex]);
	}
}

void NSM_Relation::printNSM(uint aNoPages, uint aMod)
{
	PageInterpreterSP pi;
	DateJd jd;
	uint lPages = ( aNoPages < this->getSegment().getNoPages() ? aNoPages : this->getSegment().getNoPages() );
	for(uint i = 0; i < lPages; ++i)
	{
		pi.attach(this->getSegment().getPage(i));
		print_header(std::cout, "NSM Page" + std::to_string(i+1) + " (" + _relName + ")");
		std::cout << "Free Space: " << pi.freeSpace() << std::endl;
		std::cout << "No Records: " << pi.noRecords() << std::endl;
		print_sep(std::cout);
		for(uint j = 0; j < pi.noRecords(); ++j)
		{
			if(j % aMod == 0)
			{
				std::cout << "<  Entry " << (j+1) << ":  >" << std::endl;
				for (uint k = 0; k < _noAttributes; ++k)
				{
					std::cout << _attrDescVec[k]._attrName << " :  ";

					switch(_attrDescVec[k]._attrType)
					{
						case kCHAR:
							std::cout << *(char*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kUINT32:
							std::cout << *(uint32_t*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kSTR_SDICT:
							std::cout << ((SimpleDictionary<std::string>*)(_container[_attrDescVec[k]._containerIndex]))->get_content(*(uint32_t*)(pi.getRecord(j) + _offset[k])) << std::endl;
							break;
						case kUINT64:
							std::cout << *(uint64_t*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kINT32:
							std::cout << *(int32_t*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kFLOAT32:
							std::cout << *(float*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kFLOAT64:
							std::cout << *(double*)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kCHAR_STRING:
							std::cout << *(const char**)(pi.getRecord(j) + _offset[k]) << std::endl;
							break;
						case kDATEJD:
							jd.set(*(int32_t*)(pi.getRecord(j) + _offset[k]), true);
							std::cout << jd << std::endl;
							break;
						default:
							throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
							break;
					}
				}
				std::cout << std::endl;
			}
		}
	}
}

void NSM_Relation::print(const unval_t* aTupel, std::ostream& os, const size_t aSize)
{
	DateJd jd;
	byte* lTempPointer;
	for(size_t i = 0; i < aTupel[aSize]._size; ++i)
	{
		for(size_t k = 0; k < _noAttributes; ++k)
		{
			os << _attrDescVec[k]._attrName << " :  ";
			lTempPointer = aTupel[i]._pointer + _offset[k];
			switch(_attrDescVec[k]._attrType)
			{
				case kCHAR:
					os << *(char*)lTempPointer << std::endl;
					break;
				case kUINT32:
					os << *(uint32_t*)lTempPointer << std::endl;
					break;
				case kSTR_SDICT:
					os << ((SimpleDictionary<std::string>*)_container[_attrDescVec[k]._containerIndex])->get_content(*(uint32_t*)lTempPointer) << std::endl;
					break;
				case kUINT64:
					os << *(uint64_t*)lTempPointer << std::endl;
					break;
				case kINT32:
					os << *(int32_t*)lTempPointer << std::endl;
					break;
				case kFLOAT32:
					os << *(float*)lTempPointer << std::endl;
					break;
				case kFLOAT64:
					os << *(double*)lTempPointer << std::endl;
					break;
				case kCHAR_STRING:
					os << *(const char**)lTempPointer << std::endl;
					break;
				case kDATEJD:
					jd.set(*(int32_t*)lTempPointer, true);
					os << jd << std::endl;
					break;
				default:
					throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					break;
			}
		}
		os << std::endl;
	}
}