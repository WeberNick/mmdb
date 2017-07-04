#ifndef SCAN_HH
#define SCAN_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include "infra/moer/measure.hh"

#include <vector>

template<typename T_Consumer, typename T_Relation>
class Scan
{
	public:
		Scan(T_Consumer* aConsumer, T_Relation& aRelation);
		Scan(T_Consumer* aConsumer, T_Relation& aRelation, const size_t aVectorizedSize);

	public:
		double run();

	private:
		void init();
		void finish();
		void startScan(NSM_Relation& aRelation);
		void startScan(PAX_Relation& aRelation);

	private:
		T_Consumer* _nextOp;
		T_Relation& _relation;
		const size_t _vectorizedSize;
		unval_vt _tuple;
		size_t _indexNo;
		unval_pt _output;
		double _meas;

};

template<typename T_Consumer, typename T_Relation>
Scan<T_Consumer, T_Relation>::Scan(
									T_Consumer* aConsOp,
									T_Relation& aRelation)
	: _nextOp(aConsOp), _relation(aRelation), _vectorizedSize(1), _tuple(), _indexNo(), _output()
{}

template<typename T_Consumer, typename T_Relation>
Scan<T_Consumer, T_Relation>::Scan(
									T_Consumer* aConsOp,
									T_Relation& aRelation,
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relation(aRelation), _vectorizedSize(aVectorizedSize), _tuple(), _indexNo(), _output()
{}

template<typename T_Consumer, typename T_Relation>
double Scan<T_Consumer, T_Relation>::run()
{
	init();
	startScan(_relation);
	finish();
	return _meas;
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::init() 
{
	_meas = 0;
	_indexNo = _tuple.size();
	_tuple.push_back(unval_t());
	_output = _tuple[_indexNo]._unval_pt;
	_output = new unval_t[_vectorizedSize + 1];
	_output[_vectorizedSize]._size = _vectorizedSize;
	_nextOp->init(_tuple);
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::finish() 
{
	_nextOp->finish(_tuple);
	delete[] _output;
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(NSM_Relation& aRelation)
{
	Measure lMeasure;
	if(MEASURE_GLOBAL)
	{
		lMeasure.start();
	}
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
				_output[lCounter]._pointer = lPageInterpreter.getRecord(lRecordNo);
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				lMeasure.stop();
				if(MEASURE_GLOBAL)
				{
					_meas += (lMeasure.mTotalTime() * 1000);
				}
				_nextOp->step(_tuple, aRelation, _vectorizedSize, false);
				lMeasure.start();
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_output[_vectorizedSize]._size = lCounter;
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				_meas += (lMeasure.mTotalTime() * 1000);
			}
			_nextOp->step(_tuple, aRelation, _vectorizedSize, true);
			lMeasure.start();
			lCounter = 0;
		}
		lRecordNo = 0;
	}
	lMeasure.stop();
	if(MEASURE_GLOBAL)
	{
		_meas += (lMeasure.mTotalTime() * 1000);
	}
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(PAX_Relation& aRelation)
{
	Measure lMeasure;
	if(MEASURE_GLOBAL)
	{
		lMeasure.start();
	}
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
				_output[lCounter]._tid[0] = lPageNo;
				_output[lCounter]._tid[1] = lRecordNo;
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				lMeasure.stop();
				if(MEASURE_GLOBAL)
				{
					_meas += (lMeasure.mTotalTime() * 1000);
				}
				_nextOp->step(_tuple, aRelation, _vectorizedSize, false);
				lMeasure.start();
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_output[_vectorizedSize]._size = lCounter;
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				_meas += (lMeasure.mTotalTime() * 1000);
			}
			_nextOp->step(_tuple, aRelation, _vectorizedSize, true);
			lMeasure.start();
		}
		lRecordNo = 0;
	}
	lMeasure.stop();
	if(MEASURE_GLOBAL)
	{
		_meas += (lMeasure.mTotalTime() * 1000);
	}
}


#endif