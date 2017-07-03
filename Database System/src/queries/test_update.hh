#ifndef TEST_UPDATE_HH
#define TEST_UPDATE_HH

#include "infra/webe/types.hh"
#include "memory/segment.hh"
#include "relation/nsm_relation.hh"
#include "nsm/page_interpreter_sp.hh"
#include "relation/pax_relation.hh"
#include "pax/page_interpreter_pax.hh"

void update1(NSM_Relation& aRelation, byte* aPagePointer);

void update1(PAX_Relation& aRelation, byte* aPagePointer);

void update2(NSM_Relation& aRelation, byte* aPagePointer);

void update2(PAX_Relation& aRelation, byte* aPagePointer);

void update3(NSM_Relation& aRelation, byte* aPagePointer);

void update3(PAX_Relation& aRelation, byte* aPagePointer);

void update4(NSM_Relation& aRelation, byte* aPagePointer);

void update4(PAX_Relation& aRelation, byte* aPagePointer);

void update5(NSM_Relation& aRelation, byte* aPagePointer);

void update5(PAX_Relation& aRelation, byte* aPagePointer);

void update6(NSM_Relation& aRelation, byte* aPagePointer);

void update6(PAX_Relation& aRelation, byte* aPagePointer);

void update7(NSM_Relation& aRelation, byte* aPagePointer);

void update7(PAX_Relation& aRelation, byte* aPagePointer);

void update8(NSM_Relation& aRelation, byte* aPagePointer);

void update8(PAX_Relation& aRelation, byte* aPagePointer);

void update9(NSM_Relation& aRelation, byte* aPagePointer);

void update9(PAX_Relation& aRelation, byte* aPagePointer);

void update10(NSM_Relation& aRelation, byte* aPagePointer);

void update10(PAX_Relation& aRelation, byte* aPagePointer);

void update11(NSM_Relation& aRelation, byte* aPagePointer);

void update11(PAX_Relation& aRelation, byte* aPagePointer);

void update12(NSM_Relation& aRelation, byte* aPagePointer);

void update12(PAX_Relation& aRelation, byte* aPagePointer);

void update13(NSM_Relation& aRelation, byte* aPagePointer);

void update13(PAX_Relation& aRelation, byte* aPagePointer);

void update14(NSM_Relation& aRelation, byte* aPagePointer);

void update14(PAX_Relation& aRelation, byte* aPagePointer);

void update15(NSM_Relation& aRelation, byte* aPagePointer);

void update15(PAX_Relation& aRelation, byte* aPagePointer);

void update16(NSM_Relation& aRelation, byte* aPagePointer);

void update16(PAX_Relation& aRelation, byte* aPagePointer);

void update_int(NSM_Relation& aRelation, const size_t aAttrNo);

void update_int(PAX_Relation& aRelation, const size_t aAttrNo);


#endif