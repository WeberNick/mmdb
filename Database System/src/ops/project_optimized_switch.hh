#ifndef PROJECT_OPTIMIZED_SWITCH_HH
#define PROJECT_OPTIMIZED_SWITCH_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
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
		size_t _inputIndex;
		size_t _outputIndex;
		size_t _totalInputIndex;
		size_t _totalOutputIndex;

};

template<typename T_Consumer>
ProjectOptimizedSwitch<T_Consumer>::ProjectOptimizedSwitch(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _totalInputIndex(0), _totalOutputIndex(0)
{}

template<typename T_Consumer>
ProjectOptimizedSwitch<T_Consumer>::ProjectOptimizedSwitch(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _totalInputIndex(0), _totalOutputIndex(0)
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
	for(size_t i = 0; i < _vectorizedSize; ++i)
	{
		_output[i]._unval_pt = new unval_t[_noAttributes];
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
	unval_t* 	lAttrPointerOut;




	for(size_t i = 0; i < _noAttributes; ++i)
	{
		switch(aRelation.getAttrDesc()[i]._attrType)
		{
			case kCHAR:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_c8 = *(char*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						_inputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kUINT32:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_u32 = *(uint32_t*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kSTR_SDICT:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_s32 = *(uint32_t*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kUINT64:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_u64 = *(uint64_t*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kINT32:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_i32 = *(int32_t*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kFLOAT32:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_f32 = *(float*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kFLOAT64:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_f64 = *(double*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kCHAR_STRING:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_cp64 = *(const char**)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			case kDATEJD:
				while(_inputIndex < aSize)
				{
					lTupelPointerIn = _input[_inputIndex]._pointer;
					lTupelPointerOut = _output[_outputIndex]._unval_pt;
					lAttrPointerIn = lTupelPointerIn + aRelation.getOffset()[_attrNoList[i]];
					lAttrPointerOut = &lTupelPointerOut[i];
					lAttrPointerOut->_d32 = *(uint32_t*)lAttrPointerIn;
					++_outputIndex;
					++_inputIndex;
					if(_outputIndex == _vectorizedSize)
					{
						_outputIndex = 0;
						break;
					}
				}
				_outputIndex = 0;
				_inputIndex = 0;
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
	_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);

	if(_inputIndex < aSize)
	{
		step(aTupel, aRelation, aSize, aNoMoreData);
	}
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
	}
}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX aPageInterpreter;
	uint32_t lPageNo;
	uint32_t lRecordNo;
	uint lAttrSize;
	byte* lMiniPagePointer;

	unval_t* 	lTupelPointerOut;
	byte* 		lAttrPointerIn;
	unval_t* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		lPageNo = _input[_inputIndex]._tid[0];
		lRecordNo = _input[_inputIndex]._tid[1];

		aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));

		lTupelPointerOut = _output[_outputIndex]._unval_pt;


		for(size_t i = 0; i < _noAttributes; ++i)
		{
			lAttrSize = aPageInterpreter.getAttrSize(_attrNoList[i]);
			lMiniPagePointer = aPageInterpreter.getMiniPagePointer(_attrNoList[i]);

			lAttrPointerIn = lMiniPagePointer + (lAttrSize * lRecordNo);
			lAttrPointerOut = &lTupelPointerOut[i];

			switch(aRelation.getAttrDesc()[i]._attrType)
			{
				case kCHAR:
					lAttrPointerOut->_c8 = *(char*)lAttrPointerIn;
					break;
				case kUINT32:
					lAttrPointerOut->_u32 = *(uint32_t*)lAttrPointerIn;
					break;
				case kSTR_SDICT:
					lAttrPointerOut->_s32 = *(uint32_t*)lAttrPointerIn;
					break;
				case kUINT64:
					lAttrPointerOut->_u64 = *(uint64_t*)lAttrPointerIn;
					break;
				case kINT32:
					lAttrPointerOut->_i32 = *(int32_t*)lAttrPointerIn;
					break;
				case kFLOAT32:
					lAttrPointerOut->_f32 = *(float*)lAttrPointerIn;
					break;
				case kFLOAT64:
					lAttrPointerOut->_f64 = *(double*)lAttrPointerIn;
					break;
				case kCHAR_STRING:
					lAttrPointerOut->_cp64 = *(const char**)lAttrPointerIn;
					break;
				case kDATEJD:
					lAttrPointerOut->_d32 = *(uint32_t*)lAttrPointerIn;
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
			// _nextOp->printProjectOptimizedSwitchion(aTupel, aRelation, _vectorizedSize, _attrNoList); 
			if(_inputIndex < aSize)
			{
				step(aTupel, aRelation, aSize, aNoMoreData);
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printProjectOptimizedSwitchion(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

template<typename T_Consumer>
void
ProjectOptimizedSwitch<T_Consumer>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	for(size_t i = 0; i < _vectorizedSize; ++i)
	{
		delete[] _output[i]._unval_pt;
	}
	delete[] _output;
}



#endif