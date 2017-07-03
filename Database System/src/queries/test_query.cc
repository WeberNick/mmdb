#include "test_query.hh"

void row_test_query(NSM_Relation& aRelation, const size_t aVectorizedSize)
{
	for(size_t i = 0; i < RUNS; ++i)
	{
		TestPred::Parameter lPara = {5, 100000.99};
		TestPred lPredicate(lPara);
		const uint_vt lAttrNoList = {0,1,2,3,4,5};

		row_top_test_query_t		lTop;
		row_project_test_query_t	lProject(&lTop, lAttrNoList, aVectorizedSize);
		row_select_test_query_t 	lSelect(&lProject, lPredicate, aVectorizedSize);
		row_scan_test_query_t 		lScan(&lSelect, aRelation, aVectorizedSize);


		lScan.run();
	}

}

void col_test_query(PAX_Relation& aRelation, const size_t aVectorizedSize)
{
	for(size_t i = 0; i < RUNS; ++i)
	{
		TestPred::Parameter lPara = {5, 100000.99};
		TestPred lPredicate(lPara);
		const uint_vt lAttrNoList = {0,1,2,3,4,5};

		col_top_test_query_t		lTop;
		col_project_test_query_t	lProject(&lTop, lAttrNoList);
		col_select_test_query_t 	lSelect(&lProject, lPredicate);
		col_scan_test_query_t 		lScan(&lSelect, aRelation);


		lScan.run();
	}
}



	// double avg = 0;

	// for(size_t i = 1; i <= aAttrNo; ++i)
	// {
	// 	for(size_t j = 0; j < RUNS; ++j)
	// 	{
	// 		Measure lMeasure;
	// 		if(aMeasure)
	// 		{
	// 			lMeasure.start();
	// 		}
	// 		update_int(_relation, i);
	// 		lMeasure.stop();
	// 		avg += lMeasure.mTotalTime();
	// 	}
	// 	avg /= aRuns;
	// 	print_tu1_result(aPath, i, avg);
	// }

// for(size_t j = 0; j < 8; ++j)
// 		{
// 			BulkLoader bl(lFileNames[j].c_str(), _relation_vector[j], aDelimiter, aSeperator, aBufferSize);
// 			Measure lMeasure;
// 			if(MEASURE)
// 			{
// 				lMeasure.start();
// 			}
// 			try
// 			{	
// 				bl.bulk_load();
// 			}
// 			catch(std::exception& ex)
// 			{
// 				std::cerr << "ERROR: " << ex.what() << std::endl;
// 			}
// 			lMeasure.stop();
// 			if(MEASURE)
// 			{
// 				bulk_load_sumPerRel[j] += lMeasure.mTotalTime();
// 			}


// 			if(_flag)
// 			{
// 				BulkInsertSP bi;
// 				NSM_Relation* p = (NSM_Relation*)&_relation_vector[j];
// 				if(MEASURE)
// 				{
// 					lMeasure.start();
// 				}
// 				bi.bulk_insert(bl, p);
// 			}
// 			else
// 			{
// 				BulkInsertPAX bi;
// 				PAX_Relation* p = (PAX_Relation*)&_relation_vector[j];
// 				if(MEASURE)
// 				{
// 					lMeasure.start();
// 				}
// 				bi.bulk_insert(bl, p);
// 			}
// 			lMeasure.stop();
// 			if(MEASURE)
// 			{
// 				bulk_insert_sumPerRel[j] += lMeasure.mTotalTime();
// 			}
// 		}
// 	}
// 	if(MEASURE)
// 	{
// 		string_vt relation_names = {"Customer", "Lineitem", "Nation", "Orders", "Part", "PartSupp", "Region", "Supplier", "Sum"};
//   		double sum = 0;
//   		for(size_t i = 0; i < 8; ++i)
//   		{
//   			bulk_load_sumPerRel[i] /= RUNS;
//   			sum += bulk_load_sumPerRel[i];
//   		}
//   		bulk_load_sumPerRel.push_back(sum);
//   		print_bulk_load_insert_result(relation_names, bulk_load_sumPerRel, true);

//   		sum = 0;
//   		for(size_t i = 0; i < 8; ++i)
//   		{
//   			bulk_insert_sumPerRel[i] /=  RUNS;
//   			sum += bulk_insert_sumPerRel[i];
//   		}
//   		bulk_insert_sumPerRel.push_back(sum);
//   		print_bulk_load_insert_result(relation_names, bulk_insert_sumPerRel, false);
// 	}

void row_test_projection(NSM_Relation& aRelation, const size_t aAttrNo)
{
	double_vt measure_int_projection(aAttrNo, 0);
	
	for(size_t i = 0; i < aAttrNo; ++i)
	{
		uint_vt lAttrNoList;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS; ++j)
		{
			row_top_test_query_t		lTop;
			row_project_test_query_t	lProject(&lTop, lAttrNoList);
			Scan<row_project_test_query_t, NSM_Relation> lScan(&lProject, aRelation);
			Measure lMeasure;
			if(MEASURE)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE)
			{
				measure_int_projection[i] += (lMeasure.mTotalTime() * 1000);
			}

		}
	}
	if(MEASURE)
	{
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			measure_int_projection[i] /= RUNS;
		}
		print_int_projection_result(aAttrNo, measure_int_projection);
	}
}

