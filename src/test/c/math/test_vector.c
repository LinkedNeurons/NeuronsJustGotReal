#include <errno.h>
#include <CUnit/CUnit.h>
#include <vector.h>

void vector_test_init() {
	size_t size = 4;
	Vector *v = vector_init(size, (double[]) {1, 2, 3, 4});
	
	CU_TEST_FATAL(v->size == size);
	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(i + 1 == vector_at(v, i));
	}
}

void vector_test_create() {
	size_t size = 4;
	Vector *v = vector_create(size);

	CU_TEST_FATAL(v->size == size);
	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(vector_at(v, i) == 0);
	}
}

void vector_test_clone() {
	size_t size = 4;
	Vector *v = vector_init(size, (double[]) {1, 2, 3, 4});

	Vector *vc = vector_clone(v);
	CU_TEST_FATAL(vc->size == v->size);
	CU_TEST_FATAL(vc->size == size);

	for(size_t i = 0; i < size; ++i) {
		CU_ASSERT(vector_at(v, i) == vector_at(vc, i));
	}
}

void vector_test_foldl() {

}
void vector_test_foldr() {

}