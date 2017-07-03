#include "infra/webe/types.hh"
#include "infra/webe/exception.hh"
#include "infra/moer/measure.hh"
#include "memory/memory_manager.hh"
#include "relation/relation.hh"
#include "relation/nsm_relation.hh"
#include "relation/pax_relation.hh"

#include <string>
#include <vector>

// typedef void func_t_nsm(NSM_Relation&, std::string&);
// typedef func_t_nsm* initRelationNSM;
// typedef std::vector<initRelationNSM> initRelationNSM_vt;

// typedef void func_t_pax(PAX_Relation&, std::string&);
// typedef func_t_pax* initRelationPAX;
// typedef std::vector<initRelationPAX> initRelationPAX_vt;

// initRelationNSM_vt functionsNSM = {	
// 												initCustomerRelation
// 												,initLineitemRelation
// 												,initNationRelation
// 												,initOrdersRelation
// 												,initPartRelation
// 												,initPartsuppRelation
// 												,initRegionRelation
// 												,initSupplierRelation
// 																		};

template<typename T_Relation>
void initCustomerRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "customer.tbl";
	std::string relName("CUSTOMER");
	schema_vt logSchema = {kUINT32, kCHAR_STRING, kCHAR_STRING, kUINT32, kCHAR_STRING, kFLOAT32, kSTR_SDICT, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"C_CUSTKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"C_NAME", kCHAR_STRING, 1, kSimpleCharContainer, 0});
	attrDesc.push_back({"C_ADDRESS", kCHAR_STRING, 2, kSimpleCharContainer, 1});
	attrDesc.push_back({"C_NATIONKEY", kUINT32, 3, kNoContainer, -1});
	attrDesc.push_back({"C_PHONE", kCHAR_STRING, 4, kSimpleCharContainer, 2});
	attrDesc.push_back({"C_ACCTBAL", kFLOAT32, 5, kNoContainer, -1});
	attrDesc.push_back({"C_MKTSEGMENT", kSTR_SDICT, 6, kSimpleDictionary, 3});
	attrDesc.push_back({"C_COMMENT", kCHAR_STRING, 7, kSimpleCharContainer, 4});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleDictionary, 6});
	contDesc.push_back({kSimpleCharContainer, 1024});
	string_vt c_mktsegment_dict_entries;
	c_mktsegment_dict_entries.push_back("AUTOMOBILE");
	c_mktsegment_dict_entries.push_back("BUILDING");
	c_mktsegment_dict_entries.push_back("FURNITURE");
	c_mktsegment_dict_entries.push_back("MACHINERY");
	c_mktsegment_dict_entries.push_back("HOUSEHOLD");
	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&c_mktsegment_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initLineitemRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "lineitem.tbl";
	std::string relName("LINEITEM");
	schema_vt logSchema = {kUINT32, kUINT32, kUINT32, kUINT32, kFLOAT32, kFLOAT32, kFLOAT32, kFLOAT32, kCHAR, kCHAR, kDATEJD, kDATEJD, kDATEJD, kSTR_SDICT, kSTR_SDICT, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"L_ORDERKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"L_PARTKEY", kUINT32, 1, kNoContainer, -1});
	attrDesc.push_back({"L_SUPPKEY", kUINT32, 2, kNoContainer, -1});
	attrDesc.push_back({"L_LINENUMBER", kUINT32, 3, kNoContainer, -1});
	attrDesc.push_back({"L_QUANTITY", kFLOAT32, 4, kNoContainer, -1});
	attrDesc.push_back({"L_EXTENDEDPRICE", kFLOAT32, 5, kNoContainer, -1});
	attrDesc.push_back({"L_DISCOUNT", kFLOAT32, 6, kNoContainer, -1});
	attrDesc.push_back({"L_TAX", kFLOAT32, 7, kNoContainer, -1});
	attrDesc.push_back({"L_RETURNFLAG", kCHAR, 8, kNoContainer, -1});					
	attrDesc.push_back({"L_LINESTATUS", kCHAR, 9, kNoContainer, -1});
	attrDesc.push_back({"L_SHIPDATE", kDATEJD, 10, kNoContainer, -1});
	attrDesc.push_back({"L_COMMITDATE", kDATEJD, 11, kNoContainer, -1});
	attrDesc.push_back({"L_RECEIPTDATE", kDATEJD, 12, kNoContainer, -1});
	attrDesc.push_back({"L_SHIPINSTRUCT", kSTR_SDICT, 13, kSimpleDictionary, 0});
	attrDesc.push_back({"L_SHIPMODE", kSTR_SDICT, 14, kSimpleDictionary, 1});
	attrDesc.push_back({"L_COMMENT", kCHAR_STRING, 15, kSimpleCharContainer, 2});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleDictionary, 13});
	contDesc.push_back({kSimpleDictionary, 14});
	contDesc.push_back({kSimpleCharContainer, 1024});
	string_vt l_shipinstruct_dict_entries;
	l_shipinstruct_dict_entries.push_back("DELIVER IN PERSON");
	l_shipinstruct_dict_entries.push_back("COLLECT COD");
	l_shipinstruct_dict_entries.push_back("NONE");
	l_shipinstruct_dict_entries.push_back("TAKE BACK RETURN");
	string_vt l_shipmode_dict_entries;
	l_shipmode_dict_entries.push_back("AIR");
	l_shipmode_dict_entries.push_back("REG AIR");
	l_shipmode_dict_entries.push_back("RAIL");
	l_shipmode_dict_entries.push_back("SHIP");
	l_shipmode_dict_entries.push_back("TRUCK");
	l_shipmode_dict_entries.push_back("MAIL");
	l_shipmode_dict_entries.push_back("FOB");
	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&l_shipinstruct_dict_entries);
	dict_entries_pointer.push_back((byte*)&l_shipmode_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initNationRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "nation.tbl";
	std::string relName("NATION");
	schema_vt logSchema = {kUINT32, kSTR_SDICT, kUINT32, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"N_NATIONKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"N_NAME", kUINT32, 1, kSimpleDictionary, 0});
	attrDesc.push_back({"N_REGIONKEY", kUINT32, 2, kNoContainer, -1});
	attrDesc.push_back({"N_COMMENT", kCHAR_STRING, 3, kSimpleCharContainer, 1});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleDictionary, 1});
	contDesc.push_back({kSimpleCharContainer, 1024});
	string_vt n_name_dict_entries;
	std::string nations [25]={	"ALGERIA","ARGENTINA","BRAZIL",
								"CANADA","EGYPT","ETHIOPIA",
								"FRANCE","GERMANY","INDIA",
								"INDONESIA","IRAN","IRAQ",
								"JAPAN","JORDAN","KENYA",
								"MOROCCO","MOZAMBIQUE","PERU",
								"CHINA","ROMANIA","SAUDI ARABIA",
								"VIETNAM","RUSSIA","UNITED KINGDOM",
								"UNITED STATES"};
	for(uint i = 0; i < 25; ++i)
	{
		n_name_dict_entries.push_back(nations[i]);
	}
	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&n_name_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initOrdersRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "orders.tbl";
	std::string relName("ORDERS");
	schema_vt logSchema = {kUINT32, kUINT32, kCHAR, kFLOAT32, kDATEJD, kSTR_SDICT, kCHAR_STRING, kUINT32, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"O_ORDERKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"O_CUSTKEY", kUINT32, 1, kNoContainer, -1});
	attrDesc.push_back({"O_ORDERSTATUS", kCHAR, 2, kNoContainer, -1});
	attrDesc.push_back({"O_TOTALPRICE", kFLOAT32, 3, kNoContainer, -1});
	attrDesc.push_back({"O_ORDERDATE", kDATEJD, 4, kNoContainer, -1});
	attrDesc.push_back({"O_ORDERPRIORITY", kSTR_SDICT, 5, kSimpleDictionary, 0});
	attrDesc.push_back({"O_CLERK", kCHAR_STRING, 6, kSimpleCharContainer, 1});
	attrDesc.push_back({"O_SHIPPRIORITY", kUINT32, 7, kNoContainer, -1});
	attrDesc.push_back({"O_COMMENT", kCHAR_STRING, 8, kSimpleCharContainer, 2});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleDictionary, 5});
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	string_vt o_orderpriority_dict_entries;
	o_orderpriority_dict_entries.push_back("1-URGENT");
	o_orderpriority_dict_entries.push_back("2-HIGH");
	o_orderpriority_dict_entries.push_back("3-MEDIUM");
	o_orderpriority_dict_entries.push_back("4-NOT SPECIFIED");
	o_orderpriority_dict_entries.push_back("5-LOW");
	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&o_orderpriority_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initPartRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "part.tbl";
	std::string relName("PART");
	schema_vt logSchema = {kUINT32, kCHAR_STRING, kSTR_SDICT, kSTR_SDICT, kSTR_SDICT, kUINT32, kSTR_SDICT, kFLOAT32, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"P_PARTKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"P_NAME", kCHAR_STRING, 1, kSimpleCharContainer, 0});
	attrDesc.push_back({"P_MFGR", kSTR_SDICT, 2, kSimpleDictionary, 1});
	attrDesc.push_back({"P_BRAND", kSTR_SDICT, 3, kSimpleDictionary, 2});
	attrDesc.push_back({"P_TYPE", kSTR_SDICT, 4, kSimpleDictionary, 3});
	attrDesc.push_back({"P_SIZE", kUINT32, 5, kNoContainer, -1});
	attrDesc.push_back({"P_CONTAINER", kSTR_SDICT, 6, kSimpleDictionary, 4});
	attrDesc.push_back({"P_RETAILPRICE", kFLOAT32, 7, kNoContainer, -1});
	attrDesc.push_back({"P_COMMENT", kCHAR_STRING, 8, kSimpleCharContainer, 5});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleDictionary, 2});
	contDesc.push_back({kSimpleDictionary, 3});
	contDesc.push_back({kSimpleDictionary, 4});
	contDesc.push_back({kSimpleDictionary, 6});
	contDesc.push_back({kSimpleCharContainer, 1024});

	string_vt p_mfgr_dict_entries;
	for(uint i = 1; i < 6; ++i)
	{
		p_mfgr_dict_entries.push_back("Manufacturer#" + std::to_string(i));
	}

	string_vt p_brand_dict_entries;
	for(uint i = 1; i < 56; ++i)
	{
		p_brand_dict_entries.push_back("Brand#" + std::to_string(i));
	}

	string_vt p_type_dict_entries;
	std::string comb1[6] = { "STANDARD", "SMALL", "MEDIUM", "LARGE", "ECONOMY", "PROMO" };
	std::string comb2[5] = { "ANODIZED", "BURNISHED", "PLATED", "POLISHED", "BRUSHED" };
	std::string comb3[5] = { "TIN", "NICKEL", "BRASS", "STEEL", "COPPER" };
	std::string combination;
	for (uint i = 0; i < 6; i++) 
	{
		for (uint j = 0; j < 5; j++) 
		{
			for (uint k = 0; k < 5; k++) 
			{
				combination = (comb1[i] + " " + comb2[j] + " " + comb3[k]);
				p_type_dict_entries.push_back(combination);
			}
		}
	}

	string_vt p_container_dict_entries;
	std::string comb4[5] = {"SM","LG","MED","JUMBO","WRAP"};
	std::string comb5[8] = {"CASE","BOX","BAG","JAR","PKG","PACK","CAN","DRUM"};
	for (uint i = 0; i < 5; i++) 
	{
		for (uint j = 0; j < 8; j++) 
		{
			combination = (comb4[i] + " " + comb5[j]);
			p_container_dict_entries.push_back(combination);
		}
	}

	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&p_mfgr_dict_entries);
	dict_entries_pointer.push_back((byte*)&p_brand_dict_entries);
	dict_entries_pointer.push_back((byte*)&p_type_dict_entries);
	dict_entries_pointer.push_back((byte*)&p_container_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initPartsuppRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "partsupp.tbl";
	std::string relName("PARTSUPP");
	schema_vt logSchema = {kUINT32, kUINT32, kUINT32, kFLOAT32, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"PS_PARTKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"PS_SUPPKEY", kUINT32, 1, kNoContainer, -1});
	attrDesc.push_back({"PS_AVAILQTY", kUINT32, 2, kNoContainer, -1});
	attrDesc.push_back({"PS_SUPPLYCOST", kFLOAT32, 3, kNoContainer, -1});
	attrDesc.push_back({"PS_COMMENT", kCHAR_STRING, 4, kSimpleCharContainer, 0});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleCharContainer, 1024});
	Segment segment;

	byte_vpt dict_entries_pointer;

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initRegionRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "region.tbl";
	std::string relName("REGION");
	schema_vt logSchema = {kUINT32, kSTR_SDICT, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"R_REGIONKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"R_NAME", kSTR_SDICT, 1, kSimpleDictionary, 0});
	attrDesc.push_back({"R_COMMENT", kCHAR_STRING, 2, kSimpleCharContainer, 1});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleDictionary, 1});
	contDesc.push_back({kSimpleCharContainer, 1024});
	string_vt r_name_dict_entries;
	r_name_dict_entries.push_back("AFRICA");
	r_name_dict_entries.push_back("AMERICA");
	r_name_dict_entries.push_back("ASIA");
	r_name_dict_entries.push_back("EUROPE");
	r_name_dict_entries.push_back("MIDDLE EAST");

	Segment segment;

	byte_vpt dict_entries_pointer;
	dict_entries_pointer.push_back((byte*)&r_name_dict_entries);

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}

