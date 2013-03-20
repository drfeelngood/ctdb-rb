#include <ctdb_ext.h>
#include <ct_index.h>

VALUE cCTIndex;

extern VALUE mCT;
extern VALUE cCTError;
extern VALUE cCTSegment;

static void
free_rb_ct_index(void *ptr)
{
   ct_index *index = (ct_index *)ptr;
   xfree(index);
}

VALUE
rb_ct_index_new(VALUE klass, CTHANDLE index_handle)
{
    VALUE obj;
    ct_index *index;

    obj = Data_Make_Struct(klass, ct_index, 0, free_rb_ct_index, index);
    index->handle = index_handle;

    rb_obj_call_init(obj, 0, NULL); // CT::Index.initialize

    return obj;
}

static VALUE
rb_ct_index_add_segment(VALUE self, VALUE rb_field, VALUE mode)
{
    ct_index *index;
    ct_field *field;

    Check_Type(rb_field, T_DATA);

    GetCTIndex(self, index);
    GetCTField(rb_field, field);

    // TODO: Implement segment mode
    if ( ! ctdbAddSegment(index->handle, field->handle, CTSEG_SCHSEG) )
        rb_raise(cCTError, "[%d] ctdbAddSegment failed.", 
            ctdbGetError(index->handle));

    return self;
}

/*
 * Check if the duplicate flag is set to false.
 */
static VALUE
rb_ct_index_get_allow_dups(VALUE self)
{
    ct_index *index;

    GetCTIndex(self, index);

    return ctdbGetIndexDuplicateFlag(index->handle) == YES ? Qfalse : Qtrue;
}

/*
 * Set the allow duplicate flag for this index.
 *
 * @param [Boolean] value
 * @raise [CT::Error] ctdbSetIndexDuplicateFlag failed.
 */
static VALUE
rb_ct_index_set_allow_dups(VALUE self, VALUE value)
{
    ct_index *index;  
    CTBOOL v;
  
    if ( rb_type(value) != T_TRUE && rb_type(value) != T_FALSE )
        rb_raise(rb_eArgError, "Unexpected value type `%s' for CT_BOOL", 
                 rb_obj_classname(value));

    GetCTIndex(self, index);
    v = (value == T_TRUE ? YES : NO);
    
    if ( ctdbSetIndexDuplicateFlag(index->handle, v) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetIndexDuplicateFlag failed.", 
            ctdbGetError(index->handle));

    return self;
}

/*
 * Retrieve the key type for this index.
 *
 * @return [Fixnum]
 */
static VALUE
rb_ct_index_get_key_type(VALUE self)
{
    ct_index *index;

    GetCTIndex(self, index);

    return INT2FIX(ctdbGetIndexKeyType(index->handle));
}

/*
 * Retrieve the index name
 *
 * @return [String, nil]
 */
static VALUE
rb_ct_index_get_name(VALUE self)
{
    ct_index *index;
    pTEXT name;
  
    GetCTIndex(self, index);
    name = ctdbGetIndexName(index->handle);
    
    return name ? rb_str_new_cstr(name) : Qnil;
}

/*
 * Retrieve the given index segment
 * @param [Fixnum, String] id Either the segment number of field name associated
 * with the segment.
 * @return [CT::Segment]
 */
static VALUE
rb_ct_index_get_segment(VALUE self, VALUE id)
{
    ct_index *index;
    CTHANDLE segh;
    VALUE segments;
    VALUE segment;
    VALUE name;

    if ( rb_type(id) == T_FIXNUM ) {
        GetCTIndex(self, index);
        if ( (segh = ctdbGetSegment(index->handle, FIX2INT(id)) ) == NULL )
            rb_raise(cCTError, "[%d] ctdbGetSegment failed.", 
                ctdbGetError(index->handle));

        return rb_ct_segment_new(cCTSegment, segh);
    
    } else if ( rb_type(id) == T_STRING ) {
        segments = rb_funcall(self, rb_intern("segments"), 0);
        while ( (segment = rb_ary_pop(segments)) ) {
            name = rb_funcall(segment, rb_intern("field_name"), 0);
            if ( rb_funcall(name, rb_intern("=="), 1, id) == Qtrue )
                return segment;
        }
    } 
     
    return Qnil;
}

/*
 * Retrieve an Array of all Index Segments.
 * @return [Array]
 */
static VALUE
rb_ct_index_get_segments(VALUE self)
{
    ct_index *index;
    int j;
    VRLEN cnt;
    VALUE segments;

    GetCTIndex(self, index);
    segments = rb_ary_new();

    if ( ( cnt = ctdbGetIndexSegmentCount(index->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetIndexSegmentCount failed.", 
                ctdbGetError(index->handle));

    for ( j = 0; j < cnt; j++ )
        rb_ary_push(segments, 
                rb_ct_segment_new(cCTSegment, ctdbGetSegment(index->handle, j))); 
    
    return segments;
}

/*
 * Retrieve the key length for this index.
 *
 * @return [Fixnum, nil]
 */
static VALUE
rb_ct_index_get_key_length(VALUE self)
{
    ct_index *index;
    VRLEN len;

    GetCTIndex(self, index);
    len = ctdbGetIndexKeyLength(index->handle);
    
    return len == -1 ? Qnil : INT2FIX(len);
}



void
init_rb_ct_index()
{
    cCTIndex = rb_define_class_under(mCT, "Index", rb_cObject);
    
    // rb_define_singleton_method(cCTTable, "new", rb_ct_index_init, 4);
    rb_define_method(cCTIndex, "add_segment", rb_ct_index_add_segment, 2);
    rb_define_method(cCTIndex, "allow_dups?", rb_ct_index_get_allow_dups, 0);
    rb_define_method(cCTIndex, "allow_dups=", rb_ct_index_set_allow_dups, 1);
    rb_define_method(cCTIndex, "get_segment", rb_ct_index_get_segment, 1);
    rb_define_method(cCTIndex, "key_length", rb_ct_index_get_key_length, 0);
    rb_define_method(cCTIndex, "key_type",   rb_ct_index_get_key_type, 0);
    rb_define_method(cCTIndex, "name", rb_ct_index_get_name, 0);
    rb_define_method(cCTIndex, "segments", rb_ct_index_get_segments, 0);
    // rb_define_method(cCTIndex, "number", rb_ct_index_get_number, 0);
}
