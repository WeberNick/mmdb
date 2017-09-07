/**
 *	@file 	scan.hh
 *	@author	Nick Weber (nickwebe@pi3.informatik.uni-mannheim.de)
 *	@brief	Scan operator of the physical algebra, used to scan the database
 *	@bugs 	Currently no bugs known
 *	@todos	PAX record representation with the TID concept may change in the future
 *
 *	@section DESCRIPTION
 *	This template class implements the physical algebra operator 'Scan'.
 *	The scan operator can be used to scan the database and depending on
 *	the storage layout in use (NSM or PAX), different approaches of 
 *	representing a scanned record are implemented.
 *
 *	First the description for the storage layout independent part:
 *
 *	General	At the start, the operator is initialized by calling the 'init'
 *			procedure. This allocates memory for the output of the scan and
 *			initializes certain variables neeeded by the scan operator.
 *			The database scan is started with a call to 'startScan'.
 *			The scan operator uses an associated relation object to get 
 *			all pages which store records of the relation. It then uses 
 *			a storage layout dependent page interpreter to scan each page
 *			for all records stored on the page. This can be done by using
 *			functions provided by the page interpreter. These functions
 *			allow to operate on the memory pages without detailed knowledge
 *			of the underlying page format/layout. For each record on the
 *			page data to clearly identify the record is collected. This 
 *			data, needed to identify the record, is dependent of the
 *			storage layout (e.g., in row stores a single pointer to the
 *			beginning of each record is enough to reconstruct the record).
 *			The data is stored in an output data structure. Each operator
 *			in the chain has an associated operator which will follow next
 *			in the chain (the last operator, 'Top', is the only exception).
 *			Each associated next operator has a procedure called 'step'.
 *			By calling 'step' on the next operator (and passing the output
 *			data structure as parameter) in the chain, output from one 
 *			operator can be passed to the next operator. One way to hand 
 *			over the data to the next operator is to call the 'step' 
 *			procedure for each scanned record (one tuple at a time).
 *			Another, and hopefully more efficient, way is to fill the output
 *			data structure with a user defined number of records before
 *			passing the data to the next operator (this is called vectorized
 *			processing). Therefore, the next operator can process a chunk
 *			of records instead of one tuple at a time. After all records
 *			have been scanned and passed to the next operator, the scan
 *			operator calls the 'finish' procedure, which in turn calls the
 *			'finish' procedure of all following operators. In the 'finish'
 *			procedure, all allocated memory by the operator is deallocated.
 *
 *	
 *	The description for the NSM specific part:		
 *			
 *	NSM		In NSM, a record on a page can be scanned by calling 'getRecord' 
 *			on the page interpreter (@see PageInterpreterSP) associated with
 *			this page. This function uses the slotted page's offsets to get 
 *			a pointer to each record (stored in the slots). These pointer are 
 *			then returned and stored in the output data structure. The next
 *			operator can directly access a record by using the pointer to
 *			this record.
 *
 *
 *	The description for the PAX specific part:	
 *	
 *	PAX		In PAX, a record can not be clearly identified with a single 
 *			pointer. Each attribute of a record is stored on a mini page,
 *			associated with this attribute (i.e., all attribute values of
 *			a specific attribute are stored together on a mini page).
 *			To reconstruct a record on a pax page, the page number and the
 *			record number is needed. These two identifiers are called TID
 *			(or Tuple IDentifier) together. With the page id (pid) one can
 *			access the memory page and with the record id (rid) one can
 *			combine all attributes of this record on each mini page to
 *			reconstruct the record. So instead of storing a single pointer
 *			in the output data structure, the TID is stored.
 */
#ifndef SCAN_HH
#define SCAN_HH

#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

#include <vector>

template<typename T_Consumer, typename T_Relation>
class Scan
{
	public:
		/* constructor for the scan operator with a vectorized size */
		Scan(T_Consumer* aConsumer, T_Relation& aRelation, const size_t aVectorizedSize = 1);

