#include "test_update.hh"

void update1(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
	}
}

void update1(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
	}
}

void update2(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
	}
}

void update2(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
	}
}

void update3(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
	}
}

void update3(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
	}
}

void update4(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
	}
}

void update4(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
	}
}

void update5(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
	}
}

void update5(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
	}
}

void update6(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
	}
}

void update6(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(45);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
	}
}

void update7(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
	}
}

void update7(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
	}
}

void update8(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
	}
}

void update8(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
	}
}

void update9(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(aPI.getRecord(i) + off_1) += 1;
		*(uint*)(aPI.getRecord(i) + off_2) += 2;
		*(uint*)(aPI.getRecord(i) + off_3) += 3;
		*(uint*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';

	}
}

void update9(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
	}
}

void update10(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
	}
}

void update10(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
	}
}

void update11(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
	}
}

void update11(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
	}
}

void update12(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	uint off_12 = aRelation.getOffset()[11];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_12) += 2;
	}
}

void update12(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	byte* miniPage_12 = aPI.getMiniPagePointer(11);
	uint attrSize_12 = aPI.getAttrSize(11);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
		*(uint32_t*)(miniPage_12 + (i * attrSize_12)) += 2;
	}
}

void update13(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	uint off_12 = aRelation.getOffset()[11];
	uint off_13 = aRelation.getOffset()[12];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_12) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_13) += 3;
	}
}

void update13(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	byte* miniPage_12 = aPI.getMiniPagePointer(11);
	uint attrSize_12 = aPI.getAttrSize(11);
	byte* miniPage_13 = aPI.getMiniPagePointer(12);
	uint attrSize_13 = aPI.getAttrSize(12);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
		*(uint32_t*)(miniPage_12 + (i * attrSize_12)) += 2;
		*(uint32_t*)(miniPage_13 + (i * attrSize_13)) += 3;
	}
}

void update14(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	uint off_12 = aRelation.getOffset()[11];
	uint off_13 = aRelation.getOffset()[12];
	uint off_14 = aRelation.getOffset()[13];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_12) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_13) += 3;
		uint32_t ship_instr = *(uint32_t*)(aPI.getRecord(i) + off_14);
		*(uint32_t*)(aPI.getRecord(i) + off_14) = ((ship_instr + 2) % 4);
	}
}

void update14(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	byte* miniPage_12 = aPI.getMiniPagePointer(11);
	uint attrSize_12 = aPI.getAttrSize(11);
	byte* miniPage_13 = aPI.getMiniPagePointer(12);
	uint attrSize_13 = aPI.getAttrSize(12);
	byte* miniPage_14 = aPI.getMiniPagePointer(13);
	uint attrSize_14 = aPI.getAttrSize(13);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
		*(uint32_t*)(miniPage_12 + (i * attrSize_12)) += 2;
		*(uint32_t*)(miniPage_13 + (i * attrSize_13)) += 3;
		uint32_t ship_instr = *(uint32_t*)(miniPage_14 + (i * attrSize_14));
		*(uint32_t*)(miniPage_14 + (i * attrSize_14)) = ((ship_instr + 2) % 4);
	}
}

void update15(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	uint off_12 = aRelation.getOffset()[11];
	uint off_13 = aRelation.getOffset()[12];
	uint off_14 = aRelation.getOffset()[13];
	uint off_15 = aRelation.getOffset()[14];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_12) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_13) += 3;
		uint32_t ship_instr = *(uint32_t*)(aPI.getRecord(i) + off_14);
		*(uint32_t*)(aPI.getRecord(i) + off_14) = ((ship_instr + 2) % 4);
		uint32_t ship_mode = *(uint32_t*)(aPI.getRecord(i) + off_15);
		*(uint32_t*)(aPI.getRecord(i) + off_15) = ((ship_mode + 3) % 7);
	}
}

