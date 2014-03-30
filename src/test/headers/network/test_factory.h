void network_test_feed();

int add_network_tests() {
    CU_pSuite suite = CU_add_suite("Network", NULL, NULL);

    return suite && CU_add_test(suite, "network_feed",	&network_test_feed);
}
