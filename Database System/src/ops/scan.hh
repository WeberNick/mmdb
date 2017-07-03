#ifndef SCAN_HH
#define SCAN_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>

template<typename T_Consumer, typename T_Relation>
class Scan
{
	public:
		Scan(T_Consumer* aConsumer, T_Relation& aRelation);
		Scan(T_Consumer* aConsumer, T_Relation& aRelation, const size_t aVectorizedSize);

	public:
		void run();

	private:
		void init();
		void finish();
		void startScan(NSM_Relation& aRelation);
		void startScan(PAX_Relation& aRelation);

	private:
		T_Consumer* _nextOp;
		T_Relation& _relation;
		const size_t _vectorizedSize;
		unval_vt _output;
		size_t _indexNo;

};

template<typename T_Consumer, typename T_Relation>
Scan<T_Consumer, T_Relation>::Scan(
									T_Consumer* aConsOp,
									T_Relation& aRelation)
	: _nextOp(aConsOp), _relation(aRelation), _vectorizedSize(1), _output(), _indexNo()
{}

template<typename T_Consumer, typename T_Relation>
Scan<T_Consumer, T_Relation>::Scan(
									T_Consumer* aConsOp,
									T_Relation& aRelation,
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relation(aRelation), _vectorizedSize(aVectorizedSize), _output(), _indexNo()
{}

template<typename T_Consumer, typename T_Relation>
void Scan<T_Consumer, T_Relation>::run()
{
	init();
	startScan(_relation);
	finish();
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::init() 
{
	_indexNo = _output.size();
	_output.push_back(unval_t());
	_output[_indexNo]._unval_pt = new unval_t[_vectorizedSize + 1];
	_output[_indexNo]._unval_pt[_vectorizedSize]._size = _vectorizedSize;
	_nextOp->init(_output);
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::finish() 
{
	_nextOp->finish(_output);
	delete[] _output[_indexNo]._unval_pt;
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(NSM_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterSP lPageInterpreter;
	while(lPageNo < aRelation.getSegment().getNoPages())		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		while(lRecordNo < lPageInterpreter.noRecords())			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				_output[_indexNo]._unval_pt[lCounter]._pointer = lPageInterpreter.getRecord(lRecordNo);
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_output, aRelation, _vectorizedSize, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_output[_indexNo]._unval_pt[_vectorizedSize]._size = lCounter;
			_nextOp->step(_output, aRelation, _vectorizedSize, true);
		}
		lRecordNo = 0;
	}
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(PAX_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterPAX lPageInterpreter;
	while(lPageNo < aRelation.getSegment().getNoPages())		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		while(lRecordNo < lPageInterpreter.noRecords())			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				_output[_indexNo]._unval_pt[lCounter]._tid[0] = lPageNo;
				_output[_indexNo]._unval_pt[lCounter]._tid[1] = lRecordNo;
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_output, aRelation, _vectorizedSize, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_output[_indexNo]._unval_pt[_vectorizedSize]._size = lCounter;
			_nextOp->step(_output, aRelation, _vectorizedSize, true);
		}
		lRecordNo = 0;
	}
}


#endif