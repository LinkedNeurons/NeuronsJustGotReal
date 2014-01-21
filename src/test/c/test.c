#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

int main() {
    
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite("Suite_1", NULL, NULL);
    if (!pSuite) {
        goto end;
    }

    /* add the tests to the suite */
    /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
    /*if (NULL == CU_add_test(pSuite, "test description", test_function))
    {
        goto end;
    }*/

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
   
    end:
    CU_cleanup_registry();
    return CU_get_error();
}

