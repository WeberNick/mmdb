#ifndef PROJECT_OPTIMIZED_SWITCH_HH
#define PROJECT_OPTIMIZED_SWITCH_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>

template<typename T_Consumer>
class ProjectOptimizedSwitch
{
	public:
		ProjectOptimizedSwitch(T_Consumer* aConsumer, const uint_vt& aAttrNoList);
		ProjectOptimizedSwitch(T_Consumer* aConsumer, const uint_vt& aAttrNoList, const size_t aVectorizedSize);

	public:
		void init(unval_vt& aTupel);
		void step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void finish(unval_vt& aTupel);

	private:
		T_Consumer* _nextOp;
		const uint_vt& _attrNoList;
		const size_t _noAttributes;
		const size_t _vectorizedSize;
		size_t _indexNo1;
		unval_pt _input;
		size_t _indexNo2;
		unval_pt _output;
		size_t _inputBegin;
		size_t _outputBegin;

};

template<typename T_Consumer>
ProjectOptimizedSwitch<T_Consumer>::ProjectOptimizedSwitch(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputBegin(0), _outputBegin(0)
{}

template<typename T_Consumer>
ProjectOptimizedSwitch<T_Consumer>::ProjectOptimizedSwitch(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputBegin(0), _outputBegin(0)
{}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::init(unval_vt& aTupel) 
{
	_indexNo1 = aTupel.size() - 1;
	_indexNo2 = aTupel.size();
	aTupel.push_back(unval_t());
	aTupel[_indexNo2]._unval_pt = new unval_t[_vectorizedSize];
	_input = aTupel[_indexNo1]._unval_pt;
	_output = aTupel[_indexNo2]._unval_pt;
	_output[0]._unval_pt = new unval_t[_vectorizedSize * _noAttributes];
	for(size_t i = 1; i < _vectorizedSize; ++i)
	{
		_output[i]._unval_pt = _output[i-1]._unval_pt + _noAttributes;
	}
	_nextOp->init(aTupel);
}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	byte* 		lTupelPointerIn;
	unval_t* 	lTupelPointerOut;
	byte* 		lAttrPointerIn;
	size_t lOutputIndex = _outputBegin;
	size_t lInputIndex = _inputBegin;
	const size_t lNoAttributes = _noAttributes;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		switch(aRelation.getAttrDesc()[_attrNoList[i]]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_c8 = *(char*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_u32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_s32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_u64 = *(uint64_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_i32 = *(int32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_f32 = *(float*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_f64 = *(double*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_cp64 = *(const char**)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lTupelPointerIn = _input[lInputIndex]._pointer;
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
					(&lTupelPointerOut[i])->_d32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
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
			step(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	size_t lAttrSize;
	PageInterpreterPAX aPageInterpreter;
	uint32_t lPageNo;
	uint32_t lRecordNo;
	unval_t* 	lTupelPointerOut;
	byte* 		lAttrPointerIn;
	size_t lOutputIndex = _outputBegin;
	size_t lInputIndex = _inputBegin;
	const size_t lNoAttributes = _noAttributes;

	for(size_t i = 0; i < lNoAttributes; ++i)
	{
		lAttrSize = aRelation.getPhysLayoutData()[_attrNoList[i]];
		switch(aRelation.getAttrDesc()[_attrNoList[i]]._attrType)
		{
			case kCHAR:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_c8 = *(char*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_u32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kSTR_SDICT:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_s32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kUINT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_u64 = *(uint64_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kINT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_i32 = *(int32_t*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT32:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_f32 = *(float*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kFLOAT64:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_f64 = *(double*)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kCHAR_STRING:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_cp64 = *(const char**)lAttrPointerIn;
					++lInputIndex;
					if(lOutputIndex == _vectorizedSize)
					{
						break;
					}
				}
				break;
			case kDATEJD:
				while(lInputIndex < aSize)
				{
					lPageNo = _input[lInputIndex]._tid[0];
					lRecordNo = _input[lInputIndex]._tid[1];
					aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
					lTupelPointerOut = _output[lOutputIndex++]._unval_pt;
					lAttrPointerIn = aPageInterpreter.getMiniPagePointer(_attrNoList[i]) + (lAttrSize * lRecordNo);
					(&lTupelPointerOut[i])->_d32 = *(uint32_t*)lAttrPointerIn;
					++lInputIndex;
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
			step(aTupel, aRelation, aSize, aNoMoreData);
		}
	}
	if(aNoMoreData && _outputBegin > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputBegin, aNoMoreData);
		_outputBegin = 0;
	}
	_inputBegin = 0;
}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	delete[] _output[0]._unval_pt;
	delete[] _output;
}



#endif