void network_test_feed();
void network_test_gradient_descent();

int add_network_tests() {
    CU_pSuite suite = CU_add_suite("Network", NULL, NULL);

    return suite 
		&& CU_add_test(suite, "network_feed",	&network_test_feed)
		//&& CU_add_test(suite, "network_gradient_descent",	&network_test_gradient_descent)
		;
}
