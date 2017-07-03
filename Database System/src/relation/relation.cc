#include "relation.hh"

Relation::Relation() :
	_relName(),
	_attrDescVec(),
	_logSchemaVec(),
	_contDescVec(),
	_segment(),
	_noAttributes(0),
	_logTupleSize(0),
	_container()
{}

Relation::Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer) :
	_relName(aRelName),
	_attrDescVec(aAttrDesc),
	_logSchemaVec(aLogSchema),
	_contDescVec(aContDesc),
	_segment(aSegment),
	_noAttributes(_logSchemaVec.size()),
	_logTupleSize(0),
	_container()
{
	_logTupleSize = calcTupleSize(_logSchemaVec);
	alloc(_contDescVec, aDictEntryPointer);
}

Relation::Relation(const Relation& aRelation) :
	_relName(aRelation._relName),
	_attrDescVec(aRelation._attrDescVec),
	_logSchemaVec(aRelation._logSchemaVec),
	_contDescVec(aRelation._contDescVec),
	_segment(aRelation._segment),
	_noAttributes(_logSchemaVec.size()),
	_logTupleSize(aRelation._logTupleSize),
	_container()
{}

Relation::~Relation()
{
	for(uint i = 0; i < _container.size(); ++i)
	{
		delete _container[i];
	}
}

void Relation::initRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	initLogSchema(aRelName, aAttrDesc, aLogSchema);
	initPhysSchema(aSegment, aContDesc);
	alloc(aContDesc, aDictEntryPointer);
}

void Relation::print(std::string aHeaderMessage)
{
	print_header(std::cout, aHeaderMessage);
	std::cout << "<<Number of Attributes: " << _noAttributes << "  ||  Logical Tuple Size: " << _logTupleSize << ">>" << std::endl;
	std::cout << "Attribute Name || Attribute Type || Schema Index || Attr. Container Type || Container Index" << std::endl;
	for(uint i = 0; i < _attrDescVec.size(); ++i)
	{
		std::cout << std::setw(18) << std::left << std::setfill(' ')
			<< _attrDescVec[i]._attrName
			<< std::setw(18) << std::left << std::setfill(' ')
			<< getEnumAsString(_attrDescVec[i]._attrType , true)
			<< std::setw(16) << std::left << std::setfill(' ')
			<< _attrDescVec[i]._logSchemaIndex 
			<< std::setw(24) << std::left << std::setfill(' ')
			<< getEnumAsString(_attrDescVec[i]._containerType  , false)
			<< _attrDescVec[i]._containerIndex
			<< std::endl;
	}
}

void Relation::initLogSchema(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema)
{
	_relName = aRelName;
	_attrDescVec = aAttrDesc;
	_logSchemaVec = aLogSchema;
	_noAttributes = _logSchemaVec.size();
	_logTupleSize = calcTupleSize(_logSchemaVec);
}

void Relation::initPhysSchema(Segment aSegment, const cont_desc_vt& aContDesc)
{
	_contDescVec = aContDesc;
	_segment = aSegment;
}

void Relation::alloc(const cont_desc_vt& aContDesc, const byte_vpt& aDictEntryPointer)
{
	for(size_t i = 0, j = 0; i < aContDesc.size(); ++i)
	{
		switch(aContDesc[i].type())
		{
			case kNoContainer: //-1
				break;
			case kSimpleCharContainer: //0
				try
				{
					_container.push_back((byte*)new SimpleCharContainer(aContDesc[i].helper()));
				}
				catch(std::bad_alloc& ba)
				{
					std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
					exit(1);
				}
				break;
			case kSimpleDictionary: //1
				switch(_logSchemaVec[aContDesc[i].helper()])
				{
				 	case kSTR_SDICT:
				 		try
				 		{
					 		_container.push_back((byte*)new SimpleDictionary<std::string>());
				 		}
				 		catch(std::bad_alloc& ba)
				 		{
				 			std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
				 			exit(1);
				 		}
						if(j < aDictEntryPointer.size())
						{
							for(size_t k = 0; k < ((string_vt*)aDictEntryPointer[j])->size(); ++k)
							{
								((SimpleDictionary<std::string>*)_container[i])->insert((*(string_vt*)aDictEntryPointer[j])[k]);
							}
							++j;
						}
				 		break;
				 	default:
				 		throw BaseException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "Dictionary type is currently not supported");
						break;
				}
				break;
			case kSimpleOrderedDictionary: //2
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}

}

const uint Relation::calcTupleSize(const schema_vt& aLogSchema)
{
	uint lTupleSize = 0;
	for(uint i = 0; i < aLogSchema.size(); ++i)
	{
		switch(aLogSchema[i])
		{
			case kCHAR:
				lTupleSize += 1;
				break;
			case kUINT32:
			case kINT32:
			case kFLOAT32:
			case kDATEJD:
			case kSTR_SDICT:
				lTupleSize += 4;
				break;
			case kUINT64:
			case kFLOAT64:
			case kCHAR_STRING:
				lTupleSize += 8;
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	return lTupleSize;
}



