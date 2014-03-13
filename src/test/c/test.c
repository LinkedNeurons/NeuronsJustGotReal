#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "test_network.h"
#include "math/test_matrix.h"
#include "math/test_vector.h"


int main() {
	
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	int success = 1
		&& add_matrix_tests() 
		&& add_vector_tests()
		&& add_network_tests();
	
	unsigned tests_failed = 0;
	if (success) {
		CU_basic_set_mode(CU_BRM_VERBOSE);
		CU_basic_run_tests();
		tests_failed = !!CU_get_number_of_failures();
	}

	CU_cleanup_registry();
	return tests_failed | (CU_get_error() << 1);
}

