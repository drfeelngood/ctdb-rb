#ifndef CTDB_EXT
#define CTDB_EXT

#include <ruby.h>
#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
#endif
#include <ctdbsdk.h>

#include <ct_date.h>
#include <ct_time.h>
#include <ct_date_time.h>
#include <ct_session.h>
#include <ct_table.h>
#include <ct_field.h>
#include <ct_index.h>
#include <ct_segment.h>
#include <ct_record.h>

#define RUBY_CLASS(name) rb_const_get(rb_cObject, rb_intern(name))
#define RSEND(obj, meth) rb_funcall(obj, rb_intern(meth), 0)

#endif
