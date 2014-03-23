#include <caml/alloc.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>

#include "math/vector.h"
#include "network.h"

value camel_feed(value network, value dist, value height) {
	CAMLparam3(network, dist, height);
	Vector* v = IVector.create(2);
	v->tab[0] = (double) Int_val(dist); v->tab[1] = (double) Int_val(height);
	Vector *out = NULL;
	Network *n = (Network*)Long_val(network);

	feed(n, v, &out);
	CAMLreturn(Val_bool(IVector.at(out, 0) >= 0));
}

value camel_getNeuralNetwork(value unit) {
	CAMLparam1(unit);
	Network* network = NULL; //TODO : create neural network here
	CAMLreturn(Val_long(network));
}