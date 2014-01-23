#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#include "math/test_matrix.h"

int main() {
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    int success = 1
        && add_matrix_tests();
    
    if (success) {
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
    }

    CU_cleanup_registry();
    return CU_get_error();
}

