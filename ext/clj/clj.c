#include <ruby.h>
#include <assert.h>
#include "clj.h"


ID getc_sym;


typedef struct clj_rb_value_stack {
  VALUE *values;
  size_t count;
  size_t capacity;
} clj_rb_ValueStack;

static void value_stack_init(clj_rb_ValueStack *stack) {
  stack->count = 0;
  stack->capacity = 16;
  stack->values = ALLOC_N(VALUE, stack->capacity);
}

static VALUE value_stack_peek(clj_rb_ValueStack *stack) {
  assert(stack->count > 0);
  return stack->values[stack->count - 1];
}

static VALUE value_stack_pop(clj_rb_ValueStack *stack) {
  VALUE value = value_stack_peek(stack);
  stack->count--;
  return value;
}

static void value_stack_push(clj_rb_ValueStack *stack, VALUE value) {
  if (stack->count == stack->capacity) {
    stack->capacity *= 2;
    REALLOC_N(stack->values, VALUE, stack->capacity);
  }
  stack->values[stack->count] = value;
  stack->count++;
}

static void value_stack_free(clj_rb_ValueStack *stack) {
  xfree(stack->values);
}


typedef struct clj_rb_read_data {
  VALUE io;
  clj_rb_ValueStack stack;
} clj_rb_ReadData;

static wint_t reader_getwchar(const clj_Reader *reader) {
  wint_t c;
  clj_rb_ReadData *data = (clj_rb_ReadData*)reader->data;
  VALUE s = rb_funcall(data->io, getc_sym, 0);
  if (NIL_P(s)) {
    return WEOF;
  } else {
    c = (wint_t)StringValuePtr(s)[0]; //TODO: Encoding conversion!! AND LEAK?
    return c;
  }
}

static void reader_emit(const clj_Reader *reader, const clj_Node *node) {
  clj_rb_ReadData *data = (clj_rb_ReadData*)reader->data;
  if (reader->depth == 0) { //TODO: Or beginning of collection
    value_stack_push(&data->stack, INT2FIX(123));
  } else {
    //TODO: add to collection, end collection, etc
  }
}

static VALUE method_read_io(VALUE self, VALUE io) {
  clj_rb_ReadData data;
  clj_Reader reader;
  clj_Result result;
  char message[200];
  VALUE value;
  data.io = io;
  // Initialize
  value_stack_init(&data.stack);
  reader.emit = reader_emit;
  reader.getwchar = reader_getwchar;
  reader.data = &data;
  // Read
  result = clj_read(&reader);
  if (clj_is_error(result)) {
    value_stack_free(&data.stack);
    clj_read_error(message, &reader, result);
    rb_raise(rb_eArgError, message);
  }
  // Cleanup and return
  assert(data.stack.count == 1);
  value = value_stack_peek(&data.stack);
  value_stack_free(&data.stack);
  return value;
}

void Init_clj() {

  getc_sym = rb_intern("getc");

  VALUE module = rb_define_module("CLJ");
  rb_define_singleton_method(module, "read_io", method_read_io, 1);

}
