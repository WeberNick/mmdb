#ifndef PROJECT_MAT_OPTIMIZED_SWITCH_HH
#define PROJECT_MAT_OPTIMIZED_SWITCH_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>
#include <type_traits>

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
class ProjectMaterializedOptimizedSwitch
{
	public:
		ProjectMaterializedOptimizedSwitch(T_Consumer* aConsumer, T_RelationIn& aRelation, const uint_vt& aAttrNoList);
		ProjectMaterializedOptimizedSwitch(T_Consumer* aConsumer, T_RelationIn& aRelation,const uint_vt& aAttrNoList, const size_t aVectorizedSize);

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
		uint32_t _currentPage;
		size_t _inputBegin;
		size_t _outputBegin;

};

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::ProjectMaterializedOptimizedSwitch(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _currentPage(0), _inputBegin(0), _outputBegin(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::ProjectMaterializedOptimizedSwitch(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _currentPage(0), _inputBegin(0), _outputBegin(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::init(unval_vt& aTupel) 
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
		std::cout << "Project to NSM" << std::endl;
		PageInterpreterSP lPageInterpreter;
		lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		std::cout << "Project to PAX" << std::endl;
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
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	if(std::is_same<T_RelationOut, NSM_Relation>::value)
	{
		std::cout << "NSM to NSM" << std::endl;
		//projectNSMtoNSM(aTupel, aRelation, aSize, aNoMoreData);
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		std::cout << "NSM to PAX" << std::endl;
		projectNSMtoPAX(aTupel, aRelation, aSize, aNoMoreData);
	}
	else
	{
		std::cerr << "Relation Type not supported." << std::endl;
	}
}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{

	if(std::is_same<T_RelationOut, NSM_Relation>::value)
	{
		std::cout << "PAX to NSM" << std::endl;
		//projectPAXtoNSM(aTupel, aRelation, aSize, aNoMoreData);
	}
	else if(std::is_same<T_RelationOut, PAX_Relation>::value)
	{
		std::cout << "PAX to PAX" << std::endl;
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
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::projectNSMtoNSM(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterSP lPageInterpreter;
	byte_vpt		lRecordPointer(_vectorizedSize);
	byte* 			lAttrPointerIn;
	byte* 			lAttrPointerOut;
	size_t 			lOutputIndex = _outputBegin;
	size_t 			lInputIndex = _inputBegin;
	const size_t 	lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lPageInterpreter.attach(_relationOut.getSegment().getPage(_currentPage));
		lRecordPointer[lOutputIndex] = lPageInterpreter.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer[lOutputIndex] == 0)
		{
			lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());
			++_currentPage;
		}
		else
		{
			_output[lOutputIndex]._pointer = lRecordPointer[lOutputIndex];
			++lOutputIndex;
			++lInputIndex;
			if(lOutputIndex == _vectorizedSize)
			{
				break;
			}
		}
	}
	lOutputIndex = _outputBegin;
	lInputIndex = _inputBegin;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
		if((i + 1) < lNoAttributes)
		{
			lOutputIndex = _outputBegin;
			lInputIndex = _inputBegin;
		}
	}
	_outputBegin = lOutputIndex;
	_inputBegin = lInputIndex;
	if(_outputBegin == _vectorizedSize)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectNSMtoNSM(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}

/**************************************************************************************************
** NSM to PAX *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::projectNSMtoPAX(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreter;

	int_vt			lIndeces(_vectorizedSize);
	byte* 			lAttrPointerIn;
	byte* 			lAttrPointerOut;
	size_t 			lOutputIndex = _outputBegin;
	size_t 			lInputIndex = _inputBegin;
	const size_t	lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lPageInterpreter.attach(_relationOut.getSegment().getPage(_currentPage));
		lIndeces[lOutputIndex] = lPageInterpreter.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndeces[lOutputIndex] == -1)
		{
			lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPartitionData());
			++_currentPage;
		}
		else
		{
			_output[lOutputIndex]._tid[0] = (uint32_t)_currentPage;
			_output[lOutputIndex]._tid[1] = (uint32_t)lIndeces[lOutputIndex];
			++lOutputIndex;
			++lInputIndex;
			if(lOutputIndex == _vectorizedSize)
			{
				break;
			}
		}
	}
	lOutputIndex = _outputBegin;
	lInputIndex = _inputBegin;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageInterpreter.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerIn = _input[lInputIndex++]._pointer + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = lPageInterpreter.pagePtr() + lPageInterpreter.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreter.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	_outputBegin = lOutputIndex;
	_inputBegin = lInputIndex;
	if(_outputBegin == _vectorizedSize)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectNSMtoPAX(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}

/**************************************************************************************************
** PAX to NSM *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::projectPAXtoNSM(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreterIn;
	PageInterpreterSP lPageInterpreterOut;
	byte_vpt	lRecordPointer(_vectorizedSize);
	uint32_t 	lPageNo;
	uint32_t 	lRecordNo;
	uint 		lAttrSize;
	byte* 		lMiniPagePointer;
	byte* 		lAttrPointerIn;
	byte* 		lAttrPointerOut;
	size_t 		lOutputIndex = _outputBegin;
	size_t 		lInputIndex = _inputBegin;
	const size_t lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));
		lRecordPointer[lOutputIndex] = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer[lOutputIndex] == 0)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage());
			++_currentPage;
		}
		else
		{
			_output[lOutputIndex]._pointer = lRecordPointer[lOutputIndex];
			++lOutputIndex;
			++lInputIndex;
			if(lOutputIndex == _vectorizedSize)
			{
				break;
			}
		}
	}
	lOutputIndex = _outputBegin;
	lInputIndex = _inputBegin;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lAttrPointerOut = lRecordPointer[lOutputIndex++] + _relationOut.getOffset()[i];
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	_outputBegin = lOutputIndex;
	_inputBegin = lInputIndex;
	if(_outputBegin == _vectorizedSize)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectPAXtoNSM(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}

/**************************************************************************************************
** PAX to PAX *************************************************************************************
***************************************************************************************************/

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::projectPAXtoPAX(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreterIn;
	PageInterpreterPAX lPageInterpreterOut;

	int_vt			lIndeces(_vectorizedSize);
	uint32_t 		lPageNo;
	uint32_t 		lRecordNo;
	uint 			lAttrSize;
	byte* 			lMiniPagePointer;
	byte* 			lAttrPointerIn;
	byte* 			lAttrPointerOut;

	size_t 			lOutputIndex = _outputBegin;
	size_t 			lInputIndex = _inputBegin;
	const size_t 	lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));
		lIndeces[lOutputIndex] = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndeces[lOutputIndex] == -1)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPartitionData());
			++_currentPage;
		}
		else
		{
			_output[lOutputIndex]._tid[0] = (uint32_t)_currentPage;
			_output[lOutputIndex]._tid[1] = (uint32_t)lIndeces[lOutputIndex];
			++lOutputIndex;
			++lInputIndex;
			if(lOutputIndex == _vectorizedSize)
			{
				break;
			}
		}
	}
	lOutputIndex = _outputBegin;
	lInputIndex = _inputBegin;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(char*)lAttrPointerOut = *(char*)lAttrPointerIn;
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(uint64_t*)lAttrPointerOut = *(uint64_t*)lAttrPointerIn;
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(int32_t*)lAttrPointerOut = *(int32_t*)lAttrPointerIn;
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(float*)lAttrPointerOut = *(float*)lAttrPointerIn;
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(double*)lAttrPointerOut = *(double*)lAttrPointerIn;
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(const char**)lAttrPointerOut = *(const char**)lAttrPointerIn;
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lAttrSize = lPageInterpreterIn.getAttrSize(_attrNoList[i]);
					lMiniPagePointer = lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]);
					lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					lAttrPointerOut = lPageInterpreterOut.pagePtr() + lPageInterpreterOut.getMiniPageOffset(i) + (lIndeces[lOutputIndex++] * lPageInterpreterOut.getAttrSize(i));
					*(uint32_t*)lAttrPointerOut = *(uint32_t*)lAttrPointerIn;
				}
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	_outputBegin = lOutputIndex;
	_inputBegin = lInputIndex;
	if(_outputBegin == _vectorizedSize)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectPAXtoPAX(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}


template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	delete[] _output;
}



#endif