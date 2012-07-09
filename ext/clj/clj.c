#include <ruby.h>

VALUE method_read(VALUE self) {
  int x = 10;
  return INT2NUM(x);
}

void Init_clj() {
  VALUE module = rb_define_module("CLJ");
  rb_define_singleton_method(module, "read", method_read, 0);
}
