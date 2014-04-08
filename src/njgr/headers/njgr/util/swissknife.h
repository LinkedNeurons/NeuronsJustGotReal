#ifndef SWISSKNIFE_H_
#define SWISSKNIFE_H_

#define bool _Bool
#define true 1
#define false 0

#define new_struct(var, type) var = malloc(sizeof(type)); *(var) = (type)
#define new_array(var, type, size) var = malloc(sizeof(type) * size); *(var) = (type[])

#endif // SWISSKNIFE_H_
