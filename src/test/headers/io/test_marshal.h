void test_marshal_basic();
void test_marshal_math();

int add_marshal_tests() {
    CU_pSuite suite = CU_add_suite("Marshal", NULL, NULL);

    return suite
        && CU_add_test(suite, "marshal_basic", &test_marshal_basic)
        && CU_add_test(suite, "marshal_math", &test_marshal_math);
}
