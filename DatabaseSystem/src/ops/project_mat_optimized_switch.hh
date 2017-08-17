/**
 *	@file 	project_mat_optimized_switch.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Project operator of the physical algebra, used to project and materialize attributes
 *	@bugs 	Currently no bugs known
 *	@todos	PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION
 *	TODO
 *
 */
#ifndef PROJECT_MAT_OPTIMIZED_SWITCH_HH
#define PROJECT_MAT_OPTIMIZED_SWITCH_HH

#include "infra/webe/types.hh"
#include "relation/relation_infra.hh"
#include "relation/relation.hh"
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
		ProjectMaterializedOptimizedSwitch(T_Consumer* aConsumer, T_RelationIn& aRelation,const uint_vt& aAttrNoList, const size_t aVectorizedSize = 1);

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
									const uint_vt& aAttrNoList, 
									const size_t aVectorizedSize)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _currentPage(0), _inputBegin(0), _outputBegin(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::init(unval_vt& aTupel) 
{
	std::string relName("Materialized");
	schema_vt logSchema(_noAttributes);
	attr_desc_vt attrDesc(_noAttributes);
	for(size_t i = 0; i < _noAttributes; ++i)
	{
		logSchema[i] = _relationIn.getLogSchema()[_attrNoList[i]];
		attrDesc[i] = _relationIn.getAttrDesc()[_attrNoList[i]];
	}
	Segment segment;
	cont_desc_vt contDesc;
	byte_vpt dict_entries_pointer;	//ToDo
	_relationOut.initRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);

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
		lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPhysLayoutData());
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
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
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
ProjectMaterializedOptimizedSwitch<T_Consumer, T_RelationIn, T_RelationOut>::projectNSMtoNSM(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterSP 	lPageInterpreterOut;
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));
	byte*				lRecordPointer;
	size_t				lOffsetIn;
	size_t				lOffsetOut;
	size_t 				lOutputIndex = _outputBegin;
	size_t 				lInputIndex = _inputBegin;
	const size_t 		lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lRecordPointer = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer == 0)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage());
			++_currentPage;
		}
		else
		{
			_output[lOutputIndex]._pointer = lRecordPointer;
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
		lOffsetIn = aRelation.getPhysLayoutData()[_attrNoList[i]];
		lOffsetOut = _relationOut.getPhysLayoutData()[i];
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					*(char*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(char*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					*(uint64_t*)(_output[lOutputIndex]._pointer + lOffsetOut) = *(uint64_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					*(int32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(int32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					*(float*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(float*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					*(double*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(double*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					*(const char**)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(const char**)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
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
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectNSMtoNSM(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
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
	PageInterpreterPAX 	lPageInterpreterOut;
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));
	int_vt				lIndeces(_vectorizedSize);
	size_t 				lOffsetIn;
	size_t 				lOutputIndex = _outputBegin;
	size_t 				lInputIndex = _inputBegin;
	size_t 				lAttrSize;
	const size_t		lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lIndeces[lOutputIndex] = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndeces[lOutputIndex] == -1)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPhysLayoutData());
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
		lOffsetIn = aRelation.getPhysLayoutData()[_attrNoList[i]];
		lAttrSize = _relationOut.getPhysLayoutData()[i];
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(char*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(char*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint64_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint64_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(int32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(int32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(float*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(float*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(double*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(double*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(const char**)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(const char**)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(_input[lInputIndex++]._pointer + lOffsetIn);
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
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectNSMtoPAX(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
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
	PageInterpreterPAX 	lPageInterpreterIn;
	PageInterpreterSP 	lPageInterpreterOut;
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));
	byte_vpt			lRecordPointer(_vectorizedSize);
	uint32_t 			lPageNo;
	uint32_t 			lRecordNo;
	size_t				lOffsetOut;
	size_t 				lOutputIndex = _outputBegin;
	size_t 				lInputIndex = _inputBegin;
	size_t 				lAttrSize;
	const size_t 		lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
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
		lAttrSize = aRelation.getPhysLayoutData()[_attrNoList[i]];
		lOffsetOut = _relationOut.getPhysLayoutData()[i];
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(char*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(char*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(uint64_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint64_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(int32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(int32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(float*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(float*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(double*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(double*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(const char**)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(const char**)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					*(uint32_t*)(_output[lOutputIndex++]._pointer + lOffsetOut) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
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
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectPAXtoNSM(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
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
	lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_currentPage));

	int_vt			lIndeces(_vectorizedSize);
	uint32_t 		lPageNo;
	uint32_t 		lRecordNo;
	size_t 			lOutputIndex = _outputBegin;
	size_t 			lInputIndex = _inputBegin;
	size_t 			lAttrSize;
	const size_t 	lNoAttributes = _noAttributes;
	while(lInputIndex < aSize)
	{
		lIndeces[lOutputIndex] = lPageInterpreterOut.addNewRecord(_relationOut.getLogTupleSize());
		if(lIndeces[lOutputIndex] == -1)
		{
			lPageInterpreterOut.initNewPage(_relationOut.getSegment().getNewPage(), _relationOut.getPhysLayoutData());
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
		lAttrSize = aRelation.getPhysLayoutData()[_attrNoList[i]];
		switch(_relationOut.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(char*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(char*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint64_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint64_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(int32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(int32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(float*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(float*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(double*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(double*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(const char**)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(const char**)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex++]._tid[1];
					lPageInterpreterIn.attach(aRelation.getSegment().getPage(lPageNo));
					lPageInterpreterOut.attach(_relationOut.getSegment().getPage(_output[lOutputIndex]._tid[0]));
					*(uint32_t*)(lPageInterpreterOut.getMiniPagePointer(i) + (lIndeces[lOutputIndex++] * lAttrSize)) = *(uint32_t*)(lPageInterpreterIn.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo));
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
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
		_outputBegin = 0;
		if(_inputBegin < aSize)
		{
			projectPAXtoPAX(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, _relationOut, _outputBegin, aNoMoreData);
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