#include <errno.h>
#include <CUnit/CUnit.h>
#include <vector.h>

void vector_test_init() {
	Vector *v = vector_init(4, (double[]) {1, 2, 3, 4});
	
	for(int i = 0; i < v->size; ++i) {
		CU_ASSERT(i + 1 == vector_at(v, i));
	}
}
