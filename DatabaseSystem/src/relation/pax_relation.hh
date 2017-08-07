/**
 *	@file 	pax_relation.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	This class implements PAX specific features
 *	@bugs 	Currently no bugs known
 *	@todos	Delete function printPAX if not needed for testing anymore
 *
 *	@section DESCRIPTION
 *	This class is derived of the abstract 'Relation'-class and adds PAX specific features, most importantly the access to page partition information
*/
#ifndef PAX_RELATION_HH
#define PAX_RELATION_HH

#include "relation.hh"

#include "pax/page_interpreter_pax.hh"


class PAX_Relation : public Relation
{
	public:
		/* standard constructor */
		PAX_Relation();
		/* constructor */
		PAX_Relation(const std::string aRelName, const attr_desc_vt& aAttrDescVec, const schema_vt& aLogSchema, const cont_desc_vt& aContDescVec, Segment aSegment, byte_vpt& aDictEntryPointer);
		/* copy constructor */
		PAX_Relation(const PAX_Relation& aRelation);
		/* destructor */
		~PAX_Relation();
		/* specifies the assign operator of a nsm relation */
		PAX_Relation& operator= (const PAX_Relation& aRelation);

	public:
		/**
		 * @brief	This method is called to access the PAX specific physical layout data
		 *
		 * @return 	a reference to the relations attribute sizes
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
		void initRelation(const PAX_Relation& aRelation);

	private:
		/*fills the vector passed as parameter with the information needed to partition the pax page*/
		/**
		 *
		 */
		void calcPartitionData();

	private:
		/**/
		uint_vt _partitionData;



	public:
		void printPAX(unval_vt& aTupel, const size_t aSize, const size_t aIndex)
		{
			print();
			PageInterpreterPAX lPI;
			byte* lAttrPointer;
			uint pageNo;
			uint recNo;

			for(size_t i = 0; i < aSize; ++i)
			{
				std::cout << "Tupel " << (i+1) << "/" << aSize << " :  ";
				pageNo 	= aTupel[aIndex]._unval_pt[i]._tid[0];
				recNo	= aTupel[aIndex]._unval_pt[i]._tid[1];
				lPI.attach(getSegment().getPage(pageNo));

				for(size_t j = 0; j < getNoAttributes(); ++j)
				{
					lAttrPointer = lPI.getMiniPagePointer(j) + (recNo * getPhysLayoutData()[j]);
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

const uint_vt& PAX_Relation::getPhysLayoutData() const
{
	return _partitionData;
}

#endif