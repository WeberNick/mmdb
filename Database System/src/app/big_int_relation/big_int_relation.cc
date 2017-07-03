#include "big_int_relation.hh"

template<typename T_Relation>
BIG_INT_RELATION<T_Relation>::BIG_INT_RELATION(bool aFlag) : _flag(aFlag), _relation()
{}

template BIG_INT_RELATION<NSM_Relation>::BIG_INT_RELATION(bool aFlag);
template BIG_INT_RELATION<PAX_Relation>::BIG_INT_RELATION(bool aFlag);

template<typename T_Relation>
void BIG_INT_RELATION<T_Relation>::init(const size_t aAttrNo, const size_t aSize, const size_t aBufferSize)
{
	initIntegerChunk(_relation, aAttrNo);
	BulkLoader bl(_relation, aBufferSize);
	bl.bulk_load_int_chunk(aSize);
	if(_flag)
	{
		BulkInsertSP bi;
		NSM_Relation* p = (NSM_Relation*)&_relation;
		bi.bulk_insert(bl, p);
	}
	else
	{
		BulkInsertPAX bi;
		PAX_Relation* p = (PAX_Relation*)&_relation;
		bi.bulk_insert(bl, p);
	}
}

template void BIG_INT_RELATION<NSM_Relation>::init(const size_t aAttrNo, const size_t aSize, const size_t aBufferSize);
template void BIG_INT_RELATION<PAX_Relation>::init(const size_t aAttrNo, const size_t aSize, const size_t aBufferSize);

template<typename T_Relation>
T_Relation& BIG_INT_RELATION<T_Relation>::getRelation()
{
	return _relation;
}

template NSM_Relation& BIG_INT_RELATION<NSM_Relation>::getRelation();
template PAX_Relation& BIG_INT_RELATION<PAX_Relation>::getRelation();