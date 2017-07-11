#ifndef PAX_RELATION_HH
#define PAX_RELATION_HH

#include "relation.hh"
#include "pax/page_interpreter_pax.hh"
#include "infra/moer/datejd.hh"


class PAX_Relation : public Relation
{
	public:		
		inline const uint_vt& getPartitionData() const;
		/*initialize the relation*/
		void initSubRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		void printPAX(uint aNoPages, uint aMod);
		void print(const unval_t* aTupel, std::ostream& os, const size_t aSize);
	
	public:
		PAX_Relation();
		PAX_Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		PAX_Relation(const PAX_Relation& aRelation);
		~PAX_Relation();

	private:
		/*fills the vector passed as parameter with the information needed to partition the pax page*/
		void setPartitionData();

	private:
		uint_vt _partitionData;
};

const uint_vt& PAX_Relation::getPartitionData() const
{
	return _partitionData;
}

#endif