void update15(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	byte* miniPage_12 = aPI.getMiniPagePointer(11);
	uint attrSize_12 = aPI.getAttrSize(11);
	byte* miniPage_13 = aPI.getMiniPagePointer(12);
	uint attrSize_13 = aPI.getAttrSize(12);
	byte* miniPage_14 = aPI.getMiniPagePointer(13);
	uint attrSize_14 = aPI.getAttrSize(13);
	byte* miniPage_15 = aPI.getMiniPagePointer(14);
	uint attrSize_15 = aPI.getAttrSize(14);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
		*(uint32_t*)(miniPage_12 + (i * attrSize_12)) += 2;
		*(uint32_t*)(miniPage_13 + (i * attrSize_13)) += 3;
		uint32_t ship_instr = *(uint32_t*)(miniPage_14 + (i * attrSize_14));
		*(uint32_t*)(miniPage_14 + (i * attrSize_14)) = ((ship_instr + 2) % 4);
		uint32_t ship_mode = *(uint32_t*)(miniPage_15 + (i * attrSize_15));
		*(uint32_t*)(miniPage_15 + (i * attrSize_15)) = ((ship_mode + 3) % 7);
	}
}

void update16(NSM_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterSP aPI;
	aPI.attach(aPagePointer);
	uint off_1 = aRelation.getOffset()[0];
	uint off_2 = aRelation.getOffset()[1];
	uint off_3 = aRelation.getOffset()[2];
	uint off_4 = aRelation.getOffset()[3];
	uint off_5 = aRelation.getOffset()[4];
	uint off_6 = aRelation.getOffset()[5];
	uint off_7 = aRelation.getOffset()[6];
	uint off_8 = aRelation.getOffset()[7];
	uint off_9 = aRelation.getOffset()[8];
	uint off_10 = aRelation.getOffset()[9];
	uint off_11 = aRelation.getOffset()[10];
	uint off_12 = aRelation.getOffset()[11];
	uint off_13 = aRelation.getOffset()[12];
	uint off_14 = aRelation.getOffset()[13];
	uint off_15 = aRelation.getOffset()[14];
	uint off_16 = aRelation.getOffset()[15];
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint32_t*)(aPI.getRecord(i) + off_1) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_2) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_3) += 3;
		*(uint32_t*)(aPI.getRecord(i) + off_4) += 4;
		*(float*)(aPI.getRecord(i) + off_5) *= 2;
		*(float*)(aPI.getRecord(i) + off_6) /= 2;
		*(float*)(aPI.getRecord(i) + off_7) *= 1.1;
		*(float*)(aPI.getRecord(i) + off_8) += 0.02;
		*(char*)(aPI.getRecord(i) + off_9) = 'X';
		*(char*)(aPI.getRecord(i) + off_10) = 'Y';
		*(uint32_t*)(aPI.getRecord(i) + off_11) += 1;
		*(uint32_t*)(aPI.getRecord(i) + off_12) += 2;
		*(uint32_t*)(aPI.getRecord(i) + off_13) += 3;
		uint32_t ship_instr = *(uint32_t*)(aPI.getRecord(i) + off_14);
		*(uint32_t*)(aPI.getRecord(i) + off_14) = ((ship_instr + 2) % 4);
		uint32_t ship_mode = *(uint32_t*)(aPI.getRecord(i) + off_15);
		*(uint32_t*)(aPI.getRecord(i) + off_15) = ((ship_mode + 3) % 7);
		*(const char**)(aPI.getRecord(i) + off_16) = "I am an example comment. My only job is to change the content this pointer points to.";
	}
}

