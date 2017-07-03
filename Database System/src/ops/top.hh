#ifndef TOP_HH
#define TOP_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include <iostream>

template<bool print>
class Top
{
	public:
		Top();

	public:
		void init(unval_vt& aTupel);
		void step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void finish(unval_vt& aTupel);

		void printProjection(unval_vt& aTupel, Relation& aRelation, const size_t aSize, const uint_vt& aAttrNoList);

	private:
		size_t _indexNo;
};

template<bool print>
Top<print>::Top()
	:   _indexNo()
{}

template<bool print>
void
Top<print>::init(unval_vt& aTupel) 
{
	_indexNo = aTupel.size() - 1;
}

template<bool print>
void
Top<print>::printProjection(unval_vt& aTupel, Relation& aRelation, const size_t aSize, const uint_vt& aAttrNoList) 
{
	for(size_t i = 0; i < aTupel[_indexNo]._unval_pt[aSize]._size; ++i)
	{
		aRelation.printProjection(aTupel[_indexNo]._unval_pt[i]._unval_pt, aAttrNoList);
	}
}

template<bool print>
void
Top<print>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	if(print)
	{
		aRelation.print(aTupel[_indexNo]._unval_pt, std::cout, aSize);
	}
}

template<bool print>
void
Top<print>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	if(print)
	{
		aRelation.print(aTupel[_indexNo]._unval_pt, std::cout, aSize);
	}
}

template<bool print>
void
Top<print>::finish(unval_vt& aTupel) 
{
	std::cout << "FIN" << std::endl;
}


#endif