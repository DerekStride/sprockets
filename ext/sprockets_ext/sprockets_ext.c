#include <ruby.h>
#include <stdio.h>

static VALUE add_value_to_digest(VALUE val, VALUE digest) {
  VALUE sorted_hash, mDigestBuilder;
  long i;
  mDigestBuilder = rb_define_module_under(rb_define_module("Sprockets"), "DigestBuilder");
  switch (TYPE(val)) {
  	case T_FIXNUM:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Fixnum")));
      rb_funcall(digest, rb_intern("<<"), 1, rb_inspect(val));
  		break;
  	case T_STRING:
  		rb_funcall(digest, rb_intern("<<"), 1, val);
  		break;
    case T_SYMBOL:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Symbol")));
      rb_funcall(digest, rb_intern("<<"), 1, rb_sym_to_s(val));
      break;
    case T_NIL:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("NilClass")));
      break;
    case T_FALSE:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("FalseClass")));
      break;
    case T_TRUE:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("TrueClass")));
      break;
    case T_BIGNUM:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Bignum")));
      rb_funcall(digest, rb_intern("<<"), 1, rb_inspect(val));
      break;
    case T_ARRAY:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Array")));
      for (i = 0; i < RARRAY_LEN(val); i++) {
        add_value_to_digest(RARRAY_AREF(val, i), digest);
      }
      break;
    case T_HASH:
      rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Hash")));
      sorted_hash = rb_funcall(val, rb_intern("sort"), 0, NULL);
      for (i = 0; i< RARRAY_LEN(sorted_hash); i++) {
        add_value_to_digest(RARRAY_AREF(sorted_hash, i), digest);
      }
      break;
    default:
      if (rb_obj_class(val) == rb_define_class("Encoding", rb_cObject)) {
        rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Encoding")));
        rb_funcall(digest, rb_intern("<<"), 1, rb_funcall(val, rb_intern("name"), 0, NULL));
      } else if (rb_obj_class(val) == rb_define_class("Set", rb_cObject)) {
        rb_funcall(digest, rb_intern("<<"), 1, rb_const_get_at(mDigestBuilder, rb_intern("Set")));
        add_value_to_digest(rb_funcall(val, rb_intern("to_a"), 0, NULL), digest);
      } else {
        rb_raise(rb_eTypeError, "couldn't digest %s", RSTRING(val));
      }
      break;
  }
  return digest;
}

static VALUE digest_utils_build(VALUE self, VALUE rb_obj) {
  VALUE mDigestUtils, rb_digest;

  mDigestUtils = rb_define_module_under(rb_define_module("Sprockets"), "DigestUtils");

  rb_digest = rb_funcall(mDigestUtils, rb_intern("digest_class"), 0, NULL);

  return add_value_to_digest(rb_obj, rb_digest);
}

VALUE Init_sprockets_ext() {

  VALUE mDigestBuilder;

  mDigestBuilder = rb_define_module_under(rb_define_module("Sprockets"), "DigestBuilder");

  rb_define_const(mDigestBuilder, "Fixnum", rb_str_freeze(rb_str_new_cstr("Fixnum")));
  rb_define_const(mDigestBuilder, "NilClass", rb_str_freeze(rb_str_new_cstr("NilClass")));
  rb_define_const(mDigestBuilder, "FalseClass", rb_str_freeze(rb_str_new_cstr("FalseClass")));
  rb_define_const(mDigestBuilder, "TrueClass", rb_str_freeze(rb_str_new_cstr("TrueClass")));
  rb_define_const(mDigestBuilder, "Bignum", rb_str_freeze(rb_str_new_cstr("Bignum")));
  rb_define_const(mDigestBuilder, "Array", rb_str_freeze(rb_str_new_cstr("Array")));
  rb_define_const(mDigestBuilder, "Hash", rb_str_freeze(rb_str_new_cstr("Hash")));
  rb_define_const(mDigestBuilder, "Set", rb_str_freeze(rb_str_new_cstr("Set")));
  rb_define_const(mDigestBuilder, "Encoding", rb_str_freeze(rb_str_new_cstr("Encoding")));
  rb_define_const(mDigestBuilder, "Symbol", rb_str_freeze(rb_str_new_cstr("Symbol")));

  rb_define_singleton_method(mDigestBuilder, "build_digest", digest_utils_build, 1);

  return Qnil;
}
