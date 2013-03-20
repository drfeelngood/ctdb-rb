#include <ctdb_ext.h>
#include <ct_field.h>

VALUE cCTField;

extern VALUE mCT;
extern VALUE cCTError;

static void
free_rb_ct_field(void *ptr)
{
    ct_field *field = (ct_field *)ptr;
    xfree(field);
}

VALUE
rb_ct_field_new(VALUE klass, CTHANDLE field_handle)
{
    VALUE obj;
    ct_field *field;

    obj = Data_Make_Struct(klass, ct_field, 0, free_rb_ct_field, field);
    field->handle = field_handle;

    rb_obj_call_init(obj, 0, NULL); // CT::Field.initialize

    return obj;
}

static VALUE
rb_ct_field_get_null_flag(VALUE self)
{
    ct_field *field;

    GetCTField(self, field);

    return ctdbGetFieldNullFlag(field->handle) == YES ? Qtrue : Qfalse;
}

// TODO:
static VALUE
rb_ct_field_get_default(VALUE self)
{
    return self;
}

// TODO:
static VALUE
rb_ct_field_set_default(VALUE self, VALUE value)
{
    return self;
}

// TODO:
static VALUE
rb_ct_field_inspect(VALUE self)
{
    return self;
}

/*
 * Retrieve the field length.
 *
 * @return [Fixnum] The field length.
 * @raise [CT::Error] ctdbGetFieldLength failed.
 */
