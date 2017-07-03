#include "dummy.hh"

template<typename T_Tuple>
uint dummy_function(T_Tuple* aTupleBuffer)
{
	return aTupleBuffer->_size;
}

template uint dummy_function<NSM_Tuple>(NSM_Tuple* aTupleBuffer);
template uint dummy_function<PAX_Tuple>(PAX_Tuple* aTupleBuffer);