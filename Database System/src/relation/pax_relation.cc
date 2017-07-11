#include "pax_relation.hh"

PAX_Relation::PAX_Relation() :
	Relation(), _partitionData()
{}

PAX_Relation::PAX_Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer) :
	Relation(aRelName, aAttrDesc, aLogSchema, aContDesc, aSegment, aDictEntryPointer), _partitionData(_noAttributes + 1, 0)
{
	setPartitionData();
}

PAX_Relation::PAX_Relation(const PAX_Relation& aRelation) :
	Relation(aRelation), _partitionData(_noAttributes + 1, 0)
{
	setPartitionData();
}

PAX_Relation::~PAX_Relation()
{
}

void PAX_Relation::setPartitionData()
{
	uint lTotalAttrSize = 0;
	for(uint i = 0; i < _logSchemaVec.size(); ++i)
	{
		switch(_logSchemaVec[i])
		{
			case kCHAR:
				_partitionData[i] = 1;
				lTotalAttrSize += 1;
				break;
			case kUINT32:
			case kINT32:
			case kFLOAT32:
			case kDATEJD:
			case kSTR_SDICT:
				_partitionData[i] = 4;
				lTotalAttrSize += 4;
				break;
			case kUINT64:
			case kFLOAT64:
			case kCHAR_STRING:
				_partitionData[i] = 8;
				lTotalAttrSize += 8;
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	_partitionData[_partitionData.size() - 1] = lTotalAttrSize;
}

void PAX_Relation::initSubRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	initRelation(aRelName, aAttrDesc, aLogSchema, aContDesc, aSegment, aDictEntryPointer);
	_partitionData.resize(_noAttributes + 1, 0);
	setPartitionData();
}

void PAX_Relation::printPAX(uint aNoPages, uint aMod)
{
	PageInterpreterPAX pi;
	DateJd jd;
	uint lPages = ( aNoPages < this->getSegment().getNoPages() ? aNoPages : this->getSegment().getNoPages() );
	byte* lTempPointer;
	for(uint i = 0; i < lPages; ++i)
	{
		pi.attach(this->getSegment().getPage(i));
		print_header(std::cout, "PAX Page" + std::to_string(i+1) + " (" + _relName + ")");
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
					lTempPointer = pi.pagePtr() + pi.getMiniPageOffset(k) + (j * pi.getAttrSize(k));
					std::cout << std::left << std::setw(18) << _attrDescVec[k]._attrName + " :";
					switch(_attrDescVec[k]._attrType)
					{
						case kCHAR:
							std::cout << *(char*)lTempPointer << std::endl;
							break;
						case kUINT32:
							std::cout << *(uint32_t*)lTempPointer << std::endl;
							break;
						case kSTR_SDICT:
							std::cout << ((SimpleDictionary<std::string>*)_container[_attrDescVec[k]._containerIndex])->get_content(*(uint32_t*)lTempPointer) << std::endl;
							break;
						case kUINT64:
							std::cout << *(uint64_t*)lTempPointer << std::endl;
							break;
						case kINT32:
							std::cout << *(int32_t*)lTempPointer << std::endl;
							break;
						case kFLOAT32:
							std::cout << *(float*)lTempPointer << std::endl;
							break;
						case kFLOAT64:
							std::cout << *(double*)lTempPointer << std::endl;
							break;
						case kCHAR_STRING:
							std::cout << *(const char**)lTempPointer << std::endl;
							break;
						case kDATEJD:
							jd.set(*(uint32_t*)lTempPointer, true);
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

void PAX_Relation::print(const unval_t* aTupel, std::ostream& os, const size_t aSize)
{
	PageInterpreterPAX pi;
	DateJd jd;
	byte* lTempPointer;
	uint32_t lPageNo = aTupel[0]._tid[0];
	uint32_t lRecNo = aTupel[0]._tid[1];
	pi.attach(this->getSegment().getPage(lPageNo));
	for(size_t i = 0; i < aTupel[aSize]._size; ++i)
	{
		lPageNo = aTupel[i]._tid[0];
		lRecNo = aTupel[i]._tid[1];
		for(size_t k = 0; k < _noAttributes; ++k)
		{
			os << _attrDescVec[k]._attrName << " :  ";
			lTempPointer = pi.pagePtr() + pi.getMiniPageOffset(k) + (lRecNo * pi.getAttrSize(k));
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
