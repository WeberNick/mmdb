#include "nsm_relation.hh"

NSM_Relation::NSM_Relation() :
	Relation(),
	_offset()
{}

NSM_Relation::NSM_Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer) :
	Relation(aRelName, aAttrDescVec, aLogSchema, aContDescVec, aSegment, aDictEntryPointer),
	_offset()
{
	calcPhysSchemaAndOffset();
}

NSM_Relation::NSM_Relation(const NSM_Relation& aRelation) :
	Relation(aRelation),
	_offset(aRelation._offset)
{}

NSM_Relation::~NSM_Relation()
{}

NSM_Relation& NSM_Relation::operator=(const NSM_Relation& aRelation)
{
	if(this != &aRelation)
	{
		initRelation(aRelation);
		_offset = getPhysLayoutData();
	}
	return *this;
}

void NSM_Relation::initRelation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer)
{
	init(aRelName, aAttrDescVec, aLogSchema, aContDescVec, aSegment, aDictEntryPointer);
	calcPhysSchemaAndOffset();
}

void NSM_Relation::initRelation(const NSM_Relation& aRelation)
{
	init(aRelation);
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
		for(uint j = 0; j < getLogSchema().size(); ++j)
		{
			if((getLogSchema()[j] == kUINT64 || getLogSchema()[j] == kFLOAT64 || getLogSchema()[j] == kCHAR_STRING) && i == 8)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if((getLogSchema()[j] == kUINT32 || getLogSchema()[j] == kINT32 || getLogSchema()[j] == kFLOAT32 || getLogSchema()[j] == kDATEJD || getLogSchema()[j] == kSTR_SDICT) && i == 4)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if(getLogSchema()[j] == kCHAR && i == 1)
			{
				lPhysSchema.push_back(j);
				lPhysOffset.push_back(lPhysOffset[index] + i);
				++index;
			}
			else if(getLogSchema()[j] <= kNoType || getLogSchema()[j] >= kEndType)
			{
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
			}
		}
	}
	for(uint i = 0; i < getLogSchema().size(); ++i)
	{
		uint physSchemaIndex = 0;
		while(i != lPhysSchema[physSchemaIndex])
		{
			++physSchemaIndex;
		}
		_offset.push_back(lPhysOffset[physSchemaIndex]);
	}
}