/**
 *	@file 	nsm_relation.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	This class implements NSM specific features
 *	@bugs 	Currently no bugs known
 *	@todos	Delete function printNSM if not needed for testing anymore
 *
 *	@section DESCRIPTION
 *	This class is derived of the abstract 'Relation'-class and adds NSM specific features, most importantly the access to the attribute's offsets
*/
#ifndef NSM_RELATION_HH
#define NSM_RELATION_HH

#include "relation.hh"

class NSM_Relation : public Relation
{
	public:
		/* standard constructor */
		NSM_Relation();
		/* constructor */
		NSM_Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer);
		/* copy constructor */
		NSM_Relation(const NSM_Relation& aRelation);
		/* destructor */
		~NSM_Relation();
		/* specifies the assign operator of a nsm relation */
		NSM_Relation& operator= (const NSM_Relation& aRelation);

	public:
		/**
		 * @brief	This method is called to access the NSM specific physical layout data
		 *
		 * @return 	a reference to the relations offset vector
		 */
		inline const uint_vt& getPhysLayoutData() const;
		
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
		void initRelation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer);
		void initRelation(const NSM_Relation& aRelation);

	private:
		/**
		 *	@brief	In order to prevent waste of memory through memory alignment, all attributes must be stored in the order of descending attribute size.
		 *			To achieve this, the offset at which each attribute must be stored (respective to the record pointer) must be calculated.
		 */
		void calcPhysSchemaAndOffset();

	private:
		/* a vector with offsets for each attribute */
		uint_vt _offset;




	public:
		void printNSM(unval_vt& aTupel, const size_t aSize, const size_t aIndex)
		{
			print();
			byte* lAttrPointer;
			for(size_t i = 0; i < aSize; ++i)
			{
				std::cout << "Tupel " << (i+1) << "/" << aSize << " :  ";
				for(size_t j = 0; j < getNoAttributes(); ++j)
				{
					lAttrPointer = aTupel[aIndex]._unval_pt[i]._pointer + getPhysLayoutData()[j];
					std::cout << "Attr(" << j << ")Value : ";
					switch(getAttrDesc()[j].attrType())
					{
						case kCHAR:
							std::cout << *(char*)lAttrPointer << " ,  ";
							break;
						case kUINT32:
							std::cout << *(uint32_t*)lAttrPointer << " ,  ";
							break;
						case kUINT64:
							std::cout << *(uint64_t*)lAttrPointer << " ,  ";
							break;
						case kINT32:
							std::cout << *(int32_t*)lAttrPointer << " ,  ";
							break;
						case kFLOAT32:
							std::cout << *(float*)lAttrPointer << " ,  ";
							break;
						case kFLOAT64:
							std::cout << *(double*)lAttrPointer << " ,  ";
							break;
						case kCHAR_STRING:
							std::cout << *(const char**)lAttrPointer << " ,  ";
							break;
						case kDATEJD:
							std::cout << *(uint32_t*)lAttrPointer << " ,  ";
							break;
						case kSTR_SDICT:
							std::cout << *(uint32_t*)lAttrPointer << " ,  ";
							break;
						default:
							throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
							break;
					}
				}
				std::cout << "\n" << std::endl;
			}
		}

};

const uint_vt& NSM_Relation::getPhysLayoutData() const
{
	return _offset;
}

#endif