void col_test_projection(PAX_Relation& aRelation, const size_t aAttrNo)
{
	double_vt measure_int_projection(aAttrNo, 0);
	
	for(size_t i = 0; i < aAttrNo; ++i)
	{
		uint_vt lAttrNoList;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS; ++j)
		{
			col_top_test_query_t		lTop;
			col_project_test_query_t	lProject(&lTop, lAttrNoList);
			Scan<col_project_test_query_t, PAX_Relation> lScan(&lProject, aRelation);
			Measure lMeasure;
			if(MEASURE)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE)
			{
				measure_int_projection[i] += (lMeasure.mTotalTime() * 1000);
			}

		}
	}
	if(MEASURE)
	{
		for(size_t i = 0; i < aAttrNo; ++i)
		{
			measure_int_projection[i] /= RUNS;
		}
		print_int_projection_result(aAttrNo, measure_int_projection);
	}
}



// void row_test_update1(NSM_Relation& aRelation, const uint aAttrNo, const bool aMeasure, const size_t aRuns, const char* aPath)
// {
// 	for(size_t x = 1; x <= aAttrNo; ++x)
// 	{
// 		double avg = 0;
// 		for(size_t j = 0; j < aRuns; ++j)
// 		{
// 			Measure lMeasure;
// 			if(aMeasure)
// 			{
// 				lMeasure.start();
// 			}
// 			switch(x)	
// 			{
// 				case 1:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update1(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 2:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update2(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 3:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update3(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 4:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update4(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 5:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update5(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 6:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update6(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 7:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update7(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 8:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update8(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 9:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update9(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 10:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update10(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 11:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update11(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 12:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update12(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 13:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update13(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 14:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update14(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 15:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update15(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 16:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update16(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				default:
// 					break;
// 			}
// 			lMeasure.stop();
// 			avg += lMeasure.mTotalTime();
// 		}
// 		if(aMeasure)
// 		{
// 			avg /= aRuns;
// 			print_tu1_result(aPath, x, avg);
// 		}
// 	}
// }

// void col_test_update1(PAX_Relation& aRelation, const uint aAttrNo, const bool aMeasure, const size_t aRuns, const char* aPath)
// {
// 	for(size_t x = 1; x <= aAttrNo; ++x)
// 	{
// 		double avg = 0;
// 		for(size_t j = 0; j < aRuns; ++j)
// 		{
// 			Measure lMeasure;
// 			if(aMeasure)
// 			{
// 				lMeasure.start();
// 			}
// 			switch(x)	
// 			{
// 				case 1:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update1(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 2:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update2(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 3:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update3(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 4:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update4(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 5:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update5(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 6:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update6(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 7:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update7(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 8:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update8(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 9:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update9(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 10:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update10(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 11:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update11(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 12:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update12(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 13:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update13(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 14:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update14(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 15:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update15(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				case 16:
// 					for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
// 						update16(aRelation, aRelation.getSegment().getPage(i));
// 					break;
// 				default:
// 					break;
// 			}
// 			lMeasure.stop();
// 			avg += lMeasure.mTotalTime();
// 		}
// 		if(aMeasure)
// 		{
// 			avg /= aRuns;
// 			print_tu1_result(aPath, x, avg);
// 		}
// 	}
// }

// void col_test_query1(PAX_Relation& aRelation, const uint aSize, const std::vector<float>& aSelectivity, const bool aMeasure, const size_t aRuns, const char* aPath)
// {
// 	for(size_t i = 0; i < aSelectivity.size(); ++i)
// 	{
// 		params_test_query param = {5, aSelectivity[i]};
// 		PAX_Tuple* aTuple = (PAX_Tuple*)malloc((sizeof(PAX_Tuple) + (sizeof(uint32_t) * (aSize * 2 - 1))));
		
// 		double avg = 0;

// 		for(size_t j = 0; j < aRuns; ++j)
// 		{
// 			aTuple->_size = aSize;
// 			col_top_test_query_t lTop(aTuple, aRelation);
// 			col_scan_test_query_t lScan(&lTop, aTuple, param, aRelation);

// 			Measure lMeasure;
// 			if(aMeasure)
// 			{
// 				lMeasure.start();
// 			}
// 			lScan.run();
// 			lMeasure.stop();
// 			avg += lMeasure.mTotalTime();
// 		}
// 		if(aMeasure)
// 		{
// 			double sel_p = (((aTuple->_rid) / 6007222.0) * 100);
// 			int sel = (int)round(sel_p);
// 			avg /= aRuns;
// 			print_tq1_result(aPath, sel, avg);
// 		}
// 		free(aTuple);
// 	}	
// }