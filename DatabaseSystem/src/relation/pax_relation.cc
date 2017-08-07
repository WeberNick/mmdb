#include "pax_relation.hh"

PAX_Relation::PAX_Relation() :
	Relation(), _partitionData()
{}

PAX_Relation::PAX_Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer) :
	Relation(aRelName, aAttrDescVec, aLogSchema, aContDescVec, aSegment, aDictEntryPointer), _partitionData(getNoAttributes() + 1, 0)
{
	calcPartitionData();
}

PAX_Relation::PAX_Relation(const PAX_Relation& aRelation) :
	Relation(aRelation), _partitionData(getNoAttributes() + 1, 0)
{
	calcPartitionData();
}

PAX_Relation::~PAX_Relation()
{}

PAX_Relation& PAX_Relation::operator=(const PAX_Relation& aRelation)
{
	if(this != &aRelation)
	{
		initRelation(aRelation);
		_partitionData = getPhysLayoutData();
	}
	return *this;
}

void PAX_Relation::initRelation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	init(aRelName, aAttrDescVec, aLogSchema, aContDescVec, aSegment, aDictEntryPointer);
	_partitionData.resize(getNoAttributes() + 1, 0);
	calcPartitionData();
}

void PAX_Relation::initRelation(const PAX_Relation& aRelation)
{
	init(aRelation);
	calcPartitionData();
}

void PAX_Relation::calcPartitionData()
{
	uint lTotalAttrSize = 0;
	for(uint i = 0; i < getLogSchema().size(); ++i)
	{
		switch(getLogSchema()[i])
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

