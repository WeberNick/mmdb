#include "relation.hh"

Relation::Relation() :
	_relName(),
	_noAttributes(0),
	_logTupleSize(0),
	_attrDescVec(),
	_logSchemaVec(),
	_contDescVec(),
	_segment(),
	_container()
{}

Relation::Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer) :
	_relName(aRelName),
	_noAttributes(_logSchemaVec.size()),
	_logTupleSize(0),
	_attrDescVec(aAttrDescVec),
	_logSchemaVec(aLogSchema),
	_contDescVec(aContDescVec),
	_segment(aSegment),
	_container()
{
	_logTupleSize = calcTupleSize(_logSchemaVec);
	alloc(_contDescVec, aDictEntryPointer);
}

Relation::Relation(const Relation& aRelation) :
	_relName(aRelation._relName),
	_noAttributes(aRelation._noAttributes),
	_logTupleSize(aRelation._logTupleSize),
	_attrDescVec(aRelation._attrDescVec),
	_logSchemaVec(aRelation._logSchemaVec),
	_contDescVec(aRelation._contDescVec),
	_segment(aRelation._segment),
	_container(aRelation._container)
{}

Relation::~Relation()
{
	for(uint i = 0; i < _container.size(); ++i)
	{
		delete _container[i];
	}
}

Relation& Relation::operator=(const Relation& aRelation)
{
	if(this != &aRelation)
	{
		_relName 		= aRelation._relName;
		_noAttributes 	= aRelation._noAttributes;
		_logTupleSize 	= aRelation._logTupleSize;
		_attrDescVec 	= aRelation._attrDescVec;
		_logSchemaVec 	= aRelation._logSchemaVec;
		_contDescVec 	= aRelation._contDescVec;
		_segment 		= aRelation._segment;
		_container 		= aRelation._container;
	}
	return *this;
}

void Relation::init(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	_relName = aRelName;
	_noAttributes = aLogSchema.size();
	_logTupleSize = calcTupleSize(aLogSchema);
	_attrDescVec = aAttrDescVec;
	_logSchemaVec = aLogSchema;
	_contDescVec = aContDescVec;
	_segment = aSegment;
	alloc(_contDescVec, aDictEntryPointer);
}

void Relation::init(const Relation& aRelation)
{
	_relName 		= aRelation._relName;
	_noAttributes 	= aRelation._noAttributes;
	_logTupleSize 	= aRelation._logTupleSize;
	_attrDescVec 	= aRelation._attrDescVec;
	_logSchemaVec 	= aRelation._logSchemaVec;
	_contDescVec 	= aRelation._contDescVec;
	_segment 		= aRelation._segment;
	_container 		= aRelation._container;
}

void Relation::alloc(const cont_desc_vt& aContDescVec, const byte_vpt& aDictEntryPointer)
{
	for(size_t i = 0, j = 0; i < aContDescVec.size(); ++i)
	{
		switch(aContDescVec[i].containerType())
		{
			case kNoContainer: //-1
				break;
			case kSimpleCharContainer: //0
				try
				{
					_container.push_back((byte*)new SimpleCharContainer(aContDescVec[i].helper()));
				}
				catch(std::bad_alloc& ba)
				{
					std::cerr << "bad_alloc caught: " << ba.what() << std::endl;
					exit(1);
				}
				break;
			case kSimpleDictionary: //1
				switch(_logSchemaVec[aContDescVec[i].helper()])
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

void Relation::print()
{
	print_header(std::cout, _relName);
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