template<typename T_Relation>
void initSupplierRelation(T_Relation& aRelation, std::string& aFileName)
{
	aFileName += "supplier.tbl";
	std::string relName("SUPPLIER");
	schema_vt logSchema = {kUINT32, kCHAR_STRING, kCHAR_STRING, kUINT32, kCHAR_STRING, kFLOAT32, kCHAR_STRING};
	Relation::attr_desc_vt attrDesc;
	attrDesc.push_back({"S_SUPPKEY", kUINT32, 0, kNoContainer, -1});					
	attrDesc.push_back({"S_NAME", kCHAR_STRING, 1, kSimpleCharContainer, 0});
	attrDesc.push_back({"S_ADDRESS", kCHAR_STRING, 2, kSimpleCharContainer, 1});
	attrDesc.push_back({"S_NATIONKEY", kUINT32, 3, kNoContainer, -1});
	attrDesc.push_back({"S_PHONE", kCHAR_STRING, 4, kSimpleCharContainer, 2});
	attrDesc.push_back({"S_ACCTBAL", kFLOAT32, 5, kNoContainer, -1});
	attrDesc.push_back({"S_COMMENT", kCHAR_STRING, 6, kSimpleCharContainer, 3});
	Relation::cont_desc_vt contDesc;
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	contDesc.push_back({kSimpleCharContainer, 1024});
	Segment segment;

	byte_vpt dict_entries_pointer;

	aRelation.initSubRelation(relName, attrDesc, logSchema, contDesc, segment, dict_entries_pointer);
	// aRelation.print(relName + " Relation");
}