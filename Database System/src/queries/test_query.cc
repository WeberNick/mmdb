#include "test_query.hh"

void row_test_query(NSM_Relation& aRelation, const size_t aVectorizedSize)
{
	for(size_t i = 0; i < RUNS_GLOBAL; ++i)
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
	for(size_t i = 0; i < RUNS_GLOBAL; ++i)
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

void row_test_tpch_projection(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t							lTop;
			row_project_test_query_t						lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<row_project_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Standard[TPCH]");
	}
}

void col_test_tpch_projection(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t							lTop;
			col_project_test_query_t						lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<col_project_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Standard[TPCH]");
	}
}

void row_test_tpch_projection_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t										lTop;
			row_project_opt_switch_test_query_t							lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<row_project_opt_switch_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "OptimizedSwitch[TPCH]");
	}

}

void col_test_tpch_projection_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t										lTop;
			col_project_opt_switch_test_query_t							lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<col_project_opt_switch_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "OptimizedSwitch[TPCH]");
	}
}





void row_test_projection(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t							lTop;
			row_project_test_query_t						lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<row_project_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Standard[BTR]");
	}
}

void col_test_projection(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t							lTop;
			col_project_test_query_t						lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<col_project_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Standard[BTR]");
	}
}

void row_test_projection_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t										lTop;
			row_project_opt_switch_test_query_t							lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<row_project_opt_switch_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "OptimizedSwitch[BTR]");
	}
}

void col_test_projection_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t										lTop;
			col_project_opt_switch_test_query_t							lProject(&lTop, lAttrNoList, aVectorizedSize);
			Scan<col_project_opt_switch_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "OptimizedSwitch[BTR]");
	}
}

void row_test_projection_mat(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t									lTop;
			rowToRow_project_mat_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			Scan<rowToRow_project_mat_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			// rowToCol_project_mat_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			// Scan<rowToCol_project_mat_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Materialized[BTR]r2r");
		// print_projection_result(aNoAttr, measure_int_projection, "Materialized[BTR]r2c");
	}
}

void col_test_projection_mat(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t									lTop;
			colToRow_project_mat_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			Scan<colToRow_project_mat_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			// colToCol_project_mat_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			// Scan<colToCol_project_mat_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "Materialized[BTR]c2r");
		// print_projection_result(aNoAttr, measure_int_projection, "Materialized[BTR]c2c");
	}
}

void row_test_projection_mat_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			row_top_test_query_t												lTop;
			rowToRow_project_mat_opt_switch_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			Scan<rowToRow_project_mat_opt_switch_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			// rowToCol_project_mat_opt_switch_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			// Scan<rowToCol_project_mat_opt_switch_test_query_t, NSM_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "MaterializedOptimizedSwitch[BTR]r2r");
		// print_projection_result(aNoAttr, measure_int_projection, "MaterializedOptimizedSwitch[BTR]r2c");
	}
}

void col_test_projection_mat_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const size_t aVectorizedSize)
{
	double_vt measure_int_projection(aNoAttr, 0);
	uint_vt lAttrNoList;
	
	for(size_t i = 0; i < aNoAttr; ++i)
	{
		std::cout << "Attr. " << (i+1) << "/" << aNoAttr << std::endl;
		lAttrNoList.push_back(i);
		for(size_t j = 0; j < RUNS_GLOBAL; ++j)
		{
			col_top_test_query_t												lTop;
			colToRow_project_mat_opt_switch_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			Scan<colToRow_project_mat_opt_switch_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			// colToCol_project_mat_opt_switch_test_query_t						lProject(&lTop, aRelation, lAttrNoList, aVectorizedSize);
			// Scan<colToCol_project_mat_opt_switch_test_query_t, PAX_Relation> 	lScan(&lProject, aRelation, aVectorizedSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			lScan.run();
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				measure_int_projection[i] += secToNanoSec(lMeasure.mTotalTime(), aNoTuple);
			}

		}
	}
	if(MEASURE_GLOBAL)
	{
		for(size_t i = 0; i < aNoAttr; ++i)
		{
			measure_int_projection[i] /= RUNS_GLOBAL;
		}
		print_projection_result(aNoAttr, measure_int_projection, "MaterializedOptimizedSwitch[BTR]c2r");
		// print_projection_result(aNoAttr, measure_int_projection, "MaterializedOptimizedSwitch[BTR]c2c");
	}
}



// void row_test_update1(NSM_Relation& aRelation, const uint aNoAttr, const bool aMeasure, const size_t aRuns, const char* aPath)
// {
// 	for(size_t x = 1; x <= aNoAttr; ++x)
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

// void col_test_update1(PAX_Relation& aRelation, const uint aNoAttr, const bool aMeasure, const size_t aRuns, const char* aPath)
// {
// 	for(size_t x = 1; x <= aNoAttr; ++x)
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