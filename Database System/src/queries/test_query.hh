#ifndef TEST_QUERY_HH
#define TEST_QUERY_HH

#include "infra/moer/measure.hh"
#include "infra/webe/print_helper.hh"
#include "infra/webe/types.hh"
#include "infra/webe/common.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include "ops/top.hh"
#include "ops/project.hh"
#include "ops/select.hh"
#include "ops/scan.hh"


#include "test_pred.hh"


#include <stdlib.h>
#include <fstream>
#include <vector>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>  

typedef Top<false>													row_top_test_query_t;
typedef Project<row_top_test_query_t>								row_project_test_query_t;
typedef Select<row_project_test_query_t, TestPred, NSM_Relation> 	row_select_test_query_t;
typedef Scan<row_select_test_query_t, NSM_Relation> 				row_scan_test_query_t;

typedef Top<false>													col_top_test_query_t;
typedef Project<col_top_test_query_t>								col_project_test_query_t;
typedef Select<col_project_test_query_t, TestPred, PAX_Relation>	col_select_test_query_t;
typedef Scan<col_select_test_query_t, PAX_Relation> 				col_scan_test_query_t;

void row_test_query(NSM_Relation& aRelation, const size_t aVectorizedSize);

void col_test_query(PAX_Relation& aRelation, const size_t aVectorizedSize);

void row_test_projection(NSM_Relation& aRelation, const size_t aNoAttr);

void col_test_projection(PAX_Relation& aRelation, const size_t aNoAttr);


#endif