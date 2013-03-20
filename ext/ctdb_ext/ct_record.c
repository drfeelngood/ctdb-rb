#include <ctdb_ext.h>
#include <ct_record.h>

static VALUE cCTRecord;

extern VALUE mCT;
extern VALUE cCTError;
extern VALUE cCTIndex;
extern VALUE cCTDate;
extern VALUE cCTTime;

static void
free_rb_ct_record(void *ptr)
{
    ct_record *record = (ct_record *)ptr;

    ctdbFreeRecord(record->handle);
    xfree(record);
}

/*
 * Helper function for get_field_as_... calls.
 */
CTBOOL
ctdb_record_is_field_null(pCTHANDLE record_ptr, NINT field_num)
{
    return ctdbIsNullField(record_ptr, field_num);
}

VALUE
rb_ct_record_new(VALUE klass, VALUE rb_table)
{
    ct_record *record;
    ct_table *table;
    VALUE obj;
    
    Check_Type(rb_table, T_DATA);

    GetCTTable(rb_table, table);

    obj = Data_Make_Struct(klass, ct_record, 0, free_rb_ct_record, record);
    record->table_ptr = table->handle;
    
    if ( ( record->handle = ctdbAllocRecord(record->table_ptr)) == NULL)
        rb_raise(cCTError, "[%d] ctdbAllocRecord failed.", 
            ctdbGetError(record->table_ptr));

    VALUE argv[1] = { rb_table };
    rb_obj_call_init(obj, 1, argv); // CT::Record.initialize(table)
    
    return obj;
}

static VALUE
rb_ct_record_init(VALUE klass, VALUE rb_table)
{
    return klass;
}

/*
 * Check the internal new record flag.
 */
static VALUE
rb_ct_record_is_new(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);
  
    return ( (ctdbIsNewRecord(record->handle) == YES) ? Qtrue : Qfalse );
}

/*
 * Clear the record buffer.
 *
 * @raise [CT::Error] ctdbClearRecord failed.
 */
