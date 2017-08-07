#ifndef MODES_HH
#define MODES_HH

const char* r_test_updateInt_path = "../results/r_test_updateInt_";
const char* c_test_updateInt_path = "../results/c_test_updateInt_";
const char* r_bulk_load_path = "../results/r_bulk_load_";
const char* c_bulk_load_path = "../results/c_bulk_load_";
const char* r_test_query1_path = "../results/r_test_query1_";
const char* c_test_query1_path = "../results/c_test_query1_";
const char* r_test_update1_path = "../results/r_test_update1_";
const char* c_test_update1_path = "../results/c_test_update1_";
const std::vector<float> selectivity = {105000, 71000, 59500, 51500, 44000, 37000, 29500, 22500, 15000, 8000, 0};
const size_t attrUpdates = 16;
const size_t intAttrNo = 100;
const size_t intChunkSize = 10000000;

#endif