#include <errno.h>
#include <stdio.h>
#include "math/matrix.h"

struct MatrixModule IMatrix = {
	.create         = &matrix_create,
	.init           = &matrix_init,
	.initf          = &matrix_initf,
	.destroy        = &matrix_destroy,
	.destroy_array  = &matrix_destroy_array,
	.get            = &matrix_get,
	.set            = &matrix_set,
	.product        = &matrix_product,
	.mul            = &matrix_mul,
	.add            = &matrix_add,
	.substract      = &matrix_substract,
	.vectorize      = &matrix_vectorize,
	.apply          = &matrix_apply,
	.equals         = &matrix_equals,
	.repmat         = &matrix_repmat,
	.hadamard       = &matrix_hadamard,
	.transpose      = &matrix_transpose
};

void matrix_create0(size_t rows, size_t columns, Matrix **out, bool force) {
	if (!*out) {
		*out = malloc(sizeof(Matrix));
		force = true;
	}
	if (force || !matrix_is_valid(*out)) {
		(*out)->self = *out;
		(*out)->arr  = malloc(rows * columns * sizeof(double));
		(*out)->rows = rows;
		(*out)->cols = columns;
	}
}

void matrix_create(size_t rows, size_t columns, Matrix** out) {
	if (!rows || !columns) {
		errno = EINVAL;
		return;
	}
	matrix_create0(rows, columns, out, true);
	for (size_t i = 0; i < rows * columns; ++i) {
		(*out)->arr[i] = 0;
	}
}

void matrix_init(size_t rows, size_t columns, const double* values, Matrix** out) {
	if (!rows || !columns) {
		errno = EINVAL;
		return;
	}
	matrix_create0(rows, columns, out, true);
	for (size_t i = 0; i < (*out)->rows; ++i) {
		for (size_t j = 0; j < (*out)->cols; ++j) {
			matrix_set(*out, i, j, values[j + (*out)->cols * i]);
		}
	}
}

void matrix_initf(size_t rows, size_t columns, double (*f)(size_t, size_t), Matrix** out) {
	if (!rows || !columns) {
		errno = EINVAL;
		return;
	}
	matrix_create0(rows, columns, out, true);
	for (size_t i = 0; i < (*out)->rows; ++i) {
		for (size_t j = 0; j < (*out)->cols; ++j) {
			matrix_set(*out, i, j, f(i, j));
		}
	}
}

void matrix_destroy(Matrix* matrix) {
	free(matrix->arr);
	free(matrix);
}

void matrix_destroy_array(Matrix* matrix, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		free(matrix[i].arr);
	}
	free(matrix);
}

bool matrix_is_valid(Matrix *matrix) {
	return matrix->self == matrix;
}

double matrix_get(Matrix *matrix, size_t row, size_t col) {
	return matrix->arr[col + matrix->cols * row];
}

void matrix_set(Matrix *matrix, size_t row, size_t col, double value) {
	matrix->arr[col + matrix->cols * row] = value;
}

void matrix_product(Matrix *m1, Matrix *m2, Matrix **result) {
	if (m1->cols != m2->rows) {
		errno = EDOM;
		return;
	}
	matrix_create0(m1->rows, m2->cols, result, false);
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m2->cols; ++j) {
			double sum = 0;
			for (size_t k = 0; k < m2->rows; ++k)
				sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
			matrix_set(*result, i, j, sum);
		}
	}
}

void matrix_mul(Matrix *matrix, double factor, Matrix **result) {
	matrix_create0(matrix->rows, matrix->cols, result, false);
	for (size_t i = 0; i < matrix->rows; ++i) {
		for (size_t j = 0; j < matrix->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(matrix, i, j) * factor);
		}
	}
}

void matrix_add(Matrix *m1, Matrix *m2, Matrix **result) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		errno = EDOM;
		return;
	}
	matrix_create0(m1->rows, m1->cols, result, false);
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) + matrix_get(m2, i, j));
		}
	}
}

void matrix_substract(Matrix *m1, Matrix *m2, Matrix **result) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		errno = EDOM;
		return;
	}
	matrix_create0(m1->rows, m1->cols, result, false);
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) - matrix_get(m2, i, j));
		}
	}
}

void matrix_hadamard(Matrix *m1, Matrix *m2, Matrix **result) {
	if (m1->cols != m2->cols || m1->rows != m2->rows) {
		errno = EDOM;
		return;
	}
	matrix_create0(m1->rows, m1->cols, result, false);
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) * matrix_get(m2, i, j));
		}
	}
}

void matrix_transpose(Matrix *m, Matrix **result) {
	matrix_create0(m->cols, m->rows, result, false);
	for (size_t i = 0; i < m->rows; ++i) {
		for (size_t j = 0; j < m->cols; ++j) {
			matrix_set(*result, j, i, matrix_get(m, i, j));
		}
	}
}

void matrix_vectorize(Matrix* m, Vector** v) {
	if (!((m->cols == 1) || (m->rows == 1))) {
		errno = EDOM;
		return;
	}

	IVector.init(m->cols == 1 ? m->rows : m->cols, m->arr, v);
}

void matrix_apply(Matrix* matrix, double (*f)(double), Matrix** result) {
	matrix_create0(matrix->rows, matrix->cols, result, false);
	for (size_t i = 0; i < matrix->rows; ++i) {
		for (size_t j = 0; j < matrix->cols; ++j) {
			matrix_set(*result, i, j, f(matrix_get(matrix, i, j)));
		}
	}
}

bool matrix_equals(Matrix *m1, Matrix *m2) {
	int equals = m1->cols == m2->cols && m1->rows == m2->rows;
	for (size_t i = 0; equals && i < m1->rows; ++i) {
		for (size_t j = 0; equals && j < m1->cols; ++j) {
			equals = equals && matrix_get(m1, i, j) == matrix_get(m2, i, j);
		}
	}
	return equals;
}

void matrix_repmat(Vector* v, size_t rows, size_t cols, Matrix** result) {
	matrix_create0(rows * v->size, cols, result, false);
	for (size_t i = 0; i < (*result)->rows; ++i) {
		for (size_t j = 0; j < (*result)->cols; ++j) {
			matrix_set(*result, i, j, IVector.at(v, i % v->size));
		}
	}
}
