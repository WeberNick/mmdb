#ifndef RELATION_HH
#define RELATION_HH

#include <string>
#include <vector>

#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/print_helper.hh"
#include "infra/moer/simplecharcontainer.hh"
#include "infra/moer/simpledictionary.hh"
#include "memory/segment.hh"

class Relation
{
	public:
		/*struct describing the attributes of the relation*/
		struct attr_desc_t
		{
			std::string _attrName;
			SchemaTypes_et _attrType;
			uint _logSchemaIndex;
			ContainerTypes_et _containerType;
			int _containerIndex;
		};
		typedef std::vector<attr_desc_t> attr_desc_vt;

		/*struct describing the containers of the relation*/
		struct cont_desc_t
		{
			ContainerTypes_et _containerType;
			uint _helper;	//If container type is SimpleCharContainer this variable indicates the chunk size, if container is a dictionary it indicates the attr index
			ContainerTypes_et type() const {return _containerType;}
			uint helper() const {return _helper;}
		};
		typedef std::vector<cont_desc_t> cont_desc_vt;

	public:
		/*return number of attributes the relation has*/
		inline const uint getNoAttributes() const;
		/*return a reference to the relations logical schema*/
		inline const schema_vt& getLogSchema() const;
		/**/
		inline const Relation::attr_desc_vt& getAttrDesc() const;
		/**/
		inline const Relation::cont_desc_vt& getContDesc() const;
		/*return the logical size of a tuple (in bytes)*/
		inline const uint getLogTupleSize() const;
		/*return a reference to the vector storing the addresses of the container*/
		inline const byte_vpt& getContainer() const;
		/*return an int representation of the container type used by the attribute specified in the parameter*/
		inline const ContainerTypes_et getContainerType(uint aAttributeIndex) const;
		/*return index of the container used by the attribute specified in the parameter*/
		inline const int getContainerIndex(uint aAttributeIndex) const;
		/*return a reference to the segment the relation is associated with*/
		inline Segment& getSegment();
		inline void printProjection(unval_t*& aProjection, const uint_vt& aAttrNoList);

	public:
		/*initialize the relation*/
		void initRelation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		/*prints the relation*/
		void print(std::string aHeaderMessage);
	private:
		/*initialize the logical schema*/
		void initLogSchema(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema);
		/*initialize a part of the physical schema*/
		void initPhysSchema(Segment aSegment, const cont_desc_vt& aContDesc);
		/*allocate memory for the container used by the relation*/
		void alloc(const cont_desc_vt& aContDesc, const byte_vpt& aDictEntryPointer);

	public:
		Relation();
		Relation(const std::string aRelName, const attr_desc_vt& aAttrDesc, const schema_vt& aLogSchema, const cont_desc_vt& aContDesc, Segment aSegment, byte_vpt& aDictEntryPointer);
		Relation(const Relation& aRelation);
		~Relation();

	protected:
		/*calculates the logical tuple size*/
		const uint calcTupleSize(const schema_vt& aLogSchema);

	protected:
		std::string _relName;
		attr_desc_vt _attrDescVec;
		schema_vt _logSchemaVec;
		cont_desc_vt _contDescVec;
		Segment _segment;
		uint _noAttributes;
		uint _logTupleSize;
		byte_vpt _container;
};

const uint Relation::getNoAttributes() const
{
	return _noAttributes;
}

const schema_vt& Relation::getLogSchema() const
{
	return _logSchemaVec;
}

const Relation::attr_desc_vt& Relation::getAttrDesc() const
{
	return _attrDescVec;
}

const Relation::cont_desc_vt& Relation::getContDesc() const
{
	return _contDescVec;
}

const uint Relation::getLogTupleSize() const
{
	return _logTupleSize;
}

const byte_vpt& Relation::getContainer() const
{
	return _container;
}

const ContainerTypes_et Relation::getContainerType(uint aAttributeIndex) const
{
	return _attrDescVec[aAttributeIndex]._containerType;
}

const int Relation::getContainerIndex(uint aAttributeIndex) const
{
	return _attrDescVec[aAttributeIndex]._containerIndex;
}

Segment& Relation::getSegment()
{
	return _segment;
}


void Relation::printProjection(unval_t*& aProjection, const uint_vt& aAttrNoList)
{
	for(size_t i = 0; i < aAttrNoList.size(); ++i)
	{
		switch(_logSchemaVec[aAttrNoList[i]])
		{
			case kCHAR:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].c8() << std::endl;
				break;
			case kUINT32:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].u32() << std::endl;
				break;
			case kUINT64:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].u64() << std::endl;
				break;
			case kINT32:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].i32() << std::endl;
				break;
			case kFLOAT32:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].f32() << std::endl;
				break;
			case kFLOAT64:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].f64() << std::endl;
				break;
			case kCHAR_STRING:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].cp64() << std::endl;
				break;
			case kDATEJD:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].d32() << std::endl;
				break;
			case kSTR_SDICT:
				std::cout << "Value of Attribute " << aAttrNoList[i] << ": " << aProjection[i].s32() << std::endl;
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;

		}
	}
}


#endif