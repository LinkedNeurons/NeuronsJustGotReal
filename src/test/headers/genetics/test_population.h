void population_test_generation();

int add_population_tests() {
    CU_pSuite suite = CU_add_suite("Matrix", NULL, NULL);

    return suite
        && CU_add_test(suite, "population_next_generation", &population_test_generation);
}
