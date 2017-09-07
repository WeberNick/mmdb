#include "measure.hh"

#include "immintrin.h"
#include <cstring>
#include <stdlib.h>
#include <stdint.h>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

typedef uint8_t byte;
typedef std::vector<byte*> byte_vpt;
typedef std::vector<double> double_vt;
const size_t G_ChunkSize = 1000000;
const size_t G_TupleSize = 100;
const size_t G_AttrSize = sizeof(uint32_t);
const size_t G_Alignment = 32;
const size_t G_PageSize = 16384;
const size_t G_NumbPages = ((G_ChunkSize * G_TupleSize * G_AttrSize) / G_PageSize) + 1;
const std::string G_Path = "data/";

const double NS = (1000.0L * 1000.0L * 1000.0L);

double secToNanoSec(const double aTime, const double aNumber) 
{
	return (aTime / aNumber) * NS;
}

void printSettings()
{
	std::cout << "Chunk Size:     " << G_ChunkSize << std::endl;
	std::cout << "Tuple Size:     " << G_TupleSize << std::endl;
	std::cout << "Attribute Size: " << G_AttrSize << " bytes" << std::endl;
	std::cout << "Alignment:      " << G_Alignment << " bytes" << std::endl;
	std::cout << "Page Size:      " << G_PageSize << " bytes" << std::endl;
	std::cout << "No. Pages:      " << G_NumbPages << std::endl;
}

void allocateMemory(byte*& aInputChunk, byte*& aOutputChunk)
{
	if(posix_memalign((void**)&aInputChunk, G_Alignment, G_NumbPages * G_PageSize) != 0)
	{
		std::cerr << "An error ocurred while allocating memory for the input chunk" << std::endl;
	}
	if(posix_memalign((void**)&aOutputChunk, G_Alignment, G_NumbPages * G_PageSize) != 0)
	{
		std::cerr << "An error ocurred while allocating memory for the output chunk" << std::endl;
	}
}

void freeMemory(byte*& aInputChunk, byte*& aOutputChunk)
{
	free(aInputChunk);
	aInputChunk = 0;
	free(aOutputChunk);
	aOutputChunk = 0;
}

void fillInputChunkWithData(byte_vpt& aInputTuplePointer)
{
	uint32_t lCounter = 1;
	for(size_t i = 0; i < G_ChunkSize; ++i)
	{
		for(size_t j = 0; j < G_TupleSize; ++j)
		{
			*((uint32_t*)(aInputTuplePointer[i] + (j * G_AttrSize))) = lCounter++;
		}
	}
}

void printToFile(const double_vt& aResult, const std::string aFileName)
{
	std::ofstream out;
	std::string lPathToFile = G_Path + aFileName;
	out.open(lPathToFile.c_str(), std::ios::out | std::ios::trunc);
	if(out.is_open())
	{
		for(size_t i = 0; i < aResult.size(); ++i)
		{
			out << (i+1) << ' ' << std::setprecision(3) << std::fixed << aResult[i] << std::endl;
		}
		out.close();
	}
}

void measure_ManualWrites(const byte_vpt& aInputTuplePointer, const byte_vpt& aOutputTuplePointer)
{
	std::cout << "Manual Writes" << std::endl;
	double_vt lMeasureResults(G_TupleSize, 0);
	
	for(size_t lNoAttr = 0; lNoAttr < G_TupleSize; ++lNoAttr)
	{	
		std::cout << "Attribute " << (lNoAttr+1) << "/" << G_TupleSize << std::endl;
		Measure lMeasure;
		lMeasure.start();
		for(size_t i = 0; i < G_ChunkSize; ++i)
		{
			for(size_t j = 0; j <= lNoAttr; ++j)
			{
				*((uint32_t*)(aOutputTuplePointer[i] + (j * G_AttrSize))) = *((uint32_t*)(aInputTuplePointer[i] + (j * G_AttrSize)));
			}
		}
		lMeasure.stop();
		lMeasureResults[lNoAttr] = secToNanoSec(lMeasure.mTotalTime(), G_ChunkSize);
	}
	printToFile(lMeasureResults, "manual.txt");
}

void measure_NormalWrites(const byte_vpt& aInputTuplePointer, const byte_vpt& aOutputTuplePointer)
{
	std::cout << "Normal Writes" << std::endl;
	double_vt lMeasureResults(G_TupleSize, 0);
	
	for(size_t lNoAttr = 0; lNoAttr < G_TupleSize; ++lNoAttr)
	{	
		std::cout << "Attribute " << (lNoAttr+1) << "/" << G_TupleSize << std::endl;
		const size_t lBytesToCopy = (lNoAttr + 1)  * G_AttrSize;
		Measure lMeasure;
		lMeasure.start();
		for(size_t i = 0; i < G_ChunkSize; ++i)
		{
			memcpy((void*)aOutputTuplePointer[i], (void*)aInputTuplePointer[i], lBytesToCopy);
		}
		lMeasure.stop();
		lMeasureResults[lNoAttr] = secToNanoSec(lMeasure.mTotalTime(), G_ChunkSize);
	}
	printToFile(lMeasureResults, "normal.txt");
}

