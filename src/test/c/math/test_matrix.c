#include <errno.h>
#include <CUnit/CUnit.h>
#include <math/matrix.h>

void matrix_test_create() {
	Matrix* matrix = NULL;
	IMatrix.create(3, 4, &matrix);

	CU_ASSERT(3 == matrix->rows);
	CU_ASSERT(4 == matrix->cols);

	for (size_t i = 0; i < matrix->rows; ++i) {
		for (size_t j = 0; j < matrix->cols; ++j) {
			CU_ASSERT(0 == IMatrix.get(matrix, i, j));
		}
	}
	
	IMatrix.destroy(matrix);
}

void matrix_test_create_invalid_dim() {
	Matrix* m1 = NULL;
	IMatrix.create(0, 1, &m1);
	
	CU_ASSERT(m1 == NULL);
	CU_ASSERT(errno == EINVAL);
	errno = 0;

	Matrix* m2 = NULL;
	IMatrix.create(1, 0, &m2);
	
	CU_ASSERT(m2 == NULL);
	CU_ASSERT(errno == EINVAL);
	errno = 0;
	
	Matrix* m3 = NULL;
	IMatrix.create(0, 0, &m3);
	
	CU_ASSERT(m3 == NULL);
	CU_ASSERT(errno == EINVAL);
}

void matrix_test_init() {
	Matrix* matrix = NULL;
	IMatrix.init(2, 3, (double[]) {
		1, 2, 3,
		4, 5, 6
	}, &matrix);

	CU_ASSERT(2 == matrix->rows);
	CU_ASSERT(3 == matrix->cols);

	CU_ASSERT(1 == IMatrix.get(matrix, 0, 0));
	CU_ASSERT(2 == IMatrix.get(matrix, 0, 1));
	CU_ASSERT(3 == IMatrix.get(matrix, 0, 2));
	CU_ASSERT(4 == IMatrix.get(matrix, 1, 0));
	CU_ASSERT(5 == IMatrix.get(matrix, 1, 1));
	CU_ASSERT(6 == IMatrix.get(matrix, 1, 2));
	
	IMatrix.destroy(matrix);
}

void matrix_test_product() {
	Matrix *m1 = NULL;
	IMatrix.init(2, 3, (double[]) {
		1, 2, 3,
		4, 5, 6
	}, &m1);

	Matrix *m2 = NULL;
	IMatrix.init(3, 2, (double[]) {
		 7,  8,
		 9, 10,
		11, 12
	}, &m2);

	Matrix *product = NULL;
	IMatrix.product(m1, m2, &product);

	CU_ASSERT(2 == product->rows);
	CU_ASSERT(2 == product->cols);

	CU_ASSERT(58  == IMatrix.get(product, 0, 0));
	CU_ASSERT(64  == IMatrix.get(product, 0, 1));
	CU_ASSERT(139 == IMatrix.get(product, 1, 0));
	CU_ASSERT(154 == IMatrix.get(product, 1, 1));

	IMatrix.destroy(m1);
	IMatrix.destroy(m2);
	IMatrix.destroy(product);
}

void matrix_test_product_invalid_dim() {
	Matrix *m1 = NULL;
	Matrix *m2 = NULL;
	IMatrix.create(2, 3, &m1);
	IMatrix.create(4, 5, &m2);
	
	Matrix *product = NULL;
	IMatrix.product(m1, m2, &product);

	CU_ASSERT(product == NULL);
	CU_ASSERT(errno == EDOM);
	
	IMatrix.destroy(m1);
	IMatrix.destroy(m2);
}

void matrix_test_mul() {
	Matrix *matrix = NULL;
	IMatrix.init(2, 2, (double[]) {
	   1, 1,
	   1, 1
	}, &matrix);

	Matrix *result = NULL;
	IMatrix.mul(matrix, 2, &result);
	
	CU_ASSERT(2 == IMatrix.get(result, 0, 0));
	CU_ASSERT(2 == IMatrix.get(result, 0, 1));
	CU_ASSERT(2 == IMatrix.get(result, 1, 0));
	CU_ASSERT(2 == IMatrix.get(result, 1, 1));

	IMatrix.destroy(matrix);
	IMatrix.destroy(result);
}

void matrix_test_add() {
	Matrix *m1 = NULL;
	IMatrix.init(2, 2, (double[]) {
	   1, 2,
	   3, 4
	}, &m1);
	Matrix *m2 = NULL;
	IMatrix.init(2, 2, (double[]) {
	   5, 6,
	   7, 8
	}, &m2);

	Matrix *result = NULL;
	IMatrix.add(m1, m2, &result);

	CU_ASSERT(6  == IMatrix.get(result, 0, 0));
	CU_ASSERT(8  == IMatrix.get(result, 0, 1));
	CU_ASSERT(10 == IMatrix.get(result, 1, 0));
	CU_ASSERT(12 == IMatrix.get(result, 1, 1));

	IMatrix.destroy(m1);
	IMatrix.destroy(m2);
}

void matrix_test_equals() {
	Matrix *m1 = NULL;
	IMatrix.init(2, 2, (double[]) {
	   1, 2,
	   3, 4
	}, &m1);

	Matrix *m2 = NULL;
	IMatrix.init(2, 2, (double[]) {
	   4, 3,
	   2, 1
	}, &m2);

	Matrix *m3 = NULL;
	IMatrix.init(2, 4, (double[]) {
	   4, 3, 8, 0,
	   2, 1, 9, 5
	}, &m3);

	CU_ASSERT( IMatrix.equals(m1, m1));
	CU_ASSERT(!IMatrix.equals(m1, m2));
	CU_ASSERT(!IMatrix.equals(m1, m3));
	
	IMatrix.destroy(m1);
	IMatrix.destroy(m2);
	IMatrix.destroy(m3);
}
void matrix_test_repmat() {
	Vector* v = NULL;
	IVector.init(3, (double[]) {1, 2, 3}, &v);
	Matrix* result = NULL;
	IMatrix.init(6, 2, (double[]) {
		1, 1,
		2, 2,
		3, 3,
		1, 1,
		2, 2,
		3, 3
	}, &result);

	Matrix* rep = NULL;
	IMatrix.repmat(v, 2, 2, &rep);
	CU_ASSERT(IMatrix.equals(result, rep));

	IMatrix.destroy(result);
	IMatrix.destroy(rep);
}
