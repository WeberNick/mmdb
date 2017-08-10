/**
 *	@file 	top.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Top operator of the physical algebra. This operator marks the end of the operator tree
 *	@bugs 	Currently no bugs known
 *	@todos	PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION
 *	TODO
 *
 */
#ifndef TOP_HH
#define TOP_HH

#include "infra/webe/types.hh"
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

	private:
		size_t _indexNo;
		size_t _tplCount;
		unval_pt _input;
};

template<bool print>
Top<print>::Top()
	:   _indexNo(), _tplCount(), _input()
{}

template<bool print>
void
Top<print>::init(unval_vt& aTupel) 
{
	_indexNo = aTupel.size() - 1;
	_input = aTupel[_indexNo]._unval_pt;
}

template<bool print>
void
Top<print>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	_tplCount += aSize;
	if(print)
	{
		aRelation.printNSM(aTupel, aSize, _indexNo);
	}
}

template<bool print>
void
Top<print>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	_tplCount += aSize;
	if(print)
	{
		aRelation.printPAX(aTupel, aSize, _indexNo);
	}
}

template<bool print>
void
Top<print>::finish(unval_vt& aTupel) 
{
	std::cout << "(FIN) Tuple Count: " << _tplCount << std::endl;
}


#endif