void update16(PAX_Relation& aRelation, byte* aPagePointer)
{
	PageInterpreterPAX aPI;
	aPI.attach(aPagePointer);
	byte* miniPage_1 = aPI.getMiniPagePointer(0);
	uint attrSize_1 = aPI.getAttrSize(0);
	byte* miniPage_2 = aPI.getMiniPagePointer(1);
	uint attrSize_2 = aPI.getAttrSize(1);
	byte* miniPage_3 = aPI.getMiniPagePointer(2);
	uint attrSize_3 = aPI.getAttrSize(2);
	byte* miniPage_4 = aPI.getMiniPagePointer(3);
	uint attrSize_4 = aPI.getAttrSize(3);
	byte* miniPage_5 = aPI.getMiniPagePointer(4);
	uint attrSize_5 = aPI.getAttrSize(4);
	byte* miniPage_6 = aPI.getMiniPagePointer(5);
	uint attrSize_6 = aPI.getAttrSize(5);
	byte* miniPage_7 = aPI.getMiniPagePointer(6);
	uint attrSize_7 = aPI.getAttrSize(6);
	byte* miniPage_8 = aPI.getMiniPagePointer(7);
	uint attrSize_8 = aPI.getAttrSize(7);
	byte* miniPage_9 = aPI.getMiniPagePointer(8);
	uint attrSize_9 = aPI.getAttrSize(8);
	byte* miniPage_10 = aPI.getMiniPagePointer(9);
	uint attrSize_10 = aPI.getAttrSize(9);
	byte* miniPage_11 = aPI.getMiniPagePointer(10);
	uint attrSize_11 = aPI.getAttrSize(10);
	byte* miniPage_12 = aPI.getMiniPagePointer(11);
	uint attrSize_12 = aPI.getAttrSize(11);
	byte* miniPage_13 = aPI.getMiniPagePointer(12);
	uint attrSize_13 = aPI.getAttrSize(12);
	byte* miniPage_14 = aPI.getMiniPagePointer(13);
	uint attrSize_14 = aPI.getAttrSize(13);
	byte* miniPage_15 = aPI.getMiniPagePointer(14);
	uint attrSize_15 = aPI.getAttrSize(14);
	byte* miniPage_16 = aPI.getMiniPagePointer(15);
	uint attrSize_16 = aPI.getAttrSize(15);
	for(size_t i = 0; i < aPI.noRecords(); ++i)
	{
		*(uint*)(miniPage_1 + (i * attrSize_1)) += 1;
		*(uint*)(miniPage_2 + (i * attrSize_2)) += 2;
		*(uint*)(miniPage_3 + (i * attrSize_3)) += 3;
		*(uint*)(miniPage_4 + (i * attrSize_4)) += 4;
		*(uint*)(miniPage_5 + (i * attrSize_5)) *= 2;
		*(uint*)(miniPage_6 + (i * attrSize_6)) /= 2;
		*(uint*)(miniPage_7 + (i * attrSize_7)) *= 1.1;
		*(uint*)(miniPage_8 + (i * attrSize_8)) += 0.02;
		*(char*)(miniPage_9 + (i * attrSize_9)) = 'X';
		*(char*)(miniPage_10 + (i * attrSize_10)) = 'Y';
		*(uint32_t*)(miniPage_11 + (i * attrSize_11)) += 1;
		*(uint32_t*)(miniPage_12 + (i * attrSize_12)) += 2;
		*(uint32_t*)(miniPage_13 + (i * attrSize_13)) += 3;
		uint32_t ship_instr = *(uint32_t*)(miniPage_14 + (i * attrSize_14));
		*(uint32_t*)(miniPage_14 + (i * attrSize_14)) = ((ship_instr + 2) % 4);
		uint32_t ship_mode = *(uint32_t*)(miniPage_15 + (i * attrSize_15));
		*(uint32_t*)(miniPage_15 + (i * attrSize_15)) = ((ship_mode + 3) % 7);
		*(const char**)(miniPage_16 + (i * attrSize_16)) = "I am an example comment. My only job is to change the content this pointer points to.";
	}
}

void update_int(NSM_Relation& aRelation, const size_t aAttrNo)
{
	for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
	{
		PageInterpreterSP aPI;
		aPI.attach(aRelation.getSegment().getPage(i));
		for(size_t j = 0; j < aPI.noRecords(); ++j)
		{
			for (size_t k = 0; k < aAttrNo; ++k)
			{
				++(*(uint32_t*)(aPI.getRecord(j) + aRelation.getOffset()[k]));
			}
		}
	}
}

void update_int(PAX_Relation& aRelation, const size_t aAttrNo)
{
	for(size_t i = 0; i < aRelation.getSegment().getNoPages(); ++i)
	{
		PageInterpreterPAX aPI;
		aPI.attach(aRelation.getSegment().getPage(i));
		for(size_t j = 0; j < aPI.noRecords(); ++j)
		{
			for (size_t k = 0; k < aAttrNo; ++k)
			{
				++(*(uint32_t*)(aPI.getMiniPagePointer(k) + (j * aPI.getAttrSize(k))));
			}
		}
	}
}

