#ifndef TEST_PRED_HH
#define TEST_PRED_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

class TestPred
{
	public:
		struct Parameter
		{
			uint someTestParaAttrNo;
			float someTestParaValue;
		};

	public:
		TestPred(Parameter aParas);

	public:
		const bool operator()(const unval_t* aInputTuple, unval_t* aOutputTuple, const size_t aInputSize, const size_t aOutputSize, NSM_Relation& aRelation);
		const bool operator()(const unval_t* aInputTuple, unval_t* aOutputTuple, const size_t aInputSize, const size_t aOutputSize, PAX_Relation& aRelation);

		const bool operator()(const byte* aRecordPointer, NSM_Relation& aRelation);
		const bool operator()(const size_t aPageNo, const size_t aRecordNo, PAX_Relation& aRelation);

	public:
		inline const size_t getInputIndex() const;
		inline void setInputIndexZero();
		inline const size_t getOutputIndex() const;

	private:
		size_t _inputIndex;
		size_t _outputIndex;

		Parameter _paras;

};

const size_t TestPred::getInputIndex() const
{
	return _inputIndex;
}

void TestPred::setInputIndexZero()
{
	_inputIndex = 0;
}

const size_t TestPred::getOutputIndex() const
{
	return _outputIndex;
}

#endif