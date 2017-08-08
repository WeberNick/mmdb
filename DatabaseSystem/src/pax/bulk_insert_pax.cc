#include "bulk_insert_pax.hh"

BulkInsertPAX::BulkInsertPAX()
{}

BulkInsertPAX::~BulkInsertPAX()
{}

void BulkInsertPAX::bulk_insert(const IntermediateBuffer& aBuffer, PAX_Relation* aRelation)
{
	PageInterpreterPAX lPageInterpreter;
	lPageInterpreter.initNewPage(aRelation->getSegment().getNewPage(), aRelation->getPhysLayoutData());
	size_t lTupleCount = 0;

	for(uint i = 0; i < aBuffer.getBuffer().size(); ++i)
	{
		unval_pt lBufferPointer = aBuffer.getBuffer()[i];
		for(uint j = 0; j < aBuffer.getTuplePerChunk(); )
		{
			if(lTupleCount == aBuffer.getTupleTotal())
			{
				break;
			}
			int lIndex = lPageInterpreter.addNewRecord(aRelation->getLogTupleSize());
			if(lIndex == -1)
			{
				lPageInterpreter.initNewPage(aRelation->getSegment().getNewPage(), aRelation->getPhysLayoutData());
			}
			else
			{
				try
				{
					insertTuple(lBufferPointer, lPageInterpreter, lIndex, aRelation);
					++lTupleCount;
					++j;
				}
				catch(BaseException& ex)
				{
					ex.print();
				}
			}
		}
	}
}

void BulkInsertPAX::bulk_insert(const BulkLoader& aBulkLoader, PAX_Relation* aRelation)
{
	bulk_insert(aBulkLoader.getIntermediateBuffer(), aRelation);
}

void BulkInsertPAX::insertTuple(unval_pt& aBufferPointer, PageInterpreterPAX& aPageInterpreter, const int aIndex, PAX_Relation* aRelation)
{
	byte* lTempPointer;
	for(uint i = 0; i < aRelation->getNoAttributes(); ++i)
	{
		lTempPointer = aPageInterpreter.pagePtr() + aPageInterpreter.getMiniPageOffset(i) + (aIndex * aPageInterpreter.getAttrSize(i));
		switch(aRelation->getLogSchema()[i])
		{
			case kCHAR:
				*(char*)lTempPointer = aBufferPointer->c8();
				break;
			case kUINT32:
				*(uint32_t*)lTempPointer = aBufferPointer->u32();
				break;
			case kUINT64:
				*(uint64_t*)lTempPointer = aBufferPointer->u64();
				break;
			case kINT32:
				*(int32_t*)lTempPointer = aBufferPointer->i32();
				break;
			case kFLOAT32:
				*(float*)lTempPointer = aBufferPointer->f32();
				break;
			case kFLOAT64:
				*(double*)lTempPointer = aBufferPointer->f64();
				break;
			case kCHAR_STRING:
				*(const char**)lTempPointer = aBufferPointer->cp64();
				break;
			case kDATEJD:
				*(uint32_t*)lTempPointer = aBufferPointer->d32();
				break;
			case kSTR_SDICT:
				*(uint32_t*)lTempPointer = aBufferPointer->s32();
				break;
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
		++aBufferPointer;
	}
}