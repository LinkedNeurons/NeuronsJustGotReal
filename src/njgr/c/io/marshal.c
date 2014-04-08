#include <string.h>
#include "njgr/util/swissknife.h"
#include "njgr/io/marshal.h"

void marshal_write_array(FILE *stream, void *buffer, size_t size, size_t num) {
	size_t length = size * num;
	fwrite(&length, sizeof(size_t), 1, stream);
	fwrite(buffer, size, num, stream);
}

void marshal_write_string(FILE *stream, char *buffer) {
	size_t length = strlen(buffer);
	marshal_write_array(stream, buffer, sizeof(char), length);
}

void marshal_write_matrix(FILE *stream, Matrix *matrix) {
	fwrite(&(matrix->rows), sizeof(size_t), 1, stream);
	fwrite(&(matrix->cols), sizeof(size_t), 1, stream);
	fwrite(matrix->arr, sizeof(double), matrix->rows * matrix->cols, stream);
}

void marshal_write_vector(FILE *stream, Vector *vector) {
	marshal_write_array(stream, vector->tab, sizeof(double), vector->size);
}

void marshal_read_array(FILE *stream, void **buffer, size_t *length) {
	fread(length, sizeof(size_t), 1, stream);
	*buffer = malloc(*length);
	fread(*buffer, 1, *length, stream);
}

void marshal_read_string(FILE *stream, char **buffer) {
	size_t length = 0;
	fread(&length, sizeof(size_t), 1, stream);
	*buffer = malloc(length + 1);
	(*buffer)[length] = '\0';
	fread(*buffer, 1, length, stream);
}

void marshal_read_matrix(FILE *stream, Matrix **matrix) {
	size_t rows = 0, cols = 0;
	size_t read = 0;
	read += fread(&rows, sizeof(size_t), 1, stream);
	read += fread(&cols, sizeof(size_t), 1, stream);
	if (read != 2) return;

	double *buffer = malloc(rows * cols * sizeof(double));
	read = fread(buffer, sizeof(double), rows * cols, stream);
	if (read != rows * cols) {
		free(buffer);
		return;
	}
	new_struct(*matrix, Matrix) {
		.rows = rows,
		.cols = cols,
		.arr  = buffer
	};
}

void marshal_read_vector(FILE *stream, Vector **vector) {
	size_t length = 0;
	double *buffer = NULL;
	marshal_read_array(stream, (void**) &buffer, &length);
	new_struct(*vector, Vector) {
		.size = length / sizeof(double),
		.tab  = buffer
	};
}
