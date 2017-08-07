#include "bulk_insert_pax.hh"

BulkInsertPAX::BulkInsertPAX()
{}

void BulkInsertPAX::bulk_insert(const std::vector<unval_t*>& aBuffer, const uint aTuplePerChunk, const uint aTupleTotal, PAX_Relation* aRelation)
{
	// uint_vt lMiniPageOffsets(aRelation->getNoAttributes());
	// uint_vt lMiniPageAttrSize(aRelation->getNoAttributes());
	PageInterpreterPAX lPageInterpreter;
	lPageInterpreter.initNewPage(aRelation->getSegment().getNewPage(), aRelation->getPhysLayoutData());
	// for(uint i = 0; i < aRelation->getNoAttributes(); ++i)
 //    {
 //      lMiniPageOffsets[i] = lPageInterpreter.getMiniPageOffset(i);
 //      lMiniPageAttrSize[i] = lPageInterpreter.getAttrSize(i);
 //    }
	uint lTupleCount = 0;

	for(uint i = 0; i < aBuffer.size(); ++i)
	{
		unval_t* lBufferPointer = aBuffer[i];
		for(uint j = 0; j < aTuplePerChunk; )
		{
			if(lTupleCount == aTupleTotal)
			{
				break;
			}
			int index = lPageInterpreter.addNewRecord(aRelation->getLogTupleSize());
			if(index == -1)
			{
				lPageInterpreter.initNewPage(aRelation->getSegment().getNewPage(), aRelation->getPhysLayoutData());
			}
			else
			{
				try
				{
					insertTuple(aRelation, lBufferPointer, lPageInterpreter, index);
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
	bulk_insert(aBulkLoader.getBuffer(), aBulkLoader.getTuplePerChunk(), aBulkLoader.getTupleTotal(), aRelation);
}

void BulkInsertPAX::insertTuple(PAX_Relation* aRelation, unval_t*& aBufferPointer, PageInterpreterPAX& aPageInterpreter, int aIndex)
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