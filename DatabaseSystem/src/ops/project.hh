/**
 *	@file 	project.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Project operator of the physical algebra, used to project attributes
 *	@bugs 	Currently no bugs known
 *	@todos	PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION
 *	This template class implements the physical algebra operator 'Project'.
 *	The project operator can be used to project certain attributes. 
 *	Depending on the storage layout in use (NSM or PAX), different approaches 
 *	of how to project attributes must be implemented.
 *	
 *	NOTE: 	Several different implementations of the project operator are
 *			implemented. The following description is only valid for this
 *			exact file/operator.
 *
 *	First the description for the storage layout independent part:
 *
 *	General	At the start, the operator is initialized by calling the 'init'
 *			procedure. This allocates memory for the output of the project 
 *			and initializes certain variables neeeded by the project 
 *			operator. As soon as the 'step' procedure is called by a
 *			previous operator, the projection process starts. The project 
 *			operator uses an associated attribute list indicating which 
 *			attributes have to be projected. An attribute is projected by 
 *			finding the respective stored attribute's address, copying its
 *			content and writing it to destination address. In this projection
 *			implementation, the attributes are projected into an union type
 *			data structure which exists only as long the projection operator
 *			exists (the projected attributes are not materialized). This 
 *			approach was easy and fast to implement but doesn't have full
 *			compatibility with other operators which have to work with the
 *			output produced by this operator. Instead of writing the output
 *			on a memory page (in either NSM or PAX format), this operator
 *			uses an union type storage (basically a row store) but the
 *			following operators will expect a output format as produced by
 *			the scan or select for example. The output data structure can contain
 *			either a single record (tuple at a time) or a user defined
 *			number of records (@see scan.hh for more detailed information).
 *
 *	
 *	The description for the NSM specific part:		
 *			
 *	NSM		TODO
 *
 *
 *	The description for the PAX specific part:	
 *	
 *	PAX 	TODO
 */
#ifndef PROJECT_HH
#define PROJECT_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>

template<typename T_Consumer>
class Project
{
	public:
		Project(T_Consumer* aConsumer, const uint_vt& aAttrNoList);
		Project(T_Consumer* aConsumer, const uint_vt& aAttrNoList, const size_t aVectorizedSize);

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

};

template<typename T_Consumer>
Project<T_Consumer>::Project(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(1), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0)
{}

template<typename T_Consumer>
Project<T_Consumer>::Project(
									T_Consumer* aConsOp, 
									const uint_vt& aAttrNoList, 
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _attrNoList(aAttrNoList), _noAttributes(_attrNoList.size()), _vectorizedSize(aVectorizedSize), _indexNo1(0), _input(), _indexNo2(0), _output(), _inputIndex(0), _outputIndex(0)
{}

template<typename T_Consumer>
void
Project<T_Consumer>::init(unval_vt& aTupel) 
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
Project<T_Consumer>::step(unval_vt& aTupel, NSM_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	byte* 		lTupelPointerIn;
	unval_t* 	lTupelPointerOut;
	byte* 		lAttrPointerIn;
	unval_t* 	lAttrPointerOut;
	const size_t lNoAttributes = _noAttributes;

	while(_inputIndex < aSize)
	{
		lTupelPointerIn = _input[_inputIndex]._pointer;
		lTupelPointerOut = _output[_outputIndex++]._unval_pt;
		for(size_t i = 0; i < lNoAttributes; ++i)
		{
			lAttrPointerIn = lTupelPointerIn + aRelation.getPhysLayoutData()[_attrNoList[i]];
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
		++_inputIndex;
		if(_outputIndex == _vectorizedSize)
		{
			_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
			_outputIndex = 0;
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
	}
}

template<typename T_Consumer>
void
Project<T_Consumer>::step(unval_vt& aTupel, PAX_Relation& aRelation, const size_t aSize, const bool aNoMoreData) 
{
	PageInterpreterPAX aPageInterpreter;
	uint32_t lPageNo;
	uint32_t lRecordNo;
	uint lAttrSize;
	byte* lMiniPagePointer;
	unval_t* 	lTupelPointerOut;
	byte* 		lAttrPointerIn;
	unval_t* 	lAttrPointerOut;
	const size_t lNoAttributes = _noAttributes;

	while(_inputIndex < aSize)
	{
		lPageNo = _input[_inputIndex]._tid[0];
		lRecordNo = _input[_inputIndex]._tid[1];

		aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));

		lTupelPointerOut = _output[_outputIndex++]._unval_pt;

		for(size_t i = 0; i < lNoAttributes; ++i)
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
		++_inputIndex;
		if(_outputIndex == _vectorizedSize)
		{
			_nextOp->step(aTupel, aRelation, _outputIndex, aNoMoreData);
			_outputIndex = 0;
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
	}
}

template<typename T_Consumer>
void
Project<T_Consumer>::finish(unval_vt& aTupel) 
{
	_nextOp->finish(aTupel);
	// for(size_t i = 0; i < _vectorizedSize; ++i)
	// {
	// 	delete[] _output[i]._unval_pt;
	// }
	delete[] _output[0]._unval_pt;
	delete[] _output;
}



#endif