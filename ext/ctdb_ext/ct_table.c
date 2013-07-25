#include <ctdb_ext.h>
#include <ct_table.h>

static VALUE cCTTable;
extern VALUE mCT;
extern VALUE cCTError;
extern VALUE cCTSession;
extern VALUE cCTIndex;
extern VALUE cCTField;

void
free_rb_ct_table(void *ptr)
{
    ct_table *table = (ct_table *)ptr;

    if ( ctdbIsActiveTable(table->handle) )
        ctdbCloseTable(table->handle);

    ctdbFreeTable(table->handle);
    xfree(table);
}

/*
 * @param [CT::Session]
 */
VALUE
rb_ct_table_new(VALUE klass, VALUE rb_session)
{
    ct_table *table;
    ct_session *session;
    VALUE obj;
    
    Check_Type(rb_session, T_DATA);

    GetCTSession(rb_session, session);

    obj = Data_Make_Struct(klass, ct_table, 0, free_rb_ct_table, table);
    if ( ( table->handle = ctdbAllocTable(session->handle) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbAllocTable failed", 
            ctdbGetError(session->handle));

    VALUE argv[1] = { rb_session };
    rb_obj_call_init(obj, 1, argv); // CT::Table.initialize(session)

    return obj;
}

static VALUE
rb_ct_table_init(VALUE klass, VALUE rb_session)
{
    return klass;
}

/*
 * Add a new field to the table.
 *
 * @param [String] name Field name
 * @param [Fixnum] type Field type
 * @param [Integer] length Field length
 * @param [Hash] opts Optional field flags.
 * @options opts [Boolean] :allow_nil Set the field null flag.
 * @options opts [Object] :default Set the fields default value.
 * @options opts [Fixnum] :scale The number of digits to the right of the 
 * decimal point.
 * @options opts [Fixnum] :precision The maximun number of digits.
 * @raise [CT::Error]
 */
static VALUE
rb_ct_table_add_field(VALUE self, VALUE name, VALUE type, VALUE length)
{
    ct_table *table;
    CTHANDLE field;

    Check_Type(name,   T_STRING);
    Check_Type(type,   T_FIXNUM);
    Check_Type(length, T_FIXNUM);
    
    GetCTTable(self, table);

    field = ctdbAddField(table->handle, RSTRING_PTR(name), 
                                        FIX2INT(type),
                                        FIX2INT(length));

    if ( field == NULL )
        rb_raise(cCTError, "[%d] ctdbAddField failed.", 
            ctdbGetError(table->handle));

    return rb_ct_field_new(cCTField, field);
}

/*
 * Add a new index the the table.
 *
 * @param [String] name Index name
 * @param [Fixnum] type Key type
 * @param [Hash] opts
 * @options [Boolean] :allow_dups Indication if the index allows duplicate keys.
 * @options [Boolean] :allow_nil Indidication if the index allows null keys.
 * @raise [CT::Error] ctdbAddIndex failed.
 */
static VALUE
rb_ct_table_add_index(VALUE self, VALUE name, VALUE type)/*, VALUE opts)*/
{
    ct_table *table;
    CTHANDLE index;
    CTBOOL allow_dups_flag = NO;
    CTBOOL allow_null_flag = YES;

    Check_Type(name, T_STRING);
    GetCTTable(self, table);

    index = ctdbAddIndex(table->handle, RSTRING_PTR(name), 
                                        FIX2INT(type), 
                                        allow_dups_flag, 
                                        allow_null_flag);
    
    if ( index == NULL )
        rb_raise(cCTError, "[%d] ctdbAddIndex failed.", 
            ctdbGetError(table->handle));

    return rb_ct_index_new(cCTIndex, index);
}

/*
 * Retrieve an Index by name or number.
 *
 * @param [Fixnum, String] value Field identifier
 * @return [CT::Index]
 */
static VALUE
rb_ct_table_get_index(VALUE self, VALUE value)
{
    ct_table *table;
    CTHANDLE index;

    GetCTTable(self, table);
      
    switch ( rb_type(value) ) {
        case T_STRING :
            index = ctdbGetIndexByName(table->handle, RSTRING_PTR(value));
            break;
        case T_FIXNUM :
            index = ctdbGetIndex(table->handle, FIX2INT(value));
            break;
        default :
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(value));
            break;
    }

    if ( ! index )
        rb_raise(cCTError, "[%d] ctdbGetIndex failed.", 
            ctdbGetError(table->handle));

    return rb_ct_index_new(cCTIndex, index);
}

