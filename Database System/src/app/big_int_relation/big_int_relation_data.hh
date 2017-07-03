#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/moer/measure.hh"
#include "memory/memory_manager.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include <string>
#include <vector>

template<typename T_Relation>
void initIntegerChunk(T_Relation& aRelation, const size_t aAttrNo)
{
	std::string relName("Int_Chunk");
	schema_vt logSchema(aAttrNo);
	Relation::attr_desc_vt attrDesc(aAttrNo);
	for(size_t i = 0; i < aAttrNo; ++i)
	{
		logSchema[i] = kUINT32;
		attrDesc[i] = {"Test_Int" + std::to_string(i), kUINT32, static_cast<uint>(i), kNoContainer, -1};
	}
	Segment segment;
	Relation::cont_desc_vt contDesc;
	byte_vpt dict_entries_pointer;
	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
}