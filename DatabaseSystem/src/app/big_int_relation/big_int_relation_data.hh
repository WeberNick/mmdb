#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/moer/measure.hh"
#include "memory/memory_manager.hh"
#include "relation/relation_infra.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include <string>
#include <vector>

template<typename T_Relation>
void initIntegerChunk(T_Relation& aRelation, const size_t aAttrNo)
{
	std::string relName("4BYTE_Chunk");
	schema_vt logSchema(aAttrNo);
	attr_desc_vt attrDesc(aAttrNo);
	for(size_t i = 0; i < aAttrNo; ++i)
	{
		if(i % 3 == 0)
		{
			logSchema[i] = kUINT32;
			attrDesc[i] = {"Test_UINT" + std::to_string(i), kUINT32, static_cast<uint>(i), kNoContainer, -1};
		}
		else if(i % 3 == 1)
		{
			logSchema[i] = kINT32;
			attrDesc[i] = {"Test_INT" + std::to_string(i), kINT32, static_cast<uint>(i), kNoContainer, -1};
		}
		else if(i % 3 == 2)
		{
			logSchema[i] = kFLOAT32;
			attrDesc[i] = {"Test_FLOAT" + std::to_string(i), kFLOAT32, static_cast<uint>(i), kNoContainer, -1};
		}
		
	}
	Segment segment;
	cont_desc_vt contDesc;
	byte_vpt dict_entries_pointer;
	aRelation.initRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
}