/*
 * Retstatic rieve a collection of all indecies associated with a table.
 * @return [Array]
 */
static VALUE
rb_ct_table_get_indecies(VALUE self)
{
    ct_table *table;
    int j;          // Random ass counter
    VRLEN count;    // Number of indecies associated with the table
    VALUE indecies; // Ruby Array of CT::Index objects
    CTHANDLE index; // Index handle

    GetCTTable(self, table);
    
    indecies = rb_ary_new();
    
    count = ctdbGetTableIndexCount(table->handle);
    for ( j = 0; j < count; j++ ) {
        if ( ( index = ctdbGetIndex(table->handle, j) ) == NULL )
            rb_raise(cCTError, "[%d] ctdbGetIndex failed.", 
                ctdbGetError(table->handle));

        rb_ary_push(indecies, rb_ct_index_new(cCTIndex, index));  
    }

    return indecies;
}

/*
 * Rebuild existing table based on field, index and segment changes.
 *
 * @param [Fixnum] mode The alter table action
 * @raise [CT::Error] ctdbAlterTable failed.
 */
static VALUE
rb_ct_table_alter(VALUE self, VALUE mode)
{
    ct_table *table;

    Check_Type(mode, T_FIXNUM);

    GetCTTable(self, table);

    if ( ctdbAlterTable(table->handle, FIX2INT(mode)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbAlterTable failed.", 
            ctdbGetError(table->handle));

    return self;
}

/*
 * Close the table.
 *
 * @raise [CT::Error] ctdbCloseTable failed.
 */
static VALUE
rb_ct_table_close(VALUE self)
{
    ct_table *table;

    GetCTTable(self, table);

    if ( ctdbCloseTable(table->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCloseTable failed.", 
            ctdbGetError(table->handle));

    return self;
}

/*
 * Create a new table
 *
 * @param [Strng] name The table name
 * @param [Fixnum] mode Table create mode.
 * @raise [CT::Error] ctdbCreateTable failed.
 */
static VALUE
rb_ct_table_create(VALUE self, VALUE name, VALUE mode)
{
    ct_table *table;
    
    Check_Type(name, T_STRING);
    Check_Type(mode, T_FIXNUM);

    GetCTTable(self, table);

    if ( ctdbCreateTable(table->handle, RSTRING_PTR(name), 
                                              CTCREATE_NORMAL) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCreateTable failed.", 
            ctdbGetError(table->handle));

    return self;
}

/*
 * Retrieve the table create mode.
 */
static VALUE
rb_ct_table_get_create_mode(VALUE self)
{
    ct_table *table;

    GetCTTable(self, table);
    
    return INT2FIX(ctdbGetTableCreateMode(table->handle));
}

/*
 * Retrieve the default date type.
 * 
 * @return [Fixnum] The date type.
 * @raise [CT::Error] ctdbGetDefDateType failed.
 */
static VALUE
rb_ct_get_defualt_date_type(VALUE self)
{
    ct_index *index;
    CTDATE_TYPE t;

    GetCTIndex(self, index);

    if ( ( t = ctdbGetDefDateType(index->handle) ) == 0 )
        rb_raise(cCTError, "[%d] ctdbGetDefDateType failed.", 
            ctdbGetError(index->handle));

    return INT2FIX(t);
}

/*
 * Set the default date type.
 *
 * @param [Fixnum] type
 * @raise [CT::Error] ctdbSetDefDateType failed.
 */
static VALUE
rb_ct_set_defualt_date_type(VALUE self, VALUE type)
{
    ct_index *index;

    Check_Type(type, T_FIXNUM);

    GetCTIndex(self, index);

    if ( ctdbSetDefDateType(index->handle, FIX2INT(type)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetDefDateType failed", 
            ctdbGetError(index->handle));

    return self;
}

/*
 * Retrieve the table delimiter character.
 *
 * @return [String] The field delimiter character.
 * @raise [CT::Error] ctdbGetPadChar failed.
 */
static VALUE 
rb_ct_table_get_delim_char(VALUE self)
{
    ct_table *table;
    TEXT dchar;

    GetCTTable(self, table);

    if ( ctdbGetPadChar(table->handle, NULL, &dchar) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetPadChar failed.", 
            ctdbGetError(table->handle));

    return rb_str_new_cstr(&dchar);
}

/*
 * Retrieve a table field based on the field number or name.
 *
 * @param [Fixnum, String] value The field number or name.
 * @return [CT::Field]
 * @raise [CT::Error] ctdbGetField failed.
 */
static VALUE
rb_ct_table_get_field(VALUE self, VALUE value)
{
    ct_table *table;
    CTHANDLE field;

    GetCTTable(self, table);

    switch ( rb_type(value) ) {
        case T_STRING :
            field = ctdbGetFieldByName(table->handle, (pTEXT)RSTRING_PTR(value));
            break;
        case T_FIXNUM :
            field = ctdbGetField(table->handle, FIX2INT(value));
            break;
        default :
            rb_raise(rb_eArgError, "Unexpected value type `%s'",
                rb_obj_classname(value));
            break;
    }

    if ( ! field )
        rb_raise(cCTError, "[%d] ctdbGetField failed.", 
            ctdbGetError(table->handle));

    return rb_ct_field_new(cCTField, field);
}

/*
 * Retrieve all fields in a table.
 *
 * @yield [field] Each field as a CT::Field object.
 * @return [Array] Collection of CT::Field objects.
 * @raise [CT::Error] ctdbGetTableFieldCount or ctdbGetField failed.
 */
static VALUE
rb_ct_table_get_fields(VALUE self)
{
    int i, n;
    ct_table *table;
    CTHANDLE field;
    VALUE fields, rb_field;

    GetCTTable(self, table);

    if ( ( n = ctdbGetTableFieldCount(table->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetTableFieldCount failed.", 
            ctdbGetError(table->handle));

    fields = rb_ary_new2(n);

    for(i = 0; i < n; i++){
        // Get the field handle.
        if ( ( field = ctdbGetField(table->handle, i) ) == NULL )
            rb_raise(cCTError, "[%d] ctdbGetField failed.", 
                ctdbGetError(table->handle));

        rb_field = rb_ct_field_new(cCTField, field);
        
        if(rb_block_given_p()) rb_yield(rb_field);
        
        rb_ary_store(fields, i, rb_field);
    }

    return fields;
}

/*
 * Retrieve a table field based on the field name.
 *
 * @param [String] name The field name.
 * @return [CT::Field]
 * @raise [CT::Error] ctdbGetFieldByName failed.
 */
static VALUE
rb_ct_table_get_field_by_name(VALUE self, VALUE name)
{
    ct_table *table;
    CTHANDLE field;
    
    Check_Type(name, T_STRING);
 
    GetCTTable(self, table);

    if ( ! ( field = ctdbGetFieldByName(table->handle, RSTRING_PTR(name)) ) )
        rb_raise(cCTError, "[%d] ctdbGetFieldByName failed.", 
            ctdbGetError(table->handle));
 
    return rb_ct_field_new(cCTField, field);
}

/*
 * Retrieve the number of fields associated with the table. 
 *
 * @return [Fixnum]
 * @raise [CT::Error] ctdbGetTableFieldCount failed.
 */
static VALUE
rb_ct_table_get_field_count(VALUE self)
{
    int i;
    ct_table *table;

    GetCTTable(self, table);

    if ( ( i = ctdbGetTableFieldCount(table->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetTableFieldCount failed.", 
            ctdbGetError(table->handle));

    return INT2FIX(i);
}

/*
 * Retrieve a collection of all the field names associated with the table.
 *
 * @return [Array]
 * @raise [CT::Error] ctdbGetTableFieldCount, ctdbGetField, or ctdbGetFieldName 
 *        failed.
 */
static VALUE
rb_ct_table_get_field_names(VALUE self)
{
    int i, n;
    ct_table *table;
    CTHANDLE field;
    pTEXT fname;
    VALUE fields;

    GetCTTable(self, table);

    if ( ( n = ctdbGetTableFieldCount(table->handle) ) == -1 )
        rb_raise(cCTError, "[%d] ctdbGetTableFieldCount failed.", 
            ctdbGetError(table->handle));

    fields = rb_ary_new2(n);

    for(i = 0; i < n; i++){
        // Get the field handle.
        if ( ( field = ctdbGetField(table->handle, i) ) == NULL )
            rb_raise(cCTError, "[%d] ctdbGetField failed.", 
                ctdbGetError(table->handle));
        // Use the field handle to retrieve the field name.
        if ( ( fname = ctdbGetFieldName(field) ) == NULL )
            rb_raise(cCTError, "[%d] ctdbGetFieldName failed.", 
                ctdbGetError(table->handle));
        // Store the field name
        rb_ary_store(fields, i, rb_str_new_cstr(fname));
    }
    return fields;
}

static VALUE
rb_ct_table_get_group_id(VALUE self)
{
    ct_table *table;

    GetCTTable(self, table);

    return rb_str_new_cstr((pTEXT)ctdbGetTableGroupid(table->handle));
}

static VALUE
rb_ct_table_set_group_id(VALUE self, VALUE id)
{
    ct_table *table;
    CTDBRET rc;

    Check_Type(id, T_STRING);

    GetCTTable(self, table);

    if ( ( rc = ctdbSetTableGroupid(table->handle, RSTRING_PTR(id)) ) )
        rb_raise(cCTError, "[%d] ctdbSetTableGroupid failed.", rc);

    return self;
}

/*
 * Retrieve the table name.
 *
 * @return [String, nil]
 */
static VALUE
rb_ct_table_get_name(VALUE self)
{
    ct_table *table;
    VALUE name;

    GetCTTable(self, table);

    name = rb_str_new_cstr(ctdbGetTableName(table->handle));
    return RSTRING_LEN(name) == 0 ? Qnil : name;
}

/*
 * Open the table.
 *
 * @param [String] name Name of the table to open
 * @raise [CT::Error] ctdbOpenTable failed.
 */
static VALUE
rb_ct_table_open(VALUE self, VALUE name, VALUE mode)
{
    ct_table *table;

    Check_Type(name, T_STRING);

    GetCTTable(self, table);

    if ( ctdbOpenTable(table->handle, RSTRING_PTR(name), 
                                                CTOPEN_NORMAL) !=  CTDBRET_OK )
        rb_raise(cCTError, "[%d][%d] ctdbOpenTable failed.", 
                ctdbGetError(table->handle), sysiocod);

    return self;
}

/*
 * Retrieve the active state of a table.  A table is active if it is open.
 */
static VALUE
rb_ct_table_is_active(VALUE self)
{
    ct_table *table;
    
    GetCTTable(self, table);

    return ctdbIsActiveTable(table->handle) == YES ? Qtrue : Qfalse;
}

/*
 * Retrieve the table pad character
 *
 * @return [String] The character.
 * @raise [CT::Error] ctdbGetPadChar failed
 */
static VALUE
rb_ct_table_get_pad_char(VALUE self)
{
    ct_table *table;
    TEXT pchar;

    GetCTTable(self, table);

    if ( ctdbGetPadChar(table->handle, &pchar, NULL) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbGetPadChar failed.", 
            ctdbGetError(table->handle));

    return rb_str_new_cstr(&pchar);
}

/* TODO:
 * Set the table pad character.
 *
 * @param [String] pad_char The character to pad fixed length fields with.
 */
static VALUE
rb_ct_table_set_pad_char(VALUE self, VALUE pad_char)
{
    Check_Type(pad_char, T_STRING);

    return self;
}

/*
 * Retrieve the table path.
 *
 * @return [String, nil] The path or nil if one is not set.
 */
static VALUE
rb_ct_table_get_path(VALUE self)
{
    ct_table *table;
    VALUE path;

    GetCTTable(self, table);
    path = rb_str_new_cstr(ctdbGetTablePath(table->handle));
    
    return RSTRING_LEN(path) == 0 ? Qnil : path;
}

/*
 * Set a new table path
 *
 * @param [String] path File path to the table.
 * @raise [CT::Error] ctdbSetTablePath failed.
 */
static VALUE
rb_ct_table_set_path(VALUE self, VALUE path)
{
    ct_table *table;

    Check_Type(path, T_STRING);

    GetCTTable(self, table);

    if ( ctdbSetTablePath(table->handle, RSTRING_PTR(path)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetTablePath failed.", 
            ctdbGetError(table->handle));

    return self;
}

static VALUE
rb_ct_table_get_permission(VALUE self)
{
    return self;
}

/*
 * Set the table permission mask
 * @param [Long] pmask
 *
 * @example
 */

static VALUE
rb_ct_table_set_permission(VALUE self, VALUE pmask)
{
    return self;
}

/*
 * TODO:
 *
 * @param [Fixnum] mode The rebuild mode.
 */
static VALUE
rb_ct_table_rebuild(VALUE self, VALUE mode)
{
    return self;
}

/*
 * Retrieves the table status. The table status indicates which rebuild action 
 * will be taken by an alter table operation.
 *
 * @return [Fixnum]
 */
static VALUE
rb_ct_table_get_status(VALUE self)
{
    ct_table *table;
    GetCTTable(self, table);
    
    return INT2FIX(ctdbGetTableStatus(table->handle));
}

void
init_rb_ct_table(void)
{
    cCTTable = rb_define_class_under(mCT, "Table", rb_cObject);
    
    rb_define_singleton_method(cCTTable, "new", rb_ct_table_new, 1);
    rb_define_method(cCTTable, "initialize", rb_ct_table_init, 1);
    rb_define_method(cCTTable, "add_field", rb_ct_table_add_field, 3);
    rb_define_method(cCTTable, "add_index", rb_ct_table_add_index, 2);
    rb_define_method(cCTTable, "get_index", rb_ct_table_get_index, 1);
    rb_define_method(cCTTable, "indecies", rb_ct_table_get_indecies, 0); 
    rb_define_method(cCTTable, "alter", rb_ct_table_alter, 1);
    rb_define_method(cCTTable, "close", rb_ct_table_close, 0);
    rb_define_method(cCTTable, "create", rb_ct_table_create, 2);
    rb_define_method(cCTTable, "create_mode", rb_ct_table_get_create_mode, 0);
    rb_define_method(cCTSession, "default_date_type", rb_ct_get_defualt_date_type, 0);
    rb_define_method(cCTSession, "default_date_type=", rb_ct_set_defualt_date_type, 1);
    rb_define_method(cCTTable, "delim_char", rb_ct_table_get_delim_char, 0);
    rb_define_method(cCTTable, "field_count", rb_ct_table_get_field_count, 0);
    rb_define_method(cCTTable, "field_names", rb_ct_table_get_field_names, 0);
    rb_define_method(cCTTable, "get_field", rb_ct_table_get_field, 1);
    rb_define_method(cCTTable, "get_fields", rb_ct_table_get_fields, 0);
    rb_define_method(cCTTable, "group_id", rb_ct_table_get_group_id, 0);
    rb_define_method(cCTTable, "group_id=", rb_ct_table_set_group_id, 1);
    rb_define_alias(cCTTable, "fields", "get_fields");
    rb_define_method(cCTTable, "get_field_by_name", rb_ct_table_get_field_by_name, 1);
    rb_define_method(cCTTable, "name", rb_ct_table_get_name, 0);
    rb_define_method(cCTTable, "open", rb_ct_table_open, 2);
    rb_define_method(cCTTable, "active?", rb_ct_table_is_active, 0);
    rb_define_alias(cCTTable, "open?", "active?");
    rb_define_method(cCTTable, "pad_char", rb_ct_table_get_pad_char, 0);
    rb_define_method(cCTTable, "pad_char=", rb_ct_table_set_pad_char, 1);
    rb_define_method(cCTTable, "path", rb_ct_table_get_path, 0);
    rb_define_method(cCTTable, "path=", rb_ct_table_set_path, 1);
    rb_define_method(cCTTable, "permission", rb_ct_table_get_permission, 0);
    rb_define_method(cCTTable, "permission=", rb_ct_table_set_permission, 1);
    rb_define_method(cCTTable, "rebuild", rb_ct_table_rebuild, 1);
    rb_define_method(cCTTable, "status", rb_ct_table_get_status, 0);
}
