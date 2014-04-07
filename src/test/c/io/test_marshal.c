#include <CUnit/CUnit.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "io/marshal.h"

void test_marshal_basic() {
	FILE *file = fopen("marshal_test", "w+");

	char *str = "hello world";
	double arr[] = {1,2,3,4,5};
	size_t size = 0;
	double *readarr = NULL;
	char *readstr = NULL;

	marshal_write_array(file, arr, sizeof(double), 5);
	marshal_write_string(file, str);

	rewind(file);

	marshal_read_array(file, (void**) &readarr, &size);
	marshal_read_string(file, &readstr);

	fclose (file);

	CU_ASSERT_EQUAL_FATAL(size, 5 * sizeof(double));
	for (size_t i = 0; i < 5; ++i) {
		CU_ASSERT_EQUAL(readarr[i], arr[i]);
	}
	CU_ASSERT_STRING_EQUAL(readstr, str);

	free(readarr);
	free(readstr);
}

void test_marshal_math() {
	Matrix *m = NULL, *readm = NULL;
	IMatrix.init(2, 3, (double[]) {
			1, 2, 3,
			4, 5, 6
	}, &m);

	Vector *v = NULL, *readv = NULL;
	IVector.init(2, (double[]) {
		7, 8
	}, &v);

	FILE *file = fopen("marshal_test", "w+");

	marshal_write_matrix(file, m);
	marshal_write_vector(file, v);
	
	rewind(file);

	marshal_read_matrix(file, &readm);
	marshal_read_vector(file, &readv);
	
	fclose(file);

	CU_ASSERT(IMatrix.equals(m, readm));
	CU_ASSERT(IVector.equals(v, readv));

	IMatrix.destroy(m);
	IMatrix.destroy(readm);
	IVector.destroy(v);
	IVector.destroy(readv);
}
