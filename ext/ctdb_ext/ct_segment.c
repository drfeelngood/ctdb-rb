#include <ctdb_ext.h>
#include <ct_segment.h>

VALUE cCTSegment;

extern VALUE mCT;
extern VALUE cCTError;
extern VALUE cCTField;

void
free_rb_ct_segment(void *ptr)
{
    ct_segment *segment = (ct_segment *)ptr;
    xfree(segment);
}

VALUE
rb_ct_segment_new(VALUE klass, CTHANDLE segment_handle)
{
    ct_segment *segment;
    VALUE obj;
    
    obj = Data_Make_Struct(klass, ct_segment, 0, free_rb_ct_segment, segment);
    segment->handle = segment_handle;

    rb_obj_call_init(obj, 0, NULL); // CT::Segment.initialize

    return obj;
}

/*
 * Retrieve the field object associated with the index segment.
 *
 * @return [CT::Field]
 */
static VALUE
rb_ct_segment_get_field(VALUE self)
{
    ct_segment *segment;
    CTSEG_MODE mode;
    CTHANDLE field;
    
    GetCTSegment(self, segment);

    mode = ctdbGetSegmentMode(segment->handle);
    if ( mode == CTSEG_REGSEG || mode == CTSEG_UREGSEG )
        field = ctdbGetSegmentPartialField(segment->handle);
    else
        field = ctdbGetSegmentField(segment->handle);

    if ( ! field )
        rb_raise(cCTError, "[%d] ctdbGetSegmentField failed.", 
            ctdbGetError(segment->handle));

    return rb_ct_field_new(cCTField, field);
}

// TODO:
static VALUE
rb_ct_segment_set_field(VALUE self, VALUE field)
{
    return self;
}

/*
 * Get the name of the field object associated with the index segment.
 *
 * @return [String]
 */
static VALUE
rb_ct_segment_get_field_name(VALUE self)
{
    ct_segment *segment;
    CTSEG_MODE mode;
    CTHANDLE field;
    pTEXT name;

    GetCTSegment(self, segment);

    mode = ctdbGetSegmentMode(segment->handle);
    if ( mode == CTSEG_REGSEG || mode == CTSEG_UREGSEG || CTSEG_INTSEG ||
            mode == CTSEG_SGNSEG || mode == CTSEG_FLTSEG )
        field = ctdbGetSegmentPartialField(segment->handle);
    else
        field = ctdbGetSegmentField(segment->handle);

    if ( ( name = ctdbGetFieldName(field)) == NULL )    
        rb_raise(cCTError, "[%d] ctdbGetFieldName failed.", 
            ctdbGetError(segment->handle));

    return rb_str_new_cstr(name);
}

static VALUE
rb_ct_segment_get_mode(VALUE self)
{
    ct_segment *segment;

    GetCTSegment(self, segment);
    
    return INT2FIX(ctdbGetSegmentMode(segment->handle));
}

// TODO:
static VALUE
rb_ct_segment_set_mode(VALUE self, VALUE mode)
{
    return self;
}

// TODO:
static VALUE
rb_ct_segment_move(VALUE self, VALUE index)
{
    return self;
}

static VALUE
rb_ct_segment_get_number(VALUE self)
{
    ct_segment *segment;
    VRLEN num;

    GetCTSegment(self, segment);

    if ( ctdbGetSegmentNbr(segment->handle, &num) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetSegmentNbr failed.", 
            ctdbGetError(segment->handle));

    return INT2FIX(num);
}

// TODO:
static VALUE
rb_ct_segment_get_status(VALUE self)
{
    return self;
}

/*
 * Hack to identify old school Segments
 */
static VALUE
rb_ct_segment_absolute_byte_offset(VALUE self)
{
    ct_segment *segment;
    CTSEG_MODE mode;

    GetCTSegment(self, segment);

    mode = ctdbGetSegmentMode(segment->handle);
    if ( mode == CTSEG_REGSEG || mode == CTSEG_UREGSEG || mode == CTSEG_INTSEG || 
         mode == CTSEG_SGNSEG || mode == CTSEG_FLTSEG )
        return Qtrue;
    else
        return Qfalse;
}

void
init_rb_ct_segment()
{
    cCTSegment = rb_define_class_under(mCT, "Segment", rb_cObject);

    rb_define_singleton_method(cCTSegment, "new", rb_ct_segment_new, 1);
    rb_define_method(cCTSegment, "field_name", rb_ct_segment_get_field_name, 0);
    rb_define_method(cCTSegment, "field", rb_ct_segment_get_field, 0);
    rb_define_method(cCTSegment, "field=", rb_ct_segment_set_field, 1);
    rb_define_method(cCTSegment, "mode", rb_ct_segment_get_mode, 0);
    rb_define_method(cCTSegment, "mode=", rb_ct_segment_set_mode, 1);
    rb_define_method(cCTSegment, "move", rb_ct_segment_move, 1);
    rb_define_method(cCTSegment, "number", rb_ct_segment_get_number, 0);
    rb_define_method(cCTSegment, "status", rb_ct_segment_get_status, 0);
    rb_define_method(cCTSegment, "absolute_byte_offset?", rb_ct_segment_absolute_byte_offset, 0);
}