// __m256i _mm256_load_si256 (__m256i const * mem_addr);

// void _mm256_stream_si256 (__m256i * mem_addr, __m256i a);

void measure_NonTemporalWrites(byte* aInputPointer, byte* aOutputPointer)
{
	std::cout << "Streaming Writes" << std::endl;
	double_vt lMeasureResults(G_TupleSize, 0);
	const size_t lBytesToRead = G_ChunkSize * G_TupleSize * G_AttrSize;
	const size_t lRunsToReadAllBytes = lBytesToRead / (256 / 8);

	std::cout << "Bytes to read: " << lBytesToRead << " bytes" << std::endl;
	std::cout << "Number of " << (256 / 8) << " byte reads needed: " << lRunsToReadAllBytes << std::endl;


	Measure lMeasure;
	lMeasure.start();
	for(size_t i = 0; i < lRunsToReadAllBytes; ++i)
	{	
		_mm256_stream_si256((__m256i *)&aOutputPointer[0], _mm256_load_si256((__m256i *)&aInputPointer[0]));
		aInputPointer += (256 / 8);
		aOutputPointer += (256 / 8);
	}
	lMeasure.stop();
	lMeasureResults[G_TupleSize-1] = secToNanoSec(lMeasure.mTotalTime(), G_ChunkSize);

	printToFile(lMeasureResults, "streaming.txt");
}

// __m256i _mm256_load_si256 (__m256i const * mem_addr);

// void _mm256_stream_si256 (__m256i * mem_addr, __m256i a);

// void measure_NonTemporalWrites(const byte_vpt& aInputTuplePointer, const byte_vpt& aOutputTuplePointer)
// {
// 	double_vt lMeasureResults(G_TupleSize, 0);
// 	const size_t lAttrPer256Bit = (256 / 8) / G_AttrSize;

// 	for(size_t lNoAttr = 0; lNoAttr < G_TupleSize; ++lNoAttr)
// 	{	
// 		for(size_t i = 0; i < G_ChunkSize; ++i)
// 		{
// 			size_t j = 0;
// 			while(j++ < ((lNoAttr / lAttrPer256Bit) + 1 ))
// 			{
// 				_mm256_load_si256(aInputTuplePointer[i])
// 				_mm256_stream_si256();
// 			}
// 		}


// 	}


// 	const size_t lTupleSizeInBytes = G_AttrSize * G_TupleSize;

	
// 	for(size_t lNoAttr = 0; lNoAttr < G_TupleSize; ++lNoAttr)
// 	{	
// 		const size_t lBytesToCopy = (lNoAttr + 1)  * G_AttrSize;
// 		Measure lMeasure;
// 		lMeasure.start();
// 		for(size_t i = 0; i < G_ChunkSize; ++i)
// 		{
// 			const size_t lOffset = i * lTupleSizeInBytes;
// 			memcpy((void*)(aOutputTuplePointer[i] + lOffset), (void*)(aInputTuplePointer[i] + lOffset), lBytesToCopy);
// 		}
// 		lMeasure.stop();
// 		lMeasureResults[lNoAttr] = secToNanoSec(lMeasure.mTotalTime(), G_ChunkSize);
// 	}
// 	printToFile(lMeasureResults, "streaming.txt");
// }

int main()
{
	printSettings();

	byte* lInputChunk;
	byte* lOutputChunk;
	allocateMemory(lInputChunk, lOutputChunk);


	byte_vpt lInputTuplePointer(G_ChunkSize);
	byte_vpt lOutputTuplePointer(G_ChunkSize);

	for(size_t i = 0; i < G_ChunkSize; ++i)
	{
		lInputTuplePointer[i] = lInputChunk + (i * G_TupleSize * G_AttrSize);
		lOutputTuplePointer[i] = lOutputChunk + (i * G_TupleSize * G_AttrSize);
	}

	fillInputChunkWithData(lInputTuplePointer);

	// std::cout << (void*)lInputTuplePointer[0] << std::endl;
	// std::cout << (void*)lInputTuplePointer[1] << std::endl;
	// std::cout << (uint)(lInputTuplePointer[1] - lInputTuplePointer[0]) << std::endl;
	// uint64_t ptr_in_int = (uint64_t)lInputTuplePointer[0];
	// std::cout << ptr_in_int << std::endl;
	// ptr_in_int = (ptr_in_int + 31) & ~(uint64_t)0x1F;
	// std::cout << ptr_in_int << std::endl;
	// uint64_t ptr_in_int2 = (uint64_t)lInputTuplePointer[1];
	// std::cout << ptr_in_int2 << std::endl;
	// ptr_in_int2 = (ptr_in_int2 + 31) & ~(uint64_t)0x1F;
	// std::cout << ptr_in_int2 << std::endl;
	// std::cout << (void*)ptr_in_int << std::endl;
	// std::cout << (void*)ptr_in_int2 << std::endl;


	measure_ManualWrites(lInputTuplePointer, lOutputTuplePointer);

	measure_NormalWrites(lInputTuplePointer, lOutputTuplePointer);

	measure_NonTemporalWrites(lInputChunk, lOutputChunk);

	

	freeMemory(lInputChunk, lOutputChunk);
	return 0;
}