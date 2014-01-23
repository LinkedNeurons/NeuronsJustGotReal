#include <errno.h>
#include <CUnit/CUnit.h>
#include <math/matrix.h>

void matrix_test_create() {
    Matrix* matrix = IMatrix.create(3, 4);

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
    Matrix* m1 = IMatrix.create(0, 1);
    
    CU_ASSERT(m1 == NULL);
    CU_ASSERT(errno == EINVAL);
    errno = 0;

    Matrix* m2 = IMatrix.create(1, 0);
    
    CU_ASSERT(m2 == NULL);
    CU_ASSERT(errno == EINVAL);
    errno = 0;
    
    Matrix* m3 = IMatrix.create(0, 0);
    
    CU_ASSERT(m3 == NULL);
    CU_ASSERT(errno == EINVAL);
}

void matrix_test_init() {
    Matrix* matrix = IMatrix.init(2, 3, (double[]) {
        1, 2, 3,
        4, 5, 6
    });

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
    Matrix *m1 = IMatrix.init(2, 3, (double[]) {
        1, 2, 3,
        4, 5, 6
    });

    Matrix *m2 = IMatrix.init(3, 2, (double[]) {
         7,  8,
         9, 10,
        11, 12
    });

    Matrix *product = IMatrix.product(m1, m2);

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
    Matrix *m1 = IMatrix.create(2, 3);
    Matrix *m2 = IMatrix.create(4, 5);
    
    Matrix *product = IMatrix.product(m1, m2);

    CU_ASSERT(product == NULL);
    CU_ASSERT(errno == EDOM);
    
    IMatrix.destroy(m1);
    IMatrix.destroy(m2);
}

void matrix_test_mul() {
    Matrix *matrix = IMatrix.init(2, 2, (double[]) {
       1, 1,
       1, 1
    });

    Matrix *result = IMatrix.mul(matrix, 2);
    
    CU_ASSERT(2 == IMatrix.get(result, 0, 0));
    CU_ASSERT(2 == IMatrix.get(result, 0, 1));
    CU_ASSERT(2 == IMatrix.get(result, 1, 0));
    CU_ASSERT(2 == IMatrix.get(result, 1, 1));

    IMatrix.destroy(matrix);
    IMatrix.destroy(result);
}

void matrix_test_add() {
    Matrix *m1 = IMatrix.init(2, 2, (double[]) {
       1, 2,
       3, 4
    });
    Matrix *m2 = IMatrix.init(2, 2, (double[]) {
       5, 6,
       7, 8
    });

    Matrix *result = IMatrix.add(m1, m2);

    CU_ASSERT(6  == IMatrix.get(result, 0, 0));
    CU_ASSERT(8  == IMatrix.get(result, 0, 1));
    CU_ASSERT(10 == IMatrix.get(result, 1, 0));
    CU_ASSERT(12 == IMatrix.get(result, 1, 1));

    IMatrix.destroy(m1);
    IMatrix.destroy(m2);
}

void matrix_test_equals() {
    Matrix *m1 = IMatrix.init(2, 2, (double[]) {
       1, 2,
       3, 4
    });

    Matrix *m2 = IMatrix.init(2, 2, (double[]) {
       4, 3,
       2, 1
    });

    Matrix *m3 = IMatrix.init(2, 4, (double[]) {
       4, 3, 8, 0,
       2, 1, 9, 5
    });

    CU_ASSERT( IMatrix.equals(m1, m1));
    CU_ASSERT(!IMatrix.equals(m1, m2));
    CU_ASSERT(!IMatrix.equals(m1, m3));
    
    IMatrix.destroy(m1);
    IMatrix.destroy(m2);
    IMatrix.destroy(m3);
}
