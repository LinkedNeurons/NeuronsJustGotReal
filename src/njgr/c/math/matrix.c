#include <errno.h>
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
	.member_product = &matrix_member_product,
	.transpose      = &matrix_transpose
};

void matrix_create(size_t rows, size_t columns, Matrix** out) {
	if (!rows || !columns) {
		errno = EINVAL;
		return;
	}
	if (!*out) {
		*out = malloc(sizeof(Matrix));
	}
	(*out)->arr  = calloc(rows * columns, sizeof(double));
	(*out)->rows = rows;
	(*out)->cols = columns;
}

void matrix_init(size_t rows, size_t columns, const double* values, Matrix** out) {
	if (!rows || !columns) {
		errno = EINVAL;
		return;
	}
	if (!*out) {
		*out = malloc(sizeof(Matrix));
	}
	(*out)->arr  = malloc(rows * columns * sizeof(double));
	(*out)->rows = rows;
	(*out)->cols = columns;

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
	if (!*out) {
		*out = malloc(sizeof(Matrix));
	}
	(*out)->arr  = malloc(rows * columns * sizeof(double));
	(*out)->rows = rows;
	(*out)->cols = columns;

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
	if (!*result) {
		*result = malloc(sizeof(Matrix));
		(*result)->rows = m1->rows;
		(*result)->cols = m2->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			double sum = 0;
			for (size_t k = 0; k < m1->cols; ++k)
				sum += matrix_get(m1, i, k) * matrix_get(m2, k, j);
			matrix_set(*result, i, j, sum);
		}
	}
}

void matrix_mul(Matrix *matrix, double factor, Matrix **result) {
	if (!*result) {
		*result = malloc(sizeof(Matrix));
		(*result)->rows = matrix->rows;
		(*result)->cols = matrix->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < matrix->rows; ++i) {
		for (size_t j = 0; j < matrix->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(matrix, i, j) * factor);
		}
	}
}

void matrix_add(Matrix *m1, Matrix *m2, Matrix **result) {
	if (!*result) {
		if (m1->cols != m2->cols || m1->rows != m2->rows) {
			errno = EDOM;
			return;
		}

		*result = malloc(sizeof(Matrix));
		(*result)->rows = m1->rows;
		(*result)->cols = m1->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) + matrix_get(m2, i, j));
		}
	}
}

void matrix_substract(Matrix *m1, Matrix *m2, Matrix **result) {
	if (!*result) {
		if (m1->cols != m2->cols || m1->rows != m2->rows) {
			errno = EDOM;
			return;
		}

		*result = malloc(sizeof(Matrix));
		(*result)->rows = m1->rows;
		(*result)->cols = m1->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) - matrix_get(m2, i, j));
		}
	}
}

void matrix_member_product(Matrix *m1, Matrix *m2, Matrix **result) {
	
	if (m1->cols != m2->cols || m1->rows != m2->rows) {
		errno = EDOM;
		return;
	}

	if (!*result) {
		*result = malloc(sizeof(Matrix));
		(*result)->rows = m1->rows;
		(*result)->cols = m1->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < m1->rows; ++i) {
		for (size_t j = 0; j < m1->cols; ++j) {
			matrix_set(*result, i, j, matrix_get(m1, i, j) * matrix_get(m2, i, j));
		}
	}
}

void matrix_transpose(Matrix *m, Matrix **result) {
	
	if (m->cols != m->rows) {
		errno = EDOM;
		return;
	}

	if (!*result) {
		*result = malloc(sizeof(Matrix));
		(*result)->rows = m->rows;
		(*result)->cols = m->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	} else {
		if ((*result)->cols != m->cols || (*result)->rows != m->rows) {
			errno = EDOM;
			return;
		}
	}
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
	if (!*result) {
		*result = malloc(sizeof(Matrix));
		(*result)->rows = matrix->rows;
		(*result)->cols = matrix->cols;
		(*result)->arr  = malloc((*result)->rows * (*result)->cols * sizeof(double));
	}
	for (size_t i = 0; i < matrix->rows; ++i) {
		for (size_t j = 0; j < matrix->cols; ++j) {
			matrix_set(*result, i, j, f(matrix_get(matrix, i, j)));
		}
	}
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

void matrix_repmat(Vector* v, size_t rows, size_t cols, Matrix** result) {
	size_t rowsize = cols * v->size;
	size_t length = rows * rowsize;
	double *data = malloc(sizeof(double) * length);
	for (size_t i = 0; i < length; ++i) {
		data[i] = IVector.at(v, (i % rowsize) / cols);
	}
	if (*result) {
		(*result)->rows = rows * v->size;
		(*result)->cols = cols;
		free((*result)->arr);
		(*result)->arr  = data;
	} else {
		matrix_init(rows * v->size, cols, data, result);
	}
}
