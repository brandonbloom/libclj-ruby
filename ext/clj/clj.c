#include <ruby.h>
#include <assert.h>
#include "clj.h"


//TODO: Verify encoding conversions. Search for mbtowc and wcsrtombs.


ID sym_getc;
ID sym_value;
ID sym_string;
ID sym_character;
ID sym_keyword;
ID sym_symbol;
ID sym_number;
ID sym_list_builder;
ID sym_vector_builder;
ID sym_map_builder;
ID sym_set_builder;
ID sym_begin_composite;
ID sym_atomic;
ID sym_end_composite;


typedef struct clj_rb_read_data {
  VALUE target;
  VALUE io;
} clj_rb_ReadData;

static void encoding_failure() {
  assert(0); //TODO: failure! throw? clj_reader_error? something else?
             // Be careful about throwing from inside an emit...
             // Also, should this take a clj_Reader and add line/col numbers?
}

static wint_t reader_getwchar(const clj_Reader *reader) {
  wint_t c;
  clj_rb_ReadData *data = (clj_rb_ReadData*)reader->data;
  VALUE value = rb_funcall(data->io, sym_getc, 0);
  const char *s;
  if (NIL_P(value)) {
    return WEOF;
  } else {
    s = StringValuePtr(value);
    if (-1 == mbtowc(&c, s, strlen(s))) {
      encoding_failure();
    }
    return c;
  }
}

static VALUE wcs2rb(const wchar_t *s) {
  size_t n = wcslen(s);
  size_t size = n * 6; // Max size of a UTF-8 character
  char bytes[size];
  if (wcstombs(bytes, s, size) != n) {
    encoding_failure();
  }
  return rb_str_new_cstr(bytes);
}

static void reader_emit(const clj_Reader *reader, const clj_Node *node) {
  clj_rb_ReadData *data = (clj_rb_ReadData*)reader->data;
  ID id;
  VALUE s, value;
  VALUE one = rb_str_new_cstr("1"); // TODO: Delete me
  if (clj_is_atomic(node->type)) {
    s = wcs2rb(node->value);
    //TODO: begin shit code
    switch (node->type) {
      case CLJ_NUMBER:
        value = rb_funcall(data->target, sym_number, 5,
                           s, one, one, Qfalse, Qfalse);
        break;
      case CLJ_STRING:
        value = rb_funcall(data->target, sym_string, 1, s);
        break;
      case CLJ_CHARACTER:
        value = rb_funcall(data->target, sym_character, 1, s);
        break;
      case CLJ_KEYWORD:
        value = rb_funcall(data->target, sym_keyword, 3, s, one, one);
        break;
      case CLJ_SYMBOL:
        value = rb_funcall(data->target, sym_symbol, 3, s, one, one);
        break;
      //TODO: DEFAULT CASE!
    }
    //TODO: END shit code
    rb_funcall(data->target, sym_atomic, 1, value);
  } else if (clj_is_end(node->type)) {
    rb_funcall(data->target, sym_end_composite, 0);
  } else {
    switch (node->type) {
      case CLJ_LIST:
        id = sym_list_builder;
        break;
      case CLJ_VECTOR:
        id = sym_vector_builder;
        break;
      case CLJ_MAP:
        id = sym_map_builder;
        break;
      case CLJ_SET:
        id = sym_set_builder;
        break;
      default:
        assert(0); //TODO: Real error
      //TODO: DEFAULT CASE!
    }
    value = rb_funcall(data->target, id, 0);
    rb_funcall(data->target, sym_begin_composite, 1, value);
  }
}

static VALUE method_read_ext(VALUE self, VALUE target, VALUE io) {
  clj_Reader reader;
  clj_Result result;
  char message[200];
  clj_rb_ReadData data = {target, io};
  // Initialize
  reader.emit = reader_emit;
  reader.getwchar = reader_getwchar;
  reader.data = &data;
  // Read
  result = clj_read(&reader);
  if (clj_is_error(result)) {
    clj_read_error(message, &reader, result);
    rb_raise(rb_eArgError, message);
  }
  return rb_funcall(target, sym_value, 0);
}

void Init_clj() {

  sym_getc = rb_intern("getc");
  sym_value = rb_intern("value");
  sym_string = rb_intern("string");
  sym_character = rb_intern("character");
  sym_keyword = rb_intern("keyword");
  sym_symbol = rb_intern("symbol");
  sym_number = rb_intern("number");
  sym_list_builder = rb_intern("list_builder");
  sym_vector_builder = rb_intern("vector_builder");
  sym_map_builder = rb_intern("map_builder");
  sym_set_builder = rb_intern("set_builder");
  sym_begin_composite = rb_intern("begin_composite");
  sym_atomic = rb_intern("atomic");
  sym_end_composite = rb_intern("end_composite");

  VALUE module = rb_define_module("CLJ");
  rb_define_singleton_method(module, "read_ext", method_read_ext, 2);

}