static VALUE
rb_ct_record_clear(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbClearRecord(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbClearRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Retrieve the number of records in the table.
 *
 * @return [Fixnum]
 * @raise [CT::Error] ctdbGetRecordCount failed.
 */
static VALUE
rb_ct_record_get_count(VALUE self)
{
    ct_record *record;
    CTUINT64 cnt;

    GetCTRecord(self, record);

    if ( ctdbGetRecordCount(record->handle, &cnt) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetRecordCount failed.", 
            ctdbGetError(record->handle));

    return INT2FIX(cnt);
}

/*
 * Retrieves the current default index name. When the record handle is initialized 
 * for the first time, the default index is set to zero.
 *
 * @return [CT::Index, nil]
 */
static VALUE
rb_ct_record_get_default_index(VALUE self)
{
    ct_record *record;
    NINT i; // Index number
    CTHANDLE ndx;

    GetCTRecord(self, record);

    if ( ( i = ctdbGetDefaultIndex(record->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetDefaultIndex failed.", 
                ctdbGetError(record->handle));

    if ( ( ndx = ctdbGetIndex(record->table_ptr, i) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetIndex failed.", 
            ctdbGetError(record->handle));

    return rb_ct_index_new(cCTIndex, ndx);
}

/*
 * Retrieves the current filter expression for the record.
 *
 * @return [String, nil] The expression or nil is no filters are active.
 */ 
static VALUE
rb_ct_record_get_filter(VALUE self)
{
    ct_record *record;
    pTEXT filter;

    GetCTRecord(self, record);
    filter = ctdbGetFilter(record->handle);

    return filter ? rb_str_new_cstr(filter) : Qnil;
}

/*
 * Set the filtering logic for the record.
 *
 * @param [String] filter The filter expression.
 * @raise [CT::Error] ctdbFilterRecord failed.
 */
static VALUE
rb_ct_record_set_filter(VALUE self, VALUE filter)
{
    ct_record *record;

    Check_Type(filter, T_STRING);

    GetCTRecord(self, record);

    if ( ctdbFilterRecord(record->handle, RSTRING_PTR(filter)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbFilterRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Indicate if the record is being filtered or not.
 */
static VALUE
rb_ct_record_is_filtered(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ( ctdbIsFilteredRecord(record->handle) == YES ? Qtrue : Qfalse );
}

/*
 * Find a record using the find mode as the find strategy.  Before using 
 * CT::Record#find:
 *  
 *
 * @param [Fixnum] mode The mode used to look for the record.
 * @raise [CT::Error] ctdbFindRecord failed.
 */
static VALUE
rb_ct_record_find(VALUE self, VALUE mode)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbFindRecord(record->handle, FIX2INT(mode)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbFindRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Get the first record on a table
 */
static VALUE
rb_ct_record_first(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ctdbFirstRecord(record->handle) == CTDBRET_OK ? self : Qnil;
}

/*
 * @see #first
 * @raise [CT::Error] ctdbFirstRecord failed.
 */
static VALUE
rb_ct_record_first_bang(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbFirstRecord(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbFirstRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 *
 *
 */
static VALUE
rb_ct_record_get_field(VALUE self, VALUE field_name)
{
    ct_record *record;
    CTHANDLE field;
    NINT field_nbr;
    CTDBTYPE field_type;
    VALUE rb_value = Qnil;

    Check_Type(field_name, T_STRING);

    GetCTRecord(self, record);

    if ( ( field = ctdbGetFieldByName(record->table_ptr, 
            RSTRING_PTR(field_name)) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetFieldByName failed", 
            ctdbGetError(record->handle));

    field_nbr  = ctdbGetFieldNbr(field);
    field_type = ctdbGetFieldType(field);
    
    switch(field_type){
        case CT_BOOL :
            rb_value = rb_funcall(self, rb_intern("get_field_as_bool"), 1, field_name); 
            break;
         case CT_TINYINT :
         case CT_SMALLINT :
         case CT_INTEGER :
         case CT_BIGINT :
            rb_value = rb_funcall(self, rb_intern("get_field_as_signed"), 1, field_name);
            break;
        case CT_UTINYINT :
        case CT_USMALLINT :
        case CT_UINTEGER :
            rb_value = rb_funcall(self, rb_intern("get_field_as_unsigned"), 1, field_name);
            break;
        case CT_CHARS :
        case CT_FPSTRING :
        case CT_F2STRING :
        case CT_F4STRING :
        case CT_PSTRING :
        case CT_VARBINARY :
        case CT_LVB :
        case CT_VARCHAR :
            rb_value = rb_funcall(self, rb_intern("get_field_as_string"), 1, field_name);
            break;
        case CT_DATE :
            rb_value = rb_funcall(self, rb_intern("get_field_as_date"), 1, field_name);
            break;
        case CT_FLOAT :
        case CT_DOUBLE :
            rb_value = rb_funcall(self, rb_intern("get_field_as_float"), 1, field_name);
            break;
        case CT_MONEY :
            rb_value = rb_funcall(self, rb_intern("get_field_as_money"), 1, field_name);
            break;
        case CT_TIME :
            rb_value = rb_funcall(self, rb_intern("get_field_as_time"), 1, field_name);
            break;
        case CT_TIMESTAMP :
        case CT_EFLOAT :
        //case CT_BINARY :
        case CT_NUMBER :
        case CT_CURRENCY :
            rb_raise(rb_eNotImpError, "TODO: get_field field for `%s'", RSTRING_PTR(field_name));
            break;
    }
    
    return rb_value;
}

/*
 *
 * 
 * @param [Fixnum, String] id The field number or name.
 * @return [Boolean]
 * @raise [CT::Error] ctdbGetFieldAsBool failed.
 */
static VALUE
rb_ct_record_get_field_as_bool(VALUE self, VALUE id)
{
    ct_record *record;
    NINT i; // Field number
    CTBOOL value;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdbGetFieldAsBool(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsBool failed.", 
            ctdbGetError(record->handle));

    return value == YES ? Qtrue : Qfalse;
}

/*
 *
 *
 * @param [Fixnum, String] id The field number or name.
 * @return [CT::Date]
 */
static VALUE
rb_ct_record_get_field_as_date(VALUE self, VALUE id)
{
    ct_record *record;
    ct_date *date;
    NINT i;             // Field number
    CTDATE value;       // Hopefully the Date retrieved from the field
    CTHANDLE field;     // Field handle
    VALUE obj;          // The Date as a Ruby object.
    
    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }
    field = ctdbGetField(record->table_ptr, i);

    if ( ctdbGetFieldAsDate(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsDate failed.", 
            ctdbGetError(record->handle));

    obj = Data_Make_Struct(cCTDate, ct_date, 0, free_rb_ct_date, date);
    date->value = value;
    date->type  = ctdbGetFieldDefaultDateType(field);

    return obj;
}

/*
 *
 * @return [CT::Time]
 */
static VALUE
rb_ct_record_get_field_as_time(VALUE self, VALUE id)
{
    ct_record *record;
    ct_time *time;
    CTHANDLE field;
    CTTIME value;
    NINT i;
    VALUE obj; 

    GetCTRecord(self, record);
    
    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }
    
    field = ctdbGetField(record->table_ptr, i);

    if ( ctdbGetFieldAsTime(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsDate failed.", 
            ctdbGetError(record->handle));

    obj = Data_Make_Struct(cCTDate, ct_time, 0, free_rb_ct_time, time);
    time->value = value;
    time->type  = ctdbGetDefTimeType(field);

    return obj;
}

/*
 * Retrieve field as Float value.
 *
 * @param [Fixnum, String] id The field number or name.
 * @return [Float]
 * @raise [CT::Error] ctdbGetFieldAsFloat failed.
 */
static VALUE
rb_ct_record_get_field_as_float(VALUE self, VALUE id)
{
    ct_record *record;
    NINT i; // Field number
    CTFLOAT value;
    
    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdbGetFieldAsFloat(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsFloat failed.", 
            ctdbGetError(record->handle));

    return rb_float_new(value);
}

/*
 * Retrieve the field as a signed value.
 *
 * @param [Fixnum, String] id The field number or name.
 * @return [Fixnum]
 * @raise [CT::Error] ctdbGetFieldAsSigned failed.
 */
static VALUE
rb_ct_record_get_field_as_signed(VALUE self, VALUE id)
{
    ct_record *record;
    NINT i; // Field number
    CTSIGNED value;
    
    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdb_record_is_field_null(record->handle, i) == YES ) return Qnil;

    if ( ctdbGetFieldAsSigned(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsSigned failed.", 
            ctdbGetError(record->handle));

    return INT2FIX(value);
}

/*
 * Retrieve the field as a string value.
 *
 * @param [Fixnum, String] id The field number or name.
 * @return [String]
 * @raise [CT::Error] ctdbGetFieldAsString failed.
 */
static VALUE
rb_ct_record_get_field_as_string(VALUE self, VALUE id)
{
    ct_record *record;
    NINT i; // Field number
    VRLEN len;

    GetCTRecord(self, record);
    
    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    // Retrieve the actual field size. The actual size of variable-length fields, 
    // such as CT_VARCHAR and CT_VARBINARY, may vary from the defined size.
    len = ctdbGetFieldDataLength(record->handle, i);

    TEXT value[len+1]; // Field value
    if ( ctdbGetFieldAsString(record->handle, i, value, 
                                            (VRLEN)sizeof(value)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsString failed.", 
                ctdbGetError(record->handle));

    return rb_str_new_cstr(value);
}

/*
 *
 * @param [Fixnum, String] id The field number or name.
 * @return [Fixnum]
 * @raise [CT::Error] ctdbGetFieldAsUnsigned failed.
 */
static VALUE
rb_ct_record_get_field_as_unsigned(VALUE self, VALUE id)
{
    ct_record *record;
    NINT i;
    CTUNSIGNED value;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle,
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdb_record_is_field_null(record->handle, i) == YES ) return Qnil;

    if ( ctdbGetFieldAsUnsigned(record->handle, i, &value) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetFieldAsUnsigned failed.", 
                ctdbGetError(record->handle));

    return UINT2NUM(value);
}

/*
 * Retrieve the CT::Record lock status.
 * @return [Symbol, nil]
 */
static VALUE
rb_ct_record_get_lock_mode(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);
    
    return INT2FIX(ctdbGetRecordLock(record->handle));
}

/*
 * Get the last record on a table.
 *
 * @return [CT::Record, nil]
 */
static VALUE
rb_ct_record_last(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ctdbLastRecord(record->handle) == CTDBRET_OK ? self : Qnil;
}

/*
 * @see #last
 * @raise [CT::Error] ctdbLastRecord failed.
 */
static VALUE
rb_ct_record_last_bang(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbLastRecord(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbLastRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Lock the current record.
 *
 * @param [Fixnum] mode The record lock mode.
 */
static VALUE
rb_ct_record_lock(VALUE self, VALUE mode)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbLockRecord(record->handle, FIX2INT(mode)) == CTDBRET_OK )
        return Qtrue;
    else
        return Qfalse;
}

/*
 * @see #lock
 * @raise [CT::Error] ctdbLockRecord failed.
 */
static VALUE
rb_ct_record_lock_bang(VALUE self, VALUE mode)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbLockRecord(record->handle, FIX2INT(mode)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbLockRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Has CT::Record#lock been executed on this resource.
 */
static VALUE
rb_ct_record_is_locked(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);
    
    return ((ctdbGetRecordLock(record->handle) != CTLOCK_FREE) ? Qtrue : Qfalse);
}

/*
 * Has CT::Record#lock(CT::LOCK_WRITE) been executed
 */
static VALUE
rb_ct_record_is_write_locked(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);
  
    return ((ctdbGetRecordLock(record->handle) == CTLOCK_WRITE) ? Qtrue : Qfalse);
}

/*
 * Has CT::Record#lock(CT::LOCK_READ) been executed
 */
static VALUE
rb_ct_record_is_read_locked(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ((ctdbGetRecordLock(record->handle) == CTLOCK_READ) ? Qtrue : Qfalse);
}

/*
 * Get the next record on a table.
 *
 * @return [CT::Record, nil]
 */
static VALUE
rb_ct_record_next(VALUE self)
{
    ct_record *record;
    CTDBRET rc;

    GetCTRecord(self, record);

    rc = ctdbNextRecord(record->handle);
    if ( rc != CTDBRET_OK && rc != INOT_ERR )
        rb_raise(cCTError, "[%d] ctdbNextRecord failed.", 
            ctdbGetError(record->handle));

    return rc == INOT_ERR ? Qnil : self;
}

/*
 * Get the current record offset
 * @return [Fixnum]
 */
static VALUE
rb_ct_record_position(VALUE self)
{
    ct_record *record;
    CTOFFSET i;
    CTDBRET rc;

    GetCTRecord(self, record);

    if ( ( rc = ctdbGetRecordPos(record->handle, &i)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetRecordPos failed.", rc);

    return INT2FIX(i);
}

/*
 * Get the previous record on a table.
 *
 * @return [CT::Record, nil] The previous record or nil if the record does not exist.
 */
static VALUE
rb_ct_record_prev(VALUE self)
{
    ct_record *record;
    CTDBRET rc;
    
    GetCTRecord(self, record);
    
    rc = ctdbPrevRecord(record->handle);
    if ( rc != CTDBRET_OK && rc != INOT_ERR )
        rb_raise(cCTError, "[%d] ctdbPrevRecord failed.", 
            ctdbGetError(record->handle));

    return rc == INOT_ERR ? Qnil : self;
}

/*
 * Seek to the given record offset.
 * @return [CT::Record]
 */
static VALUE
rb_ct_record_seek(VALUE self, VALUE offset)
{
    int i;
    ct_record *record;
    CTDBRET rc;

    GetCTRecord(self, record);

    i = FIX2INT(offset);
    if ( ( rc = ctdbSeekRecord(record->handle, (CTOFFSET)i) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSeekRecord failed.", rc);

    return self;
}

/*
 * Set the new default index by name or number.
 *
 * @param [String, Symbol, Fixnum] The index identifier.
 * @raise [CT::Error] ctdbSetDefaultIndexByName or ctdbSetDefaultIndex failed.
 */
static VALUE
rb_ct_record_set_default_index(VALUE self, VALUE identifier)
{
    ct_record *record;
    CTDBRET rc;

    GetCTRecord(self, record);

    switch ( rb_type(identifier) ) {
        case T_STRING :
        case T_SYMBOL :
            rc = ctdbSetDefaultIndexByName(record->handle, 
                                              RSTRING_PTR(RSTRING(identifier)));
            break;
        case T_FIXNUM :
            rc = ctdbSetDefaultIndex(record->handle, FIX2INT(identifier));
            break;
        default :
            rb_raise(rb_eArgError, "Unexpected value type `%s'", 
                     rb_obj_classname(identifier));
            break;
    }

    if ( rc != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetDefaultIndex failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 * Get a duplicate copy of the record
 * @return [CT::Record]
 */
static VALUE
rb_ct_record_duplicate(VALUE self)
{
    ct_record *record;
    ct_table *table;
    ct_record *record_copy;
    CTHANDLE ctrech;
    VALUE rb_table;
    VALUE obj;

    GetCTRecord(self, record);

    if ( ( ctrech = ctdbDuplicateRecord(record->handle) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbDuplicateRecord failed.",
            ctdbGetError(record->handle));
    
    obj = Data_Make_Struct(cCTRecord, ct_record, 0, free_rb_ct_record, record_copy);

    VALUE argv[1] = { rb_table };
    rb_obj_call_init(obj, 1, argv); // CT::Record.initialize(table)
    
    return obj;
}

/*
 *
 *
 *
 */
static VALUE
rb_ct_record_set_field(VALUE self, VALUE field_name, VALUE value)
{
    ct_record *record;
    CTHANDLE field;
    NINT field_nbr;
    CTDBTYPE field_type;
    
    Check_Type(field_name, T_STRING);

    GetCTRecord(self, record);

    if ( ( field = ctdbGetFieldByName(record->table_ptr, 
                                            RSTRING_PTR(field_name)) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetFieldByName failed", 
            ctdbGetError(record->handle));

    field_nbr  = ctdbGetFieldNbr(field);
    field_type = ctdbGetFieldType(field);
    switch ( field_type ) {
        case CT_BOOL :
            rb_funcall(self, rb_intern("set_field_as_bool"), 2, field_name, value); 
            break;
        case CT_TINYINT :
        case CT_SMALLINT :
        case CT_INTEGER :
        case CT_BIGINT :
            rb_funcall(self, rb_intern("set_field_as_signed"), 2, field_name, value);
            break;
        case CT_UTINYINT :
        case CT_USMALLINT :
        case CT_UINTEGER :
            rb_funcall(self, rb_intern("set_field_as_unsigned"), 2, field_name, value);
            break;
        case CT_CHARS :
        case CT_FPSTRING :
        case CT_F2STRING :
        case CT_F4STRING :
        case CT_PSTRING :
        case CT_VARBINARY :
        case CT_LVB :
        case CT_VARCHAR :
            rb_funcall(self, rb_intern("set_field_as_string"), 2, field_name, value);
            break;
        case CT_DATE :
            rb_funcall(self, rb_intern("set_field_as_date"), 2, field_name, value);
            break;
        case CT_MONEY :
        case CT_TIME :
        case CT_FLOAT :
        case CT_DOUBLE :
        case CT_TIMESTAMP :
        case CT_EFLOAT :
        // NOTE: I kept getting duplicate entry for this case CT_BINARY :
        case CT_NUMBER :
        case CT_CURRENCY :
            rb_raise(rb_eNotImpError, "TODO: set_field for `%s'", RSTRING_PTR(field_name));
            break;
    }

    return self;
}
// static VALUE
// rb_ct_record_set_field_as_binary(VALUE self, VALUE num, VALUE value){}

// static VALUE
// rb_ct_record_set_field_as_blob(VALUE self, VALUE num, VALUE value){}

/*
 *
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [Boolean] value
 * @raise [CT::Error] ctdbSetFieldAsBool failed.
 */
static VALUE
rb_ct_record_set_field_as_bool(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;
    CTBOOL cval;
    
    GetCTRecord(self, record);

    if ( rb_type(value) != T_TRUE && rb_type(value) != T_FALSE )
        rb_raise(rb_eArgError, "Unexpected value type `%s' for CT_BOOL", 
                 rb_obj_classname(value));
    
    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                                                      RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    cval = (rb_type(value) == T_TRUE ? YES : NO);

    if ( ctdbSetFieldAsBool(record->handle, i, cval) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsBool falied.", 
            ctdbGetError(record->handle));

    return self;
}

// static VALUE
// rb_ct_record_set_field_as_currency(VALUE self, VALUE num, VALUE value){}

/*
 *
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [Date] value
 */
static VALUE
rb_ct_record_set_field_as_date(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;
    CTDATE ctdate;
    CTDBRET rc;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if (( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( value == Qnil ) {
      if ( ctdbSetFieldAsUnsigned(record->handle, i, 0) != CTDBRET_OK )
            rb_raise(cCTError, "[%d] ctdbSetFieldAsUnsigned failed.",
                ctdbGetError(record->handle));
    } else {
        rc = ctdbDatePack(&ctdate, 
            FIX2INT(rb_funcall(value, rb_intern("year"), 0)), 
            FIX2INT(rb_funcall(value, rb_intern("month"), 0)), 
            FIX2INT(rb_funcall(value, rb_intern("day"), 0)));

        if ( rc != CTDBRET_OK )
            rb_raise(cCTError, "[%d] ctdbDatePack failed.", rc);
        
        if ( ctdbSetFieldAsDate(record->handle, i, ctdate) != CTDBRET_OK )
            rb_raise(cCTError, "[%d] ctdbSetFieldAsDate failed.", 
                ctdbGetError(record->handle));
    }

    return self;
}

// static VALUE
// rb_ct_record_set_field_as_datetime(VALUE self, VALUE num, VALUE value){}

static VALUE
rb_ct_record_set_field_as_float(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;

    Check_Type(value, T_FLOAT);
    
    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdbSetFieldAsFloat(record->handle, i, RFLOAT_VALUE(value)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsFloat failed.", 
            ctdbGetError(record->handle));

    return self;
}

// static VALUE
// rb_ct_record_set_field_as_money(VALUE self, VALUE num, VALUE value){}

// static VALUE
// rb_ct_record_set_field_as_number(VALUE self, VALUE num, VALUE value){}

/*
 * Set field as CTSIGNED type value.
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [Integer] value
 * @raise [CT::Error] ctdbSetFieldAsSigned failed.
 */ 
static VALUE
rb_ct_record_set_field_as_signed(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                                                      RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdbSetFieldAsSigned(record->handle, i, FIX2INT(value)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsSigned failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 *
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [String] value
 * @raise [CT::Error] ctdbSetFieldAsString failed.
 */
static VALUE
rb_ct_record_set_field_as_string(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;
    CTHANDLE f;
    VRLEN len;

    Check_Type(value, T_STRING);
    
    GetCTRecord(self, record);
    
    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( !( f = ctdbGetField(record->table_ptr, i)) )
        rb_raise(cCTError, "[%d] ctdbGetField failed.", 
            ctdbGetError(record->handle));

    if ( NIL_P(value) && ctdbGetFieldNullFlag(f) == NO )
        rb_raise(cCTError, "Field `%s' cannot be NULL.", ctdbGetFieldName(f));

    if ( ctdbIsVariableField(record->handle, i) == NO ) {
        // Pad string to the fixed length.
        len = ctdbGetFieldLength(f);

        // TODO: Implement dynamic field padding with ctdbGetPadChar(*ct->table_ptr, &padc, &delimc);
        while(RSTRING_LEN(value) < len-1)
            rb_str_cat2(value, " ");
    }
    
    if ( ctdbSetFieldAsString(record->handle, i, 
                      (RTEST(value) ? RSTRING_PTR(value) : '\0' )) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsString failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 *
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [Time] value
 */
static VALUE
rb_ct_record_set_field_as_time(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;
    CTDATE cttime;
    CTDBRET rc;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                                                      RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    rc = ctdbTimePack(&cttime,
            FIX2INT(rb_funcall(value, rb_intern("hour"), 0)), 
            FIX2INT(rb_funcall(value, rb_intern("min"), 0)), 
            FIX2INT(rb_funcall(value, rb_intern("sec"), 0)));

    if ( ctdbSetFieldAsTime(record->handle, i, cttime) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsTime failed.", 
                ctdbGetError(record->handle));

    return self;
}

/*
 *
 * Set field as CTUNSIGNED type value.
 *
 * @param [Fixnum, String] id Field number or name.
 * @param [Integer] value
 * @raise [CT::Error] ctdbSetFieldAsUnsigned failed.
 */
static VALUE
rb_ct_record_set_field_as_unsigned(VALUE self, VALUE id, VALUE value)
{
    ct_record *record;
    NINT i;

    GetCTRecord(self, record);

    switch ( rb_type(id) ) {
        case T_STRING :
            if ( ( i = ctdbGetFieldNumberByName(record->handle, 
                    RSTRING_PTR(id)) ) == -1 )
                rb_raise(cCTError, "[%d] ctdbGetFieldNumberByName failed.", 
                    ctdbGetError(record->handle));
            break;
        case T_FIXNUM :
            i = FIX2INT(id);
            break;
        default:
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(id));
            break;
    }

    if ( ctdbSetFieldAsUnsigned(record->handle, i, FIX2INT(value)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetFieldAsUnsigned failed.", 
            ctdbGetError(record->handle));

    return self;
}

// static VALUE
// rb_ct_record_set_field_as_utf16(VALUE self, VALUE num, VALUE value){}

/*
 * Indicates if record set is active or not.
 */
static VALUE
rb_ct_record_is_set(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ( ctdbIsRecordSetOn(record->handle) == YES ? Qtrue : Qfalse );
}

/*
 * Enable a new record set.  The target key is build from the contents of the 
 * record buffer.
 */
static VALUE
rb_ct_record_set_on(VALUE self, VALUE bytes)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbRecordSetOn(record->handle, FIX2INT(bytes)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbRecordSetOn failed.", 
                ctdbGetError(record->handle));

    return self;
}

/*
 * Disable and free an existing record set.
 */
static VALUE
rb_ct_record_set_off(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbRecordSetOff(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbRecordSetOff failed.", 
            ctdbGetError(record->handle));

    return self;
}

/*
 *
 */
static VALUE
rb_ct_record_unlock(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    return ( (ctdbUnlockRecord(record->handle) == CTDBRET_OK) ? Qtrue : Qfalse );
}

/*
 *
 *
 */
static VALUE
rb_ct_record_unlock_bang(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbUnlockRecord(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbUnlockRecord failed.", 
            ctdbGetError(record->handle));

    return Qtrue;
}

/*
 * Create or update an existing record.
 *
 * @raise [CT::Error] ctdbWriteRecord failed.
 */
static VALUE
rb_ct_record_write_bang(VALUE self)
{
    ct_record *record;

    GetCTRecord(self, record);

    if ( ctdbWriteRecord(record->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbWriteRecord failed.", 
            ctdbGetError(record->handle));

    return self;
}
  
void
init_rb_ct_record()
{
    cCTRecord = rb_define_class_under(mCT, "Record", rb_cObject);
    
    rb_define_singleton_method(cCTRecord, "new", rb_ct_record_new, 1);
    rb_define_method(cCTRecord, "initialize", rb_ct_record_init, 1);
    rb_define_method(cCTRecord, "new_record?", rb_ct_record_is_new, 0);
    rb_define_method(cCTRecord, "clear", rb_ct_record_clear, 0);
    rb_define_method(cCTRecord, "count", rb_ct_record_get_count, 0);
    rb_define_method(cCTRecord, "default_index", rb_ct_record_get_default_index, 0);
    rb_define_method(cCTRecord, "default_index=", rb_ct_record_set_default_index, 1);
    rb_define_method(cCTRecord, "duplicate", rb_ct_record_duplicate, 0);
    rb_define_method(cCTRecord, "filter", rb_ct_record_get_filter, 0);
    rb_define_method(cCTRecord, "filter=", rb_ct_record_set_filter, 1);
    rb_define_method(cCTRecord, "filtered?", rb_ct_record_is_filtered, 0);
    rb_define_method(cCTRecord, "find", rb_ct_record_find, 1);
    rb_define_method(cCTRecord, "first", rb_ct_record_first, 0);
    rb_define_method(cCTRecord, "first!", rb_ct_record_first_bang, 0);
    rb_define_method(cCTRecord, "get_field", rb_ct_record_get_field, 1);
    rb_define_alias(cCTRecord,  "[]", "get_field");
    rb_define_method(cCTRecord, "get_field_as_bool", rb_ct_record_get_field_as_bool, 1);
    rb_define_method(cCTRecord, "get_field_as_date", rb_ct_record_get_field_as_date, 1);
    rb_define_method(cCTRecord, "get_field_as_float", rb_ct_record_get_field_as_float, 1);
    rb_define_alias(cCTRecord, "get_field_as_money", "get_field_as_float");
    rb_define_method(cCTRecord, "get_field_as_signed", rb_ct_record_get_field_as_signed, 1);
    rb_define_method(cCTRecord, "get_field_as_string", rb_ct_record_get_field_as_string, 1);
    rb_define_method(cCTRecord, "get_field_as_time", rb_ct_record_get_field_as_time, 1);
    rb_define_method(cCTRecord, "get_field_as_unsigned", rb_ct_record_get_field_as_unsigned, 1);
    rb_define_method(cCTRecord, "get_lock_mode", rb_ct_record_get_lock_mode, 0);
    rb_define_method(cCTRecord, "last", rb_ct_record_last, 0);
    rb_define_method(cCTRecord, "last!", rb_ct_record_last_bang, 0);
    rb_define_method(cCTRecord, "lock", rb_ct_record_lock, 1);
    rb_define_method(cCTRecord, "lock!", rb_ct_record_lock_bang, 1);
    rb_define_method(cCTRecord, "locked?", rb_ct_record_is_locked, 0);
    rb_define_method(cCTRecord, "write_locked?", rb_ct_record_is_write_locked, 0);
    rb_define_method(cCTRecord, "read_locked?", rb_ct_record_is_read_locked, 0);
    rb_define_method(cCTRecord, "next", rb_ct_record_next, 0);
    rb_define_method(cCTRecord, "position", rb_ct_record_position, 0);
    rb_define_alias(cCTRecord,  "offset", "position");
    rb_define_method(cCTRecord, "prev", rb_ct_record_prev, 0);
    rb_define_method(cCTRecord, "seek", rb_ct_record_seek, 1);
    rb_define_method(cCTRecord, "set_field", rb_ct_record_set_field, 2);
    rb_define_alias(cCTRecord,  "[]=", "set_field");
    rb_define_method(cCTRecord, "set_field_as_bool", rb_ct_record_set_field_as_bool, 2);
    rb_define_method(cCTRecord, "set_field_as_date", rb_ct_record_set_field_as_date, 2);
    rb_define_method(cCTRecord, "set_field_as_float", rb_ct_record_set_field_as_float, 2);
    rb_define_method(cCTRecord, "set_field_as_signed", rb_ct_record_set_field_as_signed, 2);
    rb_define_method(cCTRecord, "set_field_as_string", rb_ct_record_set_field_as_string, 2);
    rb_define_method(cCTRecord, "set_field_as_time", rb_ct_record_set_field_as_time, 2);
    rb_define_method(cCTRecord, "set_field_as_unsigned", rb_ct_record_set_field_as_unsigned, 2);
    rb_define_method(cCTRecord, "set?", rb_ct_record_is_set, 0);
    rb_define_method(cCTRecord, "set_on", rb_ct_record_set_on, 1);
    rb_define_method(cCTRecord, "set_off", rb_ct_record_set_off, 0);
    rb_define_method(cCTRecord, "unlock", rb_ct_record_unlock, 0);
    rb_define_method(cCTRecord, "unlock!", rb_ct_record_unlock_bang, 0);
    rb_define_method(cCTRecord, "write!", rb_ct_record_write_bang, 0);
}
