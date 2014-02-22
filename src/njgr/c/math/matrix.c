#include <errno.h>
#include "math/matrix.h"

struct MatrixModule IMatrix = {
    .create  = &matrix_create,
    .init    = &matrix_init,
    .destroy = &matrix_destroy,
    .get     = &matrix_get,
    .set     = &matrix_set,
    .product = &matrix_product,
    .mul     = &matrix_mul,
    .add     = &matrix_add,
    .equals  = &matrix_equals
};

Matrix* matrix_create(size_t rows, size_t columns) {
    if (!rows || !columns) {
        errno = EINVAL;
        return NULL;
    }
    Matrix *matrix = malloc(sizeof(Matrix));
        matrix->arr  = calloc(rows * columns, sizeof(double));
        matrix->rows = rows;
        matrix->cols = columns;
    return matrix;
}

Matrix* matrix_init(size_t rows, size_t columns, const double* values) {
    if (!rows || !columns) {
        errno = EINVAL;
        return NULL;
    }
    Matrix *matrix = malloc(sizeof(Matrix));
        matrix->arr  = malloc(rows * columns * sizeof(double));
        matrix->rows = rows;
        matrix->cols = columns;

    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            matrix_set(matrix, i, j, values[j + matrix->cols * i]);
        }
    }
    
    return matrix;
}

void matrix_destroy(Matrix* matrix) {
    free(matrix->arr);
    free(matrix);
}

double matrix_get(Matrix *matrix, size_t row, size_t col) {
    return matrix->arr[col + matrix->cols * row];
}

void matrix_set(Matrix *matrix, size_t row, size_t col, double value) {
    matrix->arr[col + matrix->cols * row] = value;
}

void matrix_product0(Matrix *m1, Matrix *m2, Matrix *result) {
    for (size_t i = 0; i < m1->rows; ++i) {
        for (size_t j = 0; j < m1->cols; ++j) {
            double sum = 0;
            for (size_t k = 0; k < m1->cols; ++k)
                sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
            matrix_set(result, i, j, sum);
        }
    }
}

Matrix* matrix_product(Matrix *m1, Matrix *m2) {
    if (m1->cols != m2->rows) {
        errno = EDOM;
        return NULL;
    }
    Matrix *result = malloc(sizeof(Matrix));
        result->rows = m1->rows;
        result->cols = m2->cols;
        result->arr  = malloc(result->rows * result->cols * sizeof(double));
    
    matrix_product0(m1, m2, result);
    return result;
}

void matrix_mul0(Matrix *matrix, double factor, Matrix *result) {
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->cols; ++j) {
            matrix_set(result, i, j, matrix_get(matrix, i, j) * factor);
        }
    }
}

Matrix* matrix_mul(Matrix *matrix, double factor) {
    Matrix *result = malloc(sizeof(Matrix));
        result->rows = matrix->rows;
        result->cols = matrix->cols;
        result->arr  = malloc(result->rows * result->cols * sizeof(double));
    
    matrix_mul0(matrix, factor, result);
    return result;
}

void matrix_add0(Matrix *m1, Matrix *m2, Matrix *result) {
    for (size_t i = 0; i < m1->rows; ++i) {
        for (size_t j = 0; j < m1->cols; ++j) {
            matrix_set(result, i, j, matrix_get(m1, i, j) + matrix_get(m2, i, j));
        }
    }
}

Matrix* matrix_add(Matrix *m1, Matrix *m2) {
    if (m1->cols != m2->cols || m1->rows != m2->rows) {
        errno = EDOM;
        return NULL;
    }

    Matrix *result = malloc(sizeof(Matrix));
        result->rows = m1->rows;
        result->cols = m1->cols;
        result->arr  = malloc(result->rows * result->cols * sizeof(double));
    
    matrix_add0(m1, m2, result);
    return result;
}

int matrix_equals(Matrix *m1, Matrix *m2) {
    int equals = 1;
    for (size_t i = 0; equals && i < m1->rows; ++i) {
        for (size_t j = 0; equals && j < m1->cols; ++j) {
            equals = equals && matrix_get(m1, i, j) == matrix_get(m2, i, j);
        }
    }
    return equals;
}
