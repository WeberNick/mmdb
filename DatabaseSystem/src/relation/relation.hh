/**
 *	@file 	relation.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Abstract base class for a relation
 *	@bugs 	Currently no bugs known
 *	@todos	Currently no todos
 *
 *	@section DESCRIPTION
 *	This abstract class serves as base for a relation. All relations have several general variables and functions, independent of their specific storage layout.
 *	The storage layout independent part of a relation is defined in this class.
*/
#ifndef RELATION_HH
#define RELATION_HH

#include "relation_infra.hh"
#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/webe/print_helper.hh"
#include "infra/moer/simplecharcontainer.hh"
#include "infra/moer/simpledictionary.hh"
#include "memory/segment.hh"

#include <string>
#include <vector>


/**
 * 	This class defines all the variables and functions a relation must provide.
 *	A sub classes which is derived from this class, needs to implement all respective storage layout dependent parts
 */
class Relation
{
	protected:
		/* standard constructor */
		Relation();
		/* constructor */
		Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer);
		/* copy constructor */
		Relation(const Relation& aRelation);

	public:
		/* destructs a relation */
		virtual ~Relation();
		/* specifies the assign operator of a relation */
		Relation& operator= (const Relation& aRelation);

	public:
		/**
		 * @brief	Access routine to the relations number of attributes
		 * 
		 * @return 	the number of attributes the relation has 
		 */
		inline const uint getNoAttributes() const;

		/**
		 * @brief	Access routine to the relations logical tuple size
		 * 
		 * @return 	the logical size of a tuple (in bytes)
		 */
		inline const uint getLogTupleSize() const;

		/**
		 * @brief	Access routine to the relations logical schema
		 * 
		 * @return 	a reference to the relations logical schema
		 */
		inline const schema_vt& getLogSchema() const;

		/**
		 * @brief	Access routine to the relations attribute description
		 * 
		 * @return 	a reference to the relations attribute descriptions
		 */
		inline const attr_desc_vt& getAttrDesc() const;

		/**
		 * @brief	Access routine to the relations container description
		 * 
		 * @return 	a reference to the relations container descriptions
		 */
		inline const cont_desc_vt& getContDesc() const;

		/**
		 * @brief	Access routine to get the container type of the given attribute
		 * 
		 * @param 	aAttributeNo  the attributes number/index for which the container type should be returned  
		 * @return 	an enum representation of the container type used by the attribute specified in the parameter
		 */
		inline const ContainerTypes_et getContainerType(uint aAttributeNo) const;

		/**
		 * @brief	Access routine to get the container index of the given attribute
		 * 
		 * @param 	aAttributeNo  the attributes number/index for which the container index should be returned  
		 * @return 	index of the container used by the attribute specified in the parameter
		 */
		inline const int getContainerIndex(uint aAttributeNo) const;

		/**
		 * @brief	Access routine to the relations container addresses
		 * 
		 * @return 	a reference to the vector storing the addresses of the container
		 */
		inline const byte_vpt& getContainer() const;

		/**
		 * @brief	Access routine to the relations segment
		 * 
		 * @return 	a reference to the segment the relation is associated with
		 */
		inline Segment& getSegment();

		/**
		 * @brief	Pure virtual function, needs to be implemended in derived classes. Purpose of this function is to return storage layout dependent information
		 *
		 * @return 	a reference to the relations storage layout dependent data
		 */
		virtual const uint_vt& getPhysLayoutData() const = 0;

		/*prints the relation*/
		void print();

	protected:
		/**
		 *	@brief	this function can be called to initialize a relation with the passed parameter
		 *
		 *	@param 	aRelName  a relation name to initialize the relation with
		 *	@param 	aAttrDescVec  a attribute description vector to initialize the relation with
		 *	@param 	aLogSchema  a logical schema to initialize the relation with
		 *	@param 	aContDescVec  a container description vector to initialize the relation with
		 *	@param 	aSegment  a segment to initialize the relation with
		 *	@param 	aDictEntryPointer  a vector with pointer to the dictionary entries to initialize the relation with
		 *	@see 	relation_infra.hh
		 *	@see 	infra/moer/simpledictionary.hh
		 */
		void init(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer);
		void init(const Relation& aRelation);
		
	private:
		/**
		 *	@brief	allocates memory for the relation's container
		 *
		 *	@param 	aContDescVec  a vector containing the container decription
		 *	@param 	aDictEntryPointer  a vector containing pointer to the dictionary entries
		 */
		void alloc(const cont_desc_vt& aContDescVec, const byte_vpt& aDictEntryPointer);

		/**
		 *	@brief	calculates the logical tuple size of the logical schema
		 *
		 *	@param 	aLogSchema  the representation of the logical schema (vector with enum entries)
		 *	@return the logical size of a tuple
		 */
		const uint calcTupleSize(const schema_vt& aLogSchema);

	private:
		/* the relation name */
		std::string 	_relName;
		/* the number of attributes the relation has */
		uint 			_noAttributes;
		/* the logical size of the relation's tuple */
		uint 			_logTupleSize;
		/* an attribute description vector */
		attr_desc_vt 	_attrDescVec;
		/* the logical schema of the relation, represented by enums */
		schema_vt 		_logSchemaVec;
		/* a container description vector */
		cont_desc_vt 	_contDescVec;
		/* the relation's associated segment */
		Segment 		_segment;
		/* vector with pointer to the respective container */
		byte_vpt 		_container;
};

const uint Relation::getNoAttributes() const
{
	return _noAttributes;
}

const uint Relation::getLogTupleSize() const
{
	return _logTupleSize;
}

const schema_vt& Relation::getLogSchema() const
{
	return _logSchemaVec;
}

const attr_desc_vt& Relation::getAttrDesc() const
{
	return _attrDescVec;
}

const cont_desc_vt& Relation::getContDesc() const
{
	return _contDescVec;
}

const ContainerTypes_et Relation::getContainerType(uint aAttributeNo) const
{
	return _attrDescVec[aAttributeNo]._containerType;
}

const int Relation::getContainerIndex(uint aAttributeNo) const
{
	return _attrDescVec[aAttributeNo]._containerIndex;
}

const byte_vpt& Relation::getContainer() const
{
	return _container;
}

Segment& Relation::getSegment()
{
	return _segment;
}


#endif