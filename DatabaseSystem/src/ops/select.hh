/**
 *	@file 	select.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Select operator of the physical algebra, used to select records based on a predicate
 *	@bugs 	Currently no bugs known
 *	@todos	PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION
 *	This template class implements the physical algebra operator 'Select'.
 *	The select operator can be used to select certain records based on a
 *	predicate evaluation.
 *
 *	The select operator is storage layout independent. The storage layout
 *	dependent part (the predicate evaluation) is not implemented in this
 *	operator but is implemented in a separate predicate class.
 *
 *	General	At the start, the operator is initialized by calling the 'init'
 *			procedure. This allocates memory for the output of the select 
 *			and initializes certain variables neeeded by the select 
 *			operator. As soon as the 'step' procedure is called by a
 *			previous operator, the selection process starts. The select 
 *			operator uses an associated predicate object which implements
 *			the predicate evaluation. For each query with a different 
 *			predicate, the predicate class needs to be implemented. The 
 *			select operator doesn't actually implement the predicate test
 *			but implements the procedure whats happens when a predicate
 *			test resolves to 'true' (i.e., the record(s) is(are) qualified).
 *			If the predicate evaluation resolves to 'true', this means the
 *			output data structure is full and can be passed to the next
 *			operator in the chain. The output data structure can contain
 *			either a single record (tuple at a time) or a user defined
 *			number of records (@see scan.hh for more detailed information).
 *
 */
#ifndef SELECT_HH
#define SELECT_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include <vector>

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
class Select
{
	public:
		/* constructor for the select operator without a vectorized size (size 1 (i.e., no vectorized processing) is assumed) */
		Select(T_Consumer* aConsumer, T_Predicate& aPredicate);
		/* constructor for the select operator with a vectorized size */
		Select(T_Consumer* aConsumer, T_Predicate& aPredicate, const size_t aVectorizedSize);

	public:
		/**
		 *	@brief	This procedure starts the initialization
		 *
		 *	@param	aTuple  The datas structure with the input of the previous operator and output of this operator
		 */
		void init(unval_vt& aTuple);

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to finish the scanning
		 *
		 *	@param	aTuple  The datas structure with the input of the previous operator and output of this operator
		 *	@param	aRelation  The associated relation of the select operator
		 *	@param	aSize  The size of the input (i.e., number of tuples received from the previous operator)
		 *	@param	aNoMoreData  A flag indicating if more data will follow or if this is the last data
		 */
		void step(unval_vt& aTuple, T_Relation& aRelation, const size_t aSize, const bool aNoMoreData);

		/**
		 *	@brief	This procedure is called to finish the selection
		 *
		 *	@param	aTuple  The datas structure with the input of the previous operator and output of this operator
		 */
		void finish(unval_vt& aTuple);

	private:
		/* The associated next operator */
		T_Consumer* _nextOp;
		/* The associated predicate class/object */
		T_Predicate& _pred;
		/* The size for the vectorized processing */
		const size_t _vectorizedSize;
		/* Index to the input data structure */
		size_t _indexNo1;
		/* Pointer to the input data structure */
		unval_pt _input;
		/* Index to the output data structure */
		size_t _indexNo2;
		/* Pointer to the output data structure */
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
Select<T_Consumer, T_Predicate, T_Relation>::init(unval_vt& aTuple) 
{
	_indexNo1 = aTuple.size() - 1;
	_indexNo2 = aTuple.size();
	aTuple.push_back(unval_t());
	aTuple[_indexNo2]._unval_pt = new unval_t[_vectorizedSize];
	_input = aTuple[_indexNo1]._unval_pt;
	_output = aTuple[_indexNo2]._unval_pt;
	_nextOp->init(aTuple);
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
Select<T_Consumer, T_Predicate, T_Relation>::step(unval_vt& aTuple, T_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	//This recursion is not fully tested!! ToDo: think about it or fully test it
	if(_pred(_input, _output, aSize, _vectorizedSize, aRelation))
	{
		_nextOp->step(aTuple, aRelation, _vectorizedSize, aNoMoreData);
		if(_pred.getInputIndex() < aSize)
		{
			step(aTuple, aRelation, aSize, aNoMoreData);
		}
		_pred.setInputIndexZero();
	}
	else if(aNoMoreData && _pred.getOutputIndex() > 0)
	{
		_nextOp->step(aTuple, aRelation, _pred.getOutputIndex(), aNoMoreData);
	}
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
Select<T_Consumer, T_Predicate, T_Relation>::finish(unval_vt& aTuple) 
{
	_nextOp->finish(aTuple);
	delete[] _output;
}



#endif