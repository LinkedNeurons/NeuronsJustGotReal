#ifndef MARSHAL_H_
#define MARSHAL_H_

#include <stdio.h>
#include <stdint.h>
#include "njgr/math/vector.h"
#include "njgr/math/matrix.h"

void marshal_write_array(FILE *stream, void *buffer, size_t size, size_t num);
void marshal_write_string(FILE *stream, char *buffer);
void marshal_write_matrix(FILE *stream, Matrix *matrix);
void marshal_write_vector(FILE *stream, Vector *vector);
void marshal_read_array(FILE *stream, void **buffer, size_t *length);
void marshal_read_string(FILE *stream, char **buffer);
void marshal_read_matrix(FILE *stream, Matrix **matrix);
void marshal_read_vector(FILE *stream, Vector **vector);

#endif // MARSHAL_H_
