void factory_test_build();

int add_factory_tests() {
    CU_pSuite suite = CU_add_suite("Network Factory", NULL, NULL);

    return suite && CU_add_test(suite, "network_factory_build",	&factory_test_build);
}
