#include "tpc_h.hh"

template<typename T_Relation>
TPC_H<T_Relation>::TPC_H(bool aFlag) : _flag(aFlag), _relation_vector()
{}

template TPC_H<NSM_Relation>::TPC_H(bool aFlag);
template TPC_H<PAX_Relation>::TPC_H(bool aFlag);

template<typename T_Relation>
void TPC_H<T_Relation>::init(std::string aScaleFactor, char aDelimiter, char aSeperator, size_t aBufferSize)
{
	std::string lPath = "../tables/tables_" + aScaleFactor + "/";
	double_vt bulk_load_sumPerRel(8, 0);
	double_vt bulk_insert_sumPerRel(8, 0);
	for(size_t i = 0; i < RUNS_GLOBAL; ++i)
	{
		MemoryManager::freeAll();
		_relation_vector.clear();
		_relation_vector.resize(8);
		string_vt lFileNames(8, lPath);

		initCustomerRelation(_relation_vector[0], lFileNames[0]);
		initLineitemRelation(_relation_vector[1], lFileNames[1]);
		initNationRelation(_relation_vector[2], lFileNames[2]);
		initOrdersRelation(_relation_vector[3], lFileNames[3]);
		initPartRelation(_relation_vector[4], lFileNames[4]);
		initPartsuppRelation(_relation_vector[5], lFileNames[5]);
		initRegionRelation(_relation_vector[6], lFileNames[6]);
		initSupplierRelation(_relation_vector[7], lFileNames[7]);

		for(size_t j = 0; j < 8; ++j)
		{
			BulkLoader bl(lFileNames[j].c_str(), _relation_vector[j], aDelimiter, aSeperator, aBufferSize);
			Measure lMeasure;
			if(MEASURE_GLOBAL)
			{
				lMeasure.start();
			}
			try
			{	
				bl.bulk_load();
			}
			catch(std::exception& ex)
			{
				std::cerr << "ERROR: " << ex.what() << std::endl;
			}
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				bulk_load_sumPerRel[j] += lMeasure.mTotalTime();
			}


			if(_flag)
			{
				BulkInsertSP bi;
				NSM_Relation* p = (NSM_Relation*)&_relation_vector[j];
				if(MEASURE_GLOBAL)
				{
					lMeasure.start();
				}
				bi.bulk_insert(bl, p);
			}
			else
			{
				BulkInsertPAX bi;
				PAX_Relation* p = (PAX_Relation*)&_relation_vector[j];
				if(MEASURE_GLOBAL)
				{
					lMeasure.start();
				}
				bi.bulk_insert(bl, p);
			}
			lMeasure.stop();
			if(MEASURE_GLOBAL)
			{
				bulk_insert_sumPerRel[j] += lMeasure.mTotalTime();
			}
		}
	}
	if(MEASURE_GLOBAL)
	{
		string_vt relation_names = {"Customer", "Lineitem", "Nation", "Orders", "Part", "PartSupp", "Region", "Supplier", "Sum"};
  		double sum = 0;
  		for(size_t i = 0; i < 8; ++i)
  		{
  			bulk_load_sumPerRel[i] /= RUNS_GLOBAL;
  			sum += bulk_load_sumPerRel[i];
  		}
  		bulk_load_sumPerRel.push_back(sum);
  		print_bulk_load_insert_result(relation_names, bulk_load_sumPerRel, true);

  		sum = 0;
  		for(size_t i = 0; i < 8; ++i)
  		{
  			bulk_insert_sumPerRel[i] /=  RUNS_GLOBAL;
  			sum += bulk_insert_sumPerRel[i];
  		}
  		bulk_insert_sumPerRel.push_back(sum);
  		print_bulk_load_insert_result(relation_names, bulk_insert_sumPerRel, false);
	}
}

template void TPC_H<NSM_Relation>::init(std::string aScaleFactor, char aDelimiter, char aSeperator, size_t aBufferSize);
template void TPC_H<PAX_Relation>::init(std::string aScaleFactor, char aDelimiter, char aSeperator, size_t aBufferSize);

template<typename T_Relation>
typename TPC_H<T_Relation>::relation_vt& TPC_H<T_Relation>::getTPC_H_Relations()
{
	return _relation_vector;
}

template typename TPC_H<NSM_Relation>::relation_vt& TPC_H<NSM_Relation>::getTPC_H_Relations();
template typename TPC_H<PAX_Relation>::relation_vt& TPC_H<PAX_Relation>::getTPC_H_Relations();