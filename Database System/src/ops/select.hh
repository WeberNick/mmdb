#ifndef SELECT_HH
#define SELECT_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include <vector>

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
class Select
{
	public:
		Select(T_Consumer* aConsumer, T_Predicate& aPredicate);
		Select(T_Consumer* aConsumer, T_Predicate& aPredicate, const size_t aVectorizedSize);

	public:
		void init(unval_vt& aTupel);
		void step(unval_vt& aTupel, T_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void finish(unval_vt& aTupel);

	private:
		T_Consumer* _nextOp;
		T_Predicate& _pred;
		const size_t _vectorizedSize;
		size_t _indexNo1;
		unval_pt _input;
		size_t _indexNo2;
		unval_pt _output;

};

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
Select<T_Consumer, T_Predicate, T_Relation>::Select(
									T_Consumer* aConsOp,
									T_Predicate& aPredicate)
	: _nextOp(aConsOp), _pred(aPredicate), _vectorizedSize(1), _indexNo1(), _input(), _indexNo2(), _output()
{}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
Select<T_Consumer, T_Predicate, T_Relation>::Select(
									T_Consumer* aConsOp,
									T_Predicate& aPredicate, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _pred(aPredicate), _vectorizedSize(aVectorizedSize), _indexNo1(), _input(), _indexNo2(), _output()
{}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
Select<T_Consumer, T_Predicate, T_Relation>::init(unval_vt& aTupel) 
{
	_indexNo1 = aTupel.size() - 1;
	_indexNo2 = aTupel.size();
	aTupel.push_back(unval_t());
	aTupel[_indexNo2]._unval_pt = new unval_t[_vectorizedSize];
	_input = aTupel[_indexNo1]._unval_pt;
	_output = aTupel[_indexNo2]._unval_pt;
	_nextOp->init(aTupel);
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
Select<T_Consumer, T_Predicate, T_Relation>::step(unval_vt& aTupel, T_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	//This recursion is not fully tested!! ToDo: think about it or fully test it
	if(_pred(_input, _output, aSize, _vectorizedSize, aRelation))
	{
		_nextOp->step(aTupel, aRelation, _vectorizedSize, aNoMoreData);
		if(_pred.getInputIndex() < aSize)
		{
			step(aTupel, aRelation, aSize, aNoMoreData);
		}
		_pred.setInputIndexZero();
	}
	else if(aNoMoreData && _pred.getOutputIndex() > 0)
	{
		_nextOp->step(aTupel, aRelation, _pred.getOutputIndex(), aNoMoreData);
	}
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
Select<T_Consumer, T_Predicate, T_Relation>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	delete[] _output;
}



#endif