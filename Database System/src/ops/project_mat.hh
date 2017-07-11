#ifndef PROJECT_MAT_HH
#define PROJECT_MAT_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>
#include <type_traits>

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
class MaterializedProject
{
	public:
		MaterializedProject(T_Consumer* aConsumer, T_RelationIn& aRelation, const uint_vt& aAttrNoList);
		MaterializedProject(T_Consumer* aConsumer, T_RelationIn& aRelation,const uint_vt& aAttrNoList, const size_t aVectorizedSize);

	public:
		void init(unval_vt& aTupel);
		void step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void finish(unval_vt& aTupel);

	private:
		void projectNSMtoNSM(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void projectNSMtoPAX(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void projectPAXtoNSM(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void projectPAXtoPAX(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);

	private:
		T_Consumer* _nextOp;
		T_RelationIn& _relationIn;
		T_RelationOut _relationOut;
		const uint_vt& _attrNoList;
		const size_t _noAttributes;
		const size_t _vectorizedSize;
		size_t _indexNo1;
		unval_pt _input;
		size_t _indexNo2;
		unval_pt _output;
		size_t _inputIndex;
		size_t _outputIndex;
		uint32_t _currentPage;

};

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::MaterializedProject(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _currentPage(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::MaterializedProject(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _currentPage(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::init(unval_vt& aTupel) 
{
	std::string relName("Materialized");
	schema_vt logSchema(_noAttributes);
	Relation::attr_desc_vt attrDesc(_noAttributes);
	for(size_t i = 0; i < _noAttributes; ++i)
	{
		logSchema[i] = _relationIn.getLogSchema()[_attrNoList[i]];
		attrDesc[i] = _relationIn.getAttrDesc()[_attrNoList[i]];
	}
	Segment segment;
	Relation::cont_desc_vt contDesc;
	byte_vpt dict_entries_pointer;	//ToDo
	_relationOut.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);

	if(std::is_same<T_RelationOut, NSM_Relation>::value)
	{
		PageInterpreterSP lPageInterpreter;
		lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		PageInterpreterPAX lPageInterpreter;
		lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPartitionData());
	}
	else
	{
		std::cerr << "Relation Type not supported." << std::endl;
	}

	_indexNo1 = aTupel.size() - 1;
	_indexNo2 = aTupel.size();
	aTupel.push_back(unval_t());
	aTupel[_indexNo2]._unval_pt = new unval_t[_vectorizedSize];
	_input = aTupel[_indexNo1]._unval_pt;
	_output = aTupel[_indexNo2]._unval_pt;
	_nextOp->init(aTupel);
}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	if(std::is_same<T_RelationOut, NSM_Relation>::value)
	{
		projectNSMtoNSM(aTupel, aRelation, aSize, aNoMoreData);
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		projectNSMtoPAX(aTupel, aRelation, aSize, aNoMoreData);
	}
	else
	{
		std::cerr << "Relation Type not supported." << std::endl;
	}
}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{

	if(std::is_same<T_RelationOut, NSM_Relation>::value)
	{
		projectPAXtoNSM(aTupel, aRelation, aSize, aNoMoreData);
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		projectPAXtoPAX(aTupel, aRelation, aSize, aNoMoreData);
	}
	else
	{
		std::cerr << "Relation Type not supported." << std::endl;
	}
}

/**************************************************************************************************
** Different projection possibilities: ************************************************************
** NSM to NSM, ************************************************************************************
** NSM to PAX, ************************************************************************************
** PAX to NSM, ************************************************************************************
** PAX to PAX *************************************************************************************
***************************************************************************************************/

/**************************************************************************************************
** NSM to NSM *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::projectNSMtoNSM(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterSP lPageInterpreter;
	lPageInterpreter.attach(_relationOut.getSegment().getPage(_currentPage));
	byte* 	lRecordPointer;

	byte* 	lTupelPointerIn;
	byte* 	lAttrPointerIn;
	byte* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		lRecordPointer = lPageInterpreter.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer == 0)
		{
			lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());
			++_currentPage;
		}
		else
		{
			lTupelPointerIn = _input[_inputIndex]._pointer;
			_output[_outputIndex]._pointer = lRecordPointer;
			for(size_t i = 0; i < _noAttributes; ++i)
			{
				lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
				lAttrPointerOut = lRecordPointer + _relationOut.getOffset()[i];
				switch(_relationOut.getAttrDesc()[i]._attrType)
				{
					case kCHAR:
						*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
						break;
					case kUINT32:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kUINT64:
						*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
						break;
					case kINT32:
						*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
						break;
					case kFLOAT32:
						*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
						break;
					case kFLOAT64:
						*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
						break;
					case kCHAR_STRING:
						*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
						break;
					case kDATEJD:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kSTR_SDICT:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					default:
						throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
						break;
				}
			}
			++_outputIndex;
			++_inputIndex;
			if(_outputIndex == _vectorizedSize)
			{
				_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
				_outputIndex = 0;
				// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
				if(_inputIndex < aSize)
				{
					projectNSMtoNSM(aTupel, aRelation, aSize, aNoMoreData);
				}
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

/**************************************************************************************************
** NSM to PAX *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::projectNSMtoPAX(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreter;
	lPageInterpreter.attach(_relationOut.getSegment().getPage(_currentPage));

	byte* 	lTupelPointerIn;
	byte* 	lAttrPointerIn;
	byte* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		const int lIndex = lPageInterpreter.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndex == -1)
		{
			lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPartitionData());
			++_currentPage;
		}
		else
		{
			lTupelPointerIn = _input[_inputIndex]._pointer;
			_output[_outputIndex]._tid[0] = (uint32_t)_currentPage;
			_output[_outputIndex]._tid[1] = (uint32_t)lIndex;
			for(size_t i = 0; i < _noAttributes; ++i)
			{
				lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
				lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndex * lPageInterpreter.getAttrSize(i));
				switch(_relationOut.getAttrDesc()[i]._attrType)
				{
					case kCHAR:
						*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
						break;
					case kUINT32:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kUINT64:
						*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
						break;
					case kINT32:
						*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
						break;
					case kFLOAT32:
						*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
						break;
					case kFLOAT64:
						*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
						break;
					case kCHAR_STRING:
						*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
						break;
					case kDATEJD:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kSTR_SDICT:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					default:
						throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
						break;
				}
			}
			++_outputIndex;
			++_inputIndex;
			if(_outputIndex == _vectorizedSize)
			{
				_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
				_outputIndex = 0;
				// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
				if(_inputIndex < aSize)
				{
					projectNSMtoPAX(aTupel, aRelation, aSize, aNoMoreData);
				}
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

/**************************************************************************************************
** PAX to NSM *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::projectPAXtoNSM(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreterIn;
	PageInterpreterSP lPageInterpreterOut;
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));

	byte* 	lRecordPointer;

	uint32_t lPageNo;
	uint32_t lRecordNo;
	uint lAttrSize;
	byte* lMiniPagePointer;

	byte* 		lAttrPointerIn;
	byte* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		lPageNo = _input[_inputIndex]._tid[0];
		lRecordNo = _input[_inputIndex]._tid[1];
		lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
		lRecordPointer = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer == 0)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage());
			++_currentPage;
		}
		else
		{
			for(size_t i = 0; i < _noAttributes; ++i)
			{
				lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
				lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);

				lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
				lAttrPointerOut = lRecordPointer + _relationOut.getOffset()[i];

				switch(_relationOut.getAttrDesc()[i]._attrType)
				{
					case kCHAR:
						*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
						break;
					case kUINT32:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kUINT64:
						*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
						break;
					case kINT32:
						*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
						break;
					case kFLOAT32:
						*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
						break;
					case kFLOAT64:
						*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
						break;
					case kCHAR_STRING:
						*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
						break;
					case kDATEJD:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kSTR_SDICT:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					default:
						throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
						break;
				}
			}
			++_outputIndex;
			++_inputIndex;
			if(_outputIndex == _vectorizedSize)
			{
				_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
				_outputIndex = 0;
				// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
				if(_inputIndex < aSize)
				{
					projectPAXtoPAX(aTupel, aRelation, aSize, aNoMoreData);
				}
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

/**************************************************************************************************
** PAX to PAX *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::projectPAXtoPAX(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreterIn;
	PageInterpreterPAX lPageInterpreterOut;
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));

	uint32_t lPageNo;
	uint32_t lRecordNo;
	uint lAttrSize;
	byte* lMiniPagePointer;

	byte* 	lAttrPointerIn;
	byte* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		lPageNo = _input[_inputIndex]._tid[0];
		lRecordNo = _input[_inputIndex]._tid[1];

		lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));

		const int lIndex = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndex == -1)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPartitionData());
			++_currentPage;
		}
		else
		{
			for(size_t i = 0; i < _noAttributes; ++i)
			{
				lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
				lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);

				lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
				lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndex * lPageInterpreterOut.getAttrSize(i));

				switch(_relationOut.getAttrDesc()[i]._attrType)
				{
					case kCHAR:
						*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
						break;
					case kUINT32:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kUINT64:
						*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
						break;
					case kINT32:
						*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
						break;
					case kFLOAT32:
						*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
						break;
					case kFLOAT64:
						*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
						break;
					case kCHAR_STRING:
						*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
						break;
					case kDATEJD:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					case kSTR_SDICT:
						*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
						break;
					default:
						throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
						break;
				}
			}
			++_outputIndex;
			++_inputIndex;
			if(_outputIndex == _vectorizedSize)
			{
				_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
				_outputIndex = 0;
				// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
				if(_inputIndex < aSize)
				{
					projectPAXtoPAX(aTupel, aRelation, aSize, aNoMoreData);
				}
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printMaterializedProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}


template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
MaterializedProject<T_Consumer, T_RelationIn, T_RelationOut>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	delete[] _output;
}



#endif