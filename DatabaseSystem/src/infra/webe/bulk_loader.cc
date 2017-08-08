#include "bulk_loader.hh"

BulkLoader::BulkLoader(const char* aFileName, const char aDelimiter, const char aSeperator, const size_t aBufferSize, Relation& aRelation) :
	_lineReader(aFileName), 
	_delimiter(aDelimiter),
	_seperator(aSeperator),
	_intermediate_buffer(aBufferSize),
	_relation(aRelation)
{}

BulkLoader::BulkLoader(const size_t aBufferSize, Relation& aRelation) :
	_lineReader(""), 
	_delimiter(),
	_seperator(),
	_intermediate_buffer(aBufferSize),
	_relation(aRelation)
{}

BulkLoader::~BulkLoader()
{}

void BulkLoader::bulk_load()
{
	try
	{
		if(_lineReader.open())
		{
			unval_t* lChunk = 0;
			do
			{
				if(_lineReader.linecount() % _intermediate_buffer.getTuplePerChunk() == 1)
				{
					lChunk = new unval_t[_relation.getLogSchema().size() * _intermediate_buffer.getTuplePerChunk()];
					_intermediate_buffer.addChunkToBuffer(lChunk);
				}
				try
				{
					readTuple(lChunk);
				} 
				catch(BaseException& ex)
				{
					ex.print();
				}
			}
			while(_lineReader.next());
			_intermediate_buffer.setTupleTotal(_lineReader.linecount());
			_lineReader.close();
		}
	}
	catch(SimpleException& ex)
	{
		throw FileException(__FILE__, __LINE__, __PRETTY_FUNCTION__, _lineReader.filename(), "Cannot open file or file has already been opened.");
	}
}

void BulkLoader::readTuple(unval_t*& aChunk)
{
	const char* x = _lineReader.begin();
	const char* lBegin = 0;
	const char* lEnd = 0;
	DateJd aDate;
	for(uint i = 0; i < _relation.getLogSchema().size(); ++i)
	{
		switch(_relation.getLogSchema()[i])
		{
			case kCHAR:
				if(!(_lineReader.read_char(x, aChunk->_c8)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kUINT32:
				if(!(_lineReader.read_uint(x, aChunk->_u32)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kUINT64:
				if(!(_lineReader.read_uint64(x, aChunk->_u64)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kINT32:
				if(!(_lineReader.read_int(x, aChunk->_i32)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kFLOAT32:
				if(!(_lineReader.read_float(x, aChunk->_f32)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kFLOAT64:
				if(!(_lineReader.read_double(x, aChunk->_f64)))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk++;
				x++;
				break;
			case kCHAR_STRING:
			{
				if(_delimiter == 0)
				{
					if(!(_lineReader.read_string_no_delim(x, _seperator, lBegin, lEnd)))
					{
						throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					}
				}
				else
				{
					if(!(_lineReader.read_string(x, _delimiter, lBegin, lEnd)))
					{
						throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					}
				}
				aChunk->_cp64 = ((SimpleCharContainer*)_relation.getContainer()[_relation.getContainerIndex(i)])->insert(lBegin, lEnd);
				aChunk++;
				x++;
				break;
			}
			case kDATEJD:
				if(!(_lineReader.read_datejd(x, aDate, true, '-')))
				{
					throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				}
				aChunk->_d32 = aDate.julianDay();
				aChunk++;
				x++;
				break;
			case kSTR_SDICT:
			{
				if(_delimiter == 0)
				{
					if(!(_lineReader.read_string_no_delim(x, _seperator, lBegin, lEnd)))
					{
						throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					}
				}
				else
				{
					if(!(_lineReader.read_string(x, _delimiter, lBegin, lEnd)))
					{
						throw IOException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					}
				}
				aChunk->_s32 = ((SimpleDictionary<std::string>*)_relation.getContainer()[_relation.getContainerIndex(i)])->get_idx(std::string(lBegin, lEnd));
				aChunk++;
				x++;
				break;
			}
			default:
				throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
				break;
		}
	}
}

// void BulkLoader::bulk_load_int_chunk(const size_t aSize)
// {
// 	unval_t* lChunk = 0;
// 	for(size_t i = 0; i < aSize; ++i)
// 	{
// 		if(i % _intermediate_buffer.getTuplePerChunk() == 0)
// 		{
// 			lChunk = (unval_t*)malloc(sizeof(unval_t) * _relation.getLogSchema().size() * _intermediate_buffer.getTuplePerChunk());
// 			_intermediate_buffer.addChunkToBuffer(lChunk);
// 		}
// 		for(size_t j = 0; j < _relation.getLogSchema().size(); ++j)
// 		{
// 			switch(_relation.getLogSchema()[j])
// 			{
// 				case kUINT32:
// 					lChunk->_u32 = (uint32_t)rand();
// 					break;
// 				case kINT32:
// 					lChunk->_u32 = (int32_t)rand();
// 					break;
// 				case kFLOAT32:
// 					lChunk->_f32 = (float)rand();
// 					break;
// 				default:
// 					throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
// 					break;
// 			}
// 			++lChunk;
// 		}
// 	}
// 	_intermediate_buffer.setTupleTotal(aSize);
// }

void BulkLoader::bulk_load_int_chunk(const size_t aSize)
{
	unval_t* lChunk = 0;
	for(size_t i = 0; i < aSize; ++i)
	{
		if(i % _intermediate_buffer.getTuplePerChunk() == 0)
		{
			lChunk = (unval_t*)malloc(sizeof(unval_t) * _relation.getLogSchema().size() * _intermediate_buffer.getTuplePerChunk());
			_intermediate_buffer.addChunkToBuffer(lChunk);
		}
		for(size_t j = 0; j < _relation.getLogSchema().size(); ++j)
		{
			switch(_relation.getLogSchema()[j])
			{
				case kUINT32:
					lChunk->_u32 = (uint32_t)i;
					break;
				case kINT32:
					lChunk->_i32 = (int32_t)i;
					break;
				case kFLOAT32:
					lChunk->_f32 = (float)i;
					break;
				default:
					throw SwitchException(__FILE__, __LINE__, __PRETTY_FUNCTION__);
					break;
			}
			++lChunk;
		}
	}
	_intermediate_buffer.setTupleTotal(aSize);
}