/**
 *	@file 	scan_select.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Combination of the operators described in scan.hh and select.hh
 *	@bugs 	Currently no bugs known
 *	@todos	Optimization needed (implementation is not ideal atm), PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION (@see scan.hh select.hh)
 */
#ifndef SCAN_SELECT_HH
#define SCAN_SELECT_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
class ScanAndSelect
{
	public:
		/* constructor for the scan operator with a vectorized size */
		ScanAndSelect(T_Consumer* aConsumer, T_Predicate& aPredicate, T_Relation& aRelation, const size_t aVectorizedSize = 1);

	public:
		/**
		 *	@brief	This procedure is called from the outside in order to start the scan operator
		 */
		void run();

	private:
		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the initialization
		 */
		void init();

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the scanning
		 *
		 *	@param 	aRelation  a NSM relation whichs records shall be scanned
		 */
		void startScan(NSM_Relation& aRelation);

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the scanning
		 *
		 *	@param 	aRelation  a PAX relation whichs records shall be scanned
		 */
		void startScan(PAX_Relation& aRelation);

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to finish the scanning
		 */
		void finish();

	private:
		/* The associated next operator */
		T_Consumer* _nextOp;
		/* The associated predicate class/object */
		T_Predicate& _pred;
		/* The associated relation to scan */
		T_Relation& _relation;
		/* The size for the vectorized processing */
		const size_t _vectorizedSize;
		/* The vector containing the output data structure */
		unval_vt _tuple;
		/* Index to the output data structure within the vector '_tuple' */
		size_t _indexNo;
		/* Output data structure of the scan operator */
		unval_pt _output;

};

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::ScanAndSelect(
									T_Consumer* aConsOp,
									T_Predicate& aPredicate, 
									T_Relation& aRelation,
									const size_t aVectorizedSize)
	: _nextOp(aConsOp), _pred(aPredicate), _relation(aRelation), _vectorizedSize(aVectorizedSize), _tuple(), _indexNo(), _output()
{}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::run()
{
	init();
	startScan(_relation);
	finish();
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::init() 
{
	_indexNo = _tuple.size();
	_tuple.push_back(unval_t());
	_tuple[_indexNo]._unval_pt = new unval_t[_vectorizedSize];
	_output = _tuple[_indexNo]._unval_pt;
	_nextOp->init(_tuple);
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::finish() 
{
	_nextOp->finish(_tuple);
	delete[] _output;
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::startScan(NSM_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterSP lPageInterpreter;
	const size_t lNumberOfPages = aRelation.getSegment().getNoPages();
	while(lPageNo < lNumberOfPages)		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		const size_t lNumberOfRecords = lPageInterpreter.noRecords();
		while(lRecordNo < lNumberOfRecords)			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				if(_pred(lPageInterpreter.getRecord(lRecordNo), aRelation))
				{
					_output[lCounter]._pointer = lPageInterpreter.getRecord(lRecordNo);
					++lCounter;
				}
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_tuple, aRelation, lCounter, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_nextOp->step(_tuple, aRelation, lCounter, true);
			lCounter = 0;
		}
		lRecordNo = 0;
	}
}

template<typename T_Consumer, typename T_Predicate, typename T_Relation>
void
ScanAndSelect<T_Consumer, T_Predicate, T_Relation>::startScan(PAX_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterPAX lPageInterpreter;
	const size_t lNumberOfPages = aRelation.getSegment().getNoPages();
	while(lPageNo < lNumberOfPages)		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		const size_t lNumberOfRecords = lPageInterpreter.noRecords();
		while(lRecordNo < lNumberOfRecords)			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				if(_pred(lPageNo, lRecordNo, aRelation))
				{
					_output[lCounter]._tid[0] = lPageNo;
					_output[lCounter]._tid[1] = lRecordNo;
					++lCounter;
				}
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_tuple, aRelation, lCounter, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_nextOp->step(_tuple, aRelation, lCounter, true);
		}
		lRecordNo = 0;
	}
}


#endif