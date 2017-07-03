#ifndef NSM_RELATION_HH
#define NSM_RELATION_HH

#include "relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "infra/moer/datejd.hh"


class NSM_Relation : public Relation
{
	public:
		/*return a reference to the relations offset*/
		inline const uint_vt& getOffset() const;
		/*initialize the relation*/
		void initSubRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		void printNSM(uint aNoPages, uint aMod);
		void print(const unval_t* aTupel, std::ostream& os, const size_t aSize);

	public:
		NSM_Relation();
		NSM_Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		NSM_Relation(const NSM_Relation& aRelation);
		~NSM_Relation();

	private:
		uint_vt _offset;
		void calcPhysSchemaAndOffset();
};

const uint_vt& NSM_Relation::getOffset() const
{
	return _offset;
}


#endif