	public:
		/**
		 *	@brief	This procedure is called from the outside in order to start the scan operator
		 */
		void run();

	private:
		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the initialization
		 */
		void init();

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the scanning
		 *
		 *	@param 	aRelation  a NSM relation whichs records shall be scanned
		 */
		void startScan(NSM_Relation& aRelation);

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to start the scanning
		 *
		 *	@param 	aRelation  a PAX relation whichs records shall be scanned
		 */
		void startScan(PAX_Relation& aRelation);

		/**
		 *	@brief	This procedure is called by the 'run()' procedure to finish the scanning
		 */
		void finish();

	private:
		/* The associated next operator */
		T_Consumer* _nextOp;
		/* The associated relation to scan */
		T_Relation& _relation;
		/* The size for the vectorized processing */
		const size_t _vectorizedSize;
		/* The vector containing the output data structure */
		unval_vt _tuple;
		/* Index to the output data structure within the vector '_tuple' */
		size_t _indexNo;
		/* Output data structure of the scan operator */
		unval_pt _output;

};

template<typename T_Consumer, typename T_Relation>
Scan<T_Consumer, T_Relation>::Scan(
									T_Consumer* aConsOp,
									T_Relation& aRelation,
									size_t aVectorizedSize)
	: _nextOp(aConsOp), _relation(aRelation), _vectorizedSize(aVectorizedSize), _tuple(), _indexNo(), _output()
{}

template<typename T_Consumer, typename T_Relation>
void Scan<T_Consumer, T_Relation>::run()
{
	init();
	startScan(_relation);
	finish();
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::init() 
{
	_indexNo = _tuple.size();
	_tuple.push_back(unval_t());
	_tuple[_indexNo]._unval_pt = new unval_t[_vectorizedSize];
	_output = _tuple[_indexNo]._unval_pt;
	_nextOp->init(_tuple);
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::finish() 
{
	_nextOp->finish(_tuple);
	delete[] _output;
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(NSM_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterSP lPageInterpreter;
	const size_t lNumberOfPages = aRelation.getSegment().getNoPages();
	while(lPageNo < lNumberOfPages)		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		const size_t lNumberOfRecords = lPageInterpreter.noRecords();
		while(lRecordNo < lNumberOfRecords)			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				_output[lCounter]._pointer = lPageInterpreter.getRecord(lRecordNo);
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_tuple, aRelation, lCounter, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_nextOp->step(_tuple, aRelation, lCounter, true);
			lCounter = 0;
		}
		lRecordNo = 0;
	}
}

template<typename T_Consumer, typename T_Relation>
void
Scan<T_Consumer, T_Relation>::startScan(PAX_Relation& aRelation)
{
	size_t lPageNo = 0;
	size_t lRecordNo = 0;
	size_t lCounter = 0;
	PageInterpreterPAX lPageInterpreter;
	const size_t lNumberOfPages = aRelation.getSegment().getNoPages();
	while(lPageNo < lNumberOfPages)		//Scan all pages...
	{
		lPageInterpreter.attach(aRelation.getSegment().getPage(lPageNo));
		const size_t lNumberOfRecords = lPageInterpreter.noRecords();
		while(lRecordNo < lNumberOfRecords)			//...and all records...
		{
			if(lCounter < _vectorizedSize)		//if the vectorized buffer is not full, insert record pointer
			{
				_output[lCounter]._tid[0] = lPageNo;
				_output[lCounter]._tid[1] = lRecordNo;
				++lCounter;
				++lRecordNo;
			}
			else												//if the vectorized buffer is full hand it over to the next operator
			{
				_nextOp->step(_tuple, aRelation, lCounter, false);
				lCounter = 0;
			}
		}
		if(!(++lPageNo < aRelation.getSegment().getNoPages()))	//if the previous page was the last page, hand over the vectorized buffer with the remaining record pointer
		{
			_nextOp->step(_tuple, aRelation, lCounter, true);
		}
		lRecordNo = 0;
	}
}


#endif