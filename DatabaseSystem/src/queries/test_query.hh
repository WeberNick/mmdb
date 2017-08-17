#ifndef TEST_QUERY_HH
#define TEST_QUERY_HH

#include "infra/moer/measure.hh"
#include "infra/moer/random_permutation.hh"
#include "infra/webe/print_helper.hh"
#include "infra/webe/types.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"
#include "ops/top.hh"
#include "ops/project.hh"
#include "ops/project_mat.hh"
#include "ops/project_optimized_switch.hh"
#include "ops/project_mat_optimized_switch.hh"
#include "ops/select.hh"
#include "ops/scan.hh"

#include "ops/scan_select.hh"

#include "test_pred.hh"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>

struct query_infra_t
{
	const size_t			_vectorizedSize;
	const size_t 			_runs;
	const bool				_measure;
	const print_infra_t&	_print_infra;

	const size_t 			vectorizedSize() const { return _vectorizedSize; }
	const size_t 			runs() const { return _runs; }
	const bool 				measure() const { return _measure; }
	const print_infra_t& 	printInfra() const { return _print_infra; }
};

typedef Top<false>													row_top_test_query_t;
typedef Project<row_top_test_query_t>								row_project_test_query_t;
typedef Select<row_top_test_query_t, TestPred, NSM_Relation> 		row_select_test_query_t;
typedef Scan<row_select_test_query_t, NSM_Relation> 				row_scan_test_query_t;

typedef Top<false>													col_top_test_query_t;
typedef Project<col_top_test_query_t>								col_project_test_query_t;
typedef Select<col_top_test_query_t, TestPred, PAX_Relation>		col_select_test_query_t;
typedef Scan<col_select_test_query_t, PAX_Relation> 				col_scan_test_query_t;

typedef ScanAndSelect<row_top_test_query_t, TestPred, NSM_Relation>	row_scan_select_test_query_t;
typedef ScanAndSelect<col_top_test_query_t, TestPred, PAX_Relation>	col_scan_select_test_query_t;

typedef ProjectMaterialized<row_top_test_query_t, NSM_Relation, NSM_Relation> rowToRow_project_mat_test_query_t;
typedef ProjectMaterialized<col_top_test_query_t, NSM_Relation, PAX_Relation> rowToCol_project_mat_test_query_t;
typedef ProjectMaterialized<row_top_test_query_t, PAX_Relation, NSM_Relation> colToRow_project_mat_test_query_t;
typedef ProjectMaterialized<col_top_test_query_t, PAX_Relation, PAX_Relation> colToCol_project_mat_test_query_t;

typedef ProjectOptimizedSwitch<row_top_test_query_t>				row_project_opt_switch_test_query_t;
typedef ProjectOptimizedSwitch<col_top_test_query_t>				col_project_opt_switch_test_query_t;

typedef ProjectMaterializedOptimizedSwitch<row_top_test_query_t, NSM_Relation, NSM_Relation> rowToRow_project_mat_opt_switch_test_query_t;
typedef ProjectMaterializedOptimizedSwitch<col_top_test_query_t, NSM_Relation, PAX_Relation> rowToCol_project_mat_opt_switch_test_query_t;
typedef ProjectMaterializedOptimizedSwitch<row_top_test_query_t, PAX_Relation, NSM_Relation> colToRow_project_mat_opt_switch_test_query_t;
typedef ProjectMaterializedOptimizedSwitch<col_top_test_query_t, PAX_Relation, PAX_Relation> colToCol_project_mat_opt_switch_test_query_t;

void row_test_tpch_selection(NSM_Relation& aRelation, const float_vt& aSelectivities, const query_infra_t& aQueryInfra);

void col_test_tpch_selection(PAX_Relation& aRelation, const float_vt& aSelectivities, const query_infra_t& aQueryInfra);

void row_test_tpch_projection(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_tpch_projection(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void row_test_tpch_projection_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_tpch_projection_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void row_test_projection(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_projection(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void row_test_projection_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_projection_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void row_test_projection_mat(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_projection_mat(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void row_test_projection_mat_optimized_switch(NSM_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void col_test_projection_mat_optimized_switch(PAX_Relation& aRelation, const size_t aNoTuple, const size_t aNoAttr, const query_infra_t& aQueryInfra);

void random_permutation(uint_vt& aAttrList, const size_t aSize);


#endif