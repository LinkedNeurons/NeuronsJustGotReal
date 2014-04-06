#include <errno.h>
#include <CUnit/CUnit.h>
#include <math/vector.h>

void vector_test_init() {
	size_t size = 4;
	Vector *v = NULL;
	IVector.init(size, (double[]) {1, 2, 3, 4}, &v);
	
	CU_TEST_FATAL(v->size == size);
	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(i + 1 == vector_at(v, i));
	}
}

void vector_test_create() {
	size_t size = 4;
	Vector *v = IVector.create(size);

	CU_TEST_FATAL(v->size == size);
	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(IVector.at(v, i) == 0);
	}
}

void vector_test_equals() {
	size_t size = 4;
	Vector *v1 = NULL, *v2 = NULL, *v3 = NULL, *v4 = NULL;
	IVector.init(size, (double[]) {1, 2, 3, 4}, &v1);
	IVector.init(size, (double[]) {1, 2, 3, 4}, &v2);
	IVector.init(size, (double[]) {4, 3, 2, 1}, &v3);
	IVector.init(size + 1, (double[]) {1, 2, 3, 4, 5}, &v4);
	
	CU_TEST_FATAL(v1->size == v2->size);
	CU_TEST_FATAL(v2->size == v3->size);
	CU_TEST_FATAL(v1->size != v4->size);

	CU_TEST_FATAL(IVector.equals(v1, v2) == 1);
	CU_TEST_FATAL(IVector.equals(v1, v3) == 0);
}


void vector_test_clone() {
	size_t size = 4;
	Vector *v = NULL;
	IVector.init(size, (double[]) {1, 2, 3, 4}, &v);

	Vector *vc = vector_clone(v);
	CU_TEST_FATAL(vc->size == v->size);
	CU_TEST_FATAL(vc->size == size);

	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(IVector.at(v, i) == IVector.at(vc, i));
	}
}

void vector_test_foldl() {

}
void vector_test_foldr() {

}