static VALUE
rb_ct_field_get_length(VALUE self)
{
    ct_field *field;
    VRLEN length;

    GetCTField(self, field);

    if ( ( length = ctdbGetFieldLength(field->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetFieldLength failed.", 
            ctdbGetError(field->handle));

    return INT2FIX(length);
}

/*
 * Set the field length.
 *
 * @param [Fixnum] length The field length.
 * @raise [CT::Error] ctdbSetFieldLength failed.
 */
static VALUE
rb_ct_field_set_length(VALUE self, VALUE length)
{
    ct_field *field; 

    Check_Type(length, T_FIXNUM);

    GetCTField(self, field);

    if ( ctdbSetFieldLength(field->handle, FIX2INT(length)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldLength failed.", 
            ctdbGetError(field->handle));

    return self;
}

/*
 * Retreive the field name.
 *
 * @return [String]
 * @raise [CT::Error] ctdbGetFieldName failed.
 */
static VALUE
rb_ct_field_get_name(VALUE self)
{
    ct_field *field;
    pTEXT name;

    GetCTField(self, field);

    if ( ( name = ctdbGetFieldName(field->handle) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetFieldName failed.", 
            ctdbGetError(field->handle));

    return rb_str_new_cstr(name);
}

/*
 * Set the new field name.
 *
 * @param [String] name The field name.
 * @raise [CT::Error] ctdbSetFieldName failed.
 */
static VALUE
rb_ct_field_set_name(VALUE self, VALUE name)
{
    ct_field *field;

    Check_Type(name, T_STRING);

    GetCTField(self, field);

    if ( ctdbSetFieldName(field->handle, RSTRING_PTR(name)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldName failed.", 
            ctdbGetError(field->handle));

    return self;
}

/*
 * Retreive the field number in the table field list.
 *
 * @return [Fixnum]
 * @raise [CT::Error] ctdbGetFieldnbr failed.
 */
static VALUE
rb_ct_field_get_number(VALUE self)
{
    ct_field *field;
    NINT n;

    GetCTField(self, field);
    n = ctdbGetFieldNbr(field->handle);
    
    return INT2FIX(n);
}

/*
 * Retreive the fields precision. The field precision represents the total number 
 * of digits in a BCD number.
 * 
 * @return [Fixnum]
 */
static VALUE
rb_ct_field_get_precision(VALUE self)
{
    ct_field *field;

    GetCTField(self, field);

    return INT2FIX(ctdbGetFieldPrecision(field->handle));
}

/*
 * Set the field precision (maximun number of digits).
 *
 * @raise [CT::Error] ctdbSetFieldPrecision failed.
 */
static VALUE
rb_ct_field_set_precision(VALUE self, VALUE precision)
{ 
    ct_field *field;

    Check_Type(precision, T_FIXNUM);
    
    GetCTField(self, field);

    if ( ctdbSetFieldPrecision(field->handle, FIX2INT(precision)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldPrecision failed.", 
            ctdbGetError(field->handle));

    return self;
}

/* TODO:
 * Retrieve field properties such as name, type and length.
 *
 * @return [Hash]
 * @raise [CT::Error] ctdbGetFieldProperties failed.
 */
static VALUE
rb_ct_field_get_properties(VALUE self)
{
    // pCTHANDLE cth = CTH(self);
    // pTEXT n;
    // pCTDBTYPE t;
    // pVRLEN l;
    // printf("[%d]\n", __LINE__);
    // 
    // if(ctdbGetFieldProperties(*cth, &n, t, l) != CTDBRET_OK)
    //     rb_raise(cCTError, "[%d] ctdbGetFieldProperties failed.", ctdbGetError(*cth));
    // printf("[%d]\n", __LINE__);
    VALUE hash = rb_hash_new();
    // rb_hash_aset(hash, rb_str_new_cstr("name"), rb_str_new_cstr(*n));
    // rb_hash_aset(hash, rb_str_new_cstr("type"), FIX2INT(*t));
    // rb_hash_aset(hash, rb_str_new_cstr("length"), FIX2INT(*l));
    // printf("[%d]\n", __LINE__);
    return hash;
}

/*
 * Retrieve the field scale.  This represents the number of digits to the right
 * of the decimal point.
 *
 * @return [Fixnum]
 */
static VALUE
rb_ct_field_get_scale(VALUE self)
{
    ct_field *field;

    GetCTField(self, field);

    return INT2FIX(ctdbGetFieldScale(field->handle));
}

// TODO:
static VALUE
rb_ct_field_set_scale(VALUE self, VALUE scale)
{
    return self;
}

static VALUE
rb_ct_field_get_type(VALUE self)
{ 
    ct_field *field;
    CTDBTYPE type;

    GetCTField(self, field);

    if ( ( type = ctdbGetFieldType(field->handle) ) == 0 )
        rb_raise(cCTError, "[%d] ctdbGetFieldType failed.", 
            ctdbGetError(field->handle));

    return INT2FIX(type);
}

// TODO:
static VALUE
rb_ct_field_set_type(VALUE self, VALUE type)
{
    return self;
}

/*
 * Does the field contain a nuberic value?
 */
static VALUE
rb_ct_field_is_numeric(VALUE self)
{
    ct_field *field;

    GetCTField(self, field);

    return ( ((ctdbIsFieldNumeric(field->handle)) == YES) ? Qtrue : Qfalse );
}


void
init_rb_ct_field(void)
{
    cCTField = rb_define_class_under(mCT, "Field", rb_cObject);
    
    rb_define_method(cCTField, "allow_nil?", rb_ct_field_get_null_flag, 0);
    rb_define_method(cCTField, "default", rb_ct_field_get_default, 0);
    rb_define_method(cCTField, "default=", rb_ct_field_set_default, 1);
    rb_define_method(cCTField, "inspect", rb_ct_field_inspect, 0);
    rb_define_method(cCTField, "length", rb_ct_field_get_length, 0);
    rb_define_method(cCTField, "length=", rb_ct_field_set_length, 1);
    rb_define_method(cCTField, "name", rb_ct_field_get_name, 0);
    rb_define_method(cCTField, "name=", rb_ct_field_set_name, 1);
    rb_define_method(cCTField, "number", rb_ct_field_get_number, 0);
    rb_define_method(cCTField, "precision", rb_ct_field_get_precision, 0);
    rb_define_method(cCTField, "precision=", rb_ct_field_set_precision, 1);
    rb_define_method(cCTField, "properties", rb_ct_field_get_properties, 0);
    rb_define_method(cCTField, "scale", rb_ct_field_get_scale, 0);
    rb_define_method(cCTField, "scale=", rb_ct_field_set_scale, 1);
    rb_define_method(cCTField, "type", rb_ct_field_get_type, 0);
    rb_define_method(cCTField, "type=", rb_ct_field_set_type, 1);
    // rb_define_method(cCTField, "variable_length?", rb_ct_field_is_variable, 0);
    rb_define_method(cCTField, "numeric?", rb_ct_field_is_numeric, 0);
}
