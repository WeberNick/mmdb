#ifndef PROJECT_HH
#define PROJECT_HH

#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>
#include <type_traits>

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
class Project
{
	public:
		Project(T_Consumer* aConsumer, T_RelationIn& aRelation, const uint_vt& aAttrNoList);
		Project(T_Consumer* aConsumer, T_RelationIn& aRelation,const uint_vt& aAttrNoList, const size_t aVectorizedSize);

	public:
		void init(unval_vt& aTupel);
		void step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData);
		void finish(unval_vt& aTupel);

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
		size_t _currentPage;

};

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
Project<T_Consumer, T_RelationIn, T_RelationOut>::Project(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _currentPage(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
Project<T_Consumer, T_RelationIn, T_RelationOut>::Project(
									T_Consumer* aConsOp,
									T_RelationIn& aRelation, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relationIn(aRelation), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0), _currentPage(0)
{}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
Project<T_Consumer, T_RelationIn, T_RelationOut>::init(unval_vt& aTupel) 
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

	PageInterpreterSP lPageInterpreter;
	lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());

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
Project<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	if(std::is_same<T_RelationIn, T_RelationOut>::value)
	{
		
	}
	else
	{

	}
	PageInterpreterSP lPageInterpreter;
	lPageInterpreter.attach(_relationOut.getSegment().getPage(_currentPage));

	byte* 	lTupelPointerIn;
	byte* 	lRecordPointer;

	byte* 	lAttrPointerIn;
	byte* 	lAttrPointerOut;

	while(_inputIndex < aSize)
	{
		lRecordPointer = lPageInterpreter.addNewRecord(_relationOut.getLogTupleSize());
		if(lRecordPointer == 0)
		{
			lPageInterpreter.initNewPage(_relationOut.getSegment().getNewPage());
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
				// _nextOp->printProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
				if(_inputIndex < aSize)
				{
					step(aTupel, aRelation, aSize, aNoMoreData);
				}
			}
		}
	}
	_inputIndex = 0;
	if(aNoMoreData && _outputIndex > 0)
	{
		_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
		_outputIndex = 0;
		// _nextOp->printProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
Project<T_Consumer, T_RelationIn, T_RelationOut>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX lPageInterpreter;
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

		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));

		lTupelPointerOut = _output[_outputIndex]._unval_pt;


		for(size_t i = 0; i < _noAttributes; ++i)
		{
			lAttrSize = lPageInterpreter.getAttrSize(_attrNoList[i]);
			lMiniPagePointer = lPageInterpreter.getMiniPagePointer(_attrNoList[i]);

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
			// _nextOp->printProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
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
		// _nextOp->printProjection(aTupel, aRelation, _vectorizedSize, _attrNoList); 
	}
}

template<typename T_Consumer, typename T_RelationIn, typename T_RelationOut>
void
Project<T_Consumer, T_RelationIn, T_RelationOut>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	delete[] _output;
}



#endif