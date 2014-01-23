
void vector_test_init();
void vector_test_create();
void vector_test_clone();

void vector_test_foldl();
void vector_test_foldr();

int add_vector_tests() {
    CU_pSuite suite = CU_add_suite("Vector", NULL, NULL);

    return suite
			&& CU_add_test(suite, "vector_init",	&vector_test_init)
			&& CU_add_test(suite, "vector_create",	&vector_test_create)
			&& CU_add_test(suite, "vector_clone",	&vector_test_clone)

			&& CU_add_test(suite, "vector_foldl",	&vector_test_foldl)
			&& CU_add_test(suite, "vector_foldr",	&vector_test_foldr);
}
