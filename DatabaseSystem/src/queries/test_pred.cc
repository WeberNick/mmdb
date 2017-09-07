#include "test_pred.hh"

TestPred::TestPred(Parameter aParas) : _inputIndex(0), _outputIndex(0), _paras(aParas){}

const bool TestPred::operator()(const unval_t* aInputTuple, unval_t* aOutputTuple, const size_t aInputSize, const size_t aOutputSize, NSM_Relation& aRelation)
{
	const uint someTestParaOffset = aRelation.getPhysLayoutData()[_paras.someTestParaAttrNo];
	while(_inputIndex < aInputSize)
	{
		if(*(float*)(aInputTuple[_inputIndex]._pointer + someTestParaOffset) < _paras.someTestParaValue)
		{
			aOutputTuple[_outputIndex]._pointer = aInputTuple[_inputIndex]._pointer;
			++_outputIndex;

			if(_outputIndex == aOutputSize)
			{
				_outputIndex = 0;
				++_inputIndex;
				return true;
			}
		}
		++_inputIndex;
	}
	_inputIndex = 0;
	return false;
}

const bool TestPred::operator()(const unval_t* aInputTuple, unval_t* aOutputTuple, const size_t aInputSize, const size_t aOutputSize, PAX_Relation& aRelation)
{
	PageInterpreterPAX aPageInterpreter;
	uint32_t lPageNo = aInputTuple[_inputIndex]._tid[0];
	aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
	byte* lMiniPagePointer = aPageInterpreter.getMiniPagePointer(_paras.someTestParaAttrNo);
	const uint lAttrSize = aRelation.getPhysLayoutData()[_paras.someTestParaAttrNo];
	while(_inputIndex < aInputSize)
	{
		if(lPageNo != aInputTuple[_inputIndex]._tid[0])
		{
			lPageNo = aInputTuple[_inputIndex]._tid[0];
			aPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
			lMiniPagePointer = aPageInterpreter.getMiniPagePointer(_paras.someTestParaAttrNo);
		}

		uint32_t lRecordNo = aInputTuple[_inputIndex]._tid[1];
		if(*(float*)(lMiniPagePointer + (lAttrSize * lRecordNo)) < _paras.someTestParaValue)
		{
			aOutputTuple[_outputIndex]._tid[0] = lPageNo;
			aOutputTuple[_outputIndex]._tid[1] = lRecordNo;
			++_outputIndex;

			if(_outputIndex == aOutputSize)
			{
				_outputIndex = 0;
				++_inputIndex;
				return true;
			}
		}
		++_inputIndex;
	}
	_inputIndex = 0;
	return false;
}

const bool TestPred::operator()(const byte* aRecordPointer, NSM_Relation& aRelation)
{
	const uint someTestParaOffset = aRelation.getPhysLayoutData()[_paras.someTestParaAttrNo];
	if(*(float*)(aRecordPointer + someTestParaOffset) < _paras.someTestParaValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool TestPred::operator()(const size_t aPageNo, const size_t aRecordNo, PAX_Relation& aRelation)
{

	PageInterpreterPAX aPageInterpreter;
	aPageInterpreter.attach(aRelation.getSegment().getPage(aPageNo));
	const uint lAttrSize = aRelation.getPhysLayoutData()[_paras.someTestParaAttrNo];
	if(*(float*)(aPageInterpreter.getMiniPagePointer(_paras.someTestParaAttrNo) + (lAttrSize * aRecordNo)) < _paras.someTestParaValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}