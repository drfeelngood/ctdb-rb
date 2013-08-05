#include <ctdb_ext.h>
#include <ct_date.h>

VALUE cCTDate;

extern VALUE mCT;
extern VALUE cCTError;

void
free_rb_ct_date(void *ptr)
{
    ct_date *date = (ct_date *)ptr;
    xfree(date);
}

VALUE
ct_date_init_with(pCTDATE dt)
{
    ct_date *date;
    VALUE obj;

    obj = Data_Make_Struct(cCTDate, ct_date, 0, free_rb_ct_date, date); 
    date->value = (CTDATE)*dt;
    date->type  = CTDATE_MDCY;

    return obj;
}

VALUE
ct_date_init_with2(pCTDATE dt, CTDATE_TYPE type)
{
  ct_date *date;
  VALUE obj;

  obj = Data_Make_Struct(cCTDate, ct_date, 0, free_rb_ct_date, date); 
  date->value = (CTDATE)*dt;
  date->type  = type; 

  return obj;
}

/*
 * Create a new instance of CT::Date 
 *
 * @param [Fixnum] year
 * @param [Fixnum] month
 * @param [Fixnum] day
 * @return [CT::Date]
 */
VALUE
rb_ct_date_new(VALUE klass, VALUE year, VALUE month, VALUE day)
{
    CTDATE date;
    CTDBRET rc;
    NINT y, m, d;

    Check_Type(year,  T_FIXNUM);
    Check_Type(month, T_FIXNUM);
    Check_Type(day,   T_FIXNUM);

    y = FIX2INT(year);
    m = FIX2INT(month);
    d = FIX2INT(day);

    if ( ( rc = ctdbDatePack(&date, y, m, d) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDatePack failed.", rc);
    
    return ct_date_init_with(&date);
}

/*
 * Get the current date yo.
 * @return [CT::Date]
 */
static VALUE
rb_ct_date_get_current_date(VALUE klass)
{
    CTDATE dt;
    CTDBRET rc;
    NINT y, m, d;

    if ( ( rc = ctdbCurrentDate(&dt) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCurrentDate failed.", rc);

    if ( ( rc = ctdbDateUnpack(dt, &y, &m, &d) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateUnpack failed.", rc);

    return rb_ct_date_new(cCTDate, INT2FIX(y), INT2FIX(m), INT2FIX(d));
}

/*
 *
 */
/*
 *static VALUE
 *rb_ct_date_strptime(VALUE klass, VALUE string, VALUE format)
 *{
 *}
 */

/*
 * Convert date to a String
 *
 * @return [String]
 */
static VALUE
rb_ct_date_to_string(VALUE self)
{
    ct_date *date;
    char *format;
    CTDBRET rc;
    VRLEN size = 0;
    TEXT str;

    GetCTDate(self, date);

    switch ( date->type ) {
        case CTDATE_MDCY :
            format = (char *)"%m/%d/%Y";
            break;
        case CTDATE_DMCY : 
            format = (char *)"%m/%d/%y";
            break;
        case CTDATE_CYMD :
            format = (char *)"%d/%m/%Y";
            break;
        case CTDATE_MDY :
            format = (char *)"%d/%m/%y";
            break;
        case CTDATE_DMY :
            format = (char *)"%Y%m%d";
            break;
        case CTDATE_YMD :
            format = (char *)"%y%m%d";
            break;
        default :
            rb_raise(cCTError, "Unexpected default date format");
            break;
    }

    if ( date->value > 0 ) {
        size = (VRLEN)(strlen(format) + 3);
        if ( (rc = ctdbDateToString(date->value, date->type, &str, size) ) != CTDBRET_OK )
            rb_raise(cCTError, "[%d] ctdbDateToString failed.", rc);
        
        return rb_str_new_cstr(&str);
    } else
        return rb_str_new_cstr("");

}

/*
 * Retrieve the day component
 *
 * @return [Fixnum]
 */
static VALUE
rb_ct_date_get_day(VALUE self)
{
    ct_date *date;
    NINT day;

    GetCTDate(self, date);

    if ( ( day = ctdbGetDay(date->value) ) == 0 )
        rb_raise(cCTError, "ctdbGetDay failed.");

    return INT2FIX(day);
}

/*
 * Retrieve the month component
 *
 * @return [Fixnum]
 */
static VALUE
rb_ct_date_get_month(VALUE self)
{
    ct_date *date;
    NINT month;
    
    GetCTDate(self, date);

    if ( ( month = ctdbGetMonth(date->value) ) == 0 )
        rb_raise(cCTError, "ctdbGetMonth failed.");

    return INT2FIX(month);
}

/*
 * Retrieve the year component
 *
 * @return [Fixnum]
 */
static VALUE   
rb_ct_date_get_year(VALUE self)
{
    ct_date *date;
    NINT year;

    GetCTDate(self, date);

    if ( ( year = ctdbGetYear(date->value) ) == 0 )
        rb_raise(cCTError, "ctdbGetYear failed.");

    return INT2FIX(year);
}

/*
 *rb_ct_is_leap_year(VALUE self)
 *{
 *}
 */

/*
 * Convert CT::Date to ruby Date
 */
static VALUE
rb_ct_date_to_date(VALUE self)
{
    ct_date *date;
    NINT y, m, d;
    CTDBRET rc;
    VALUE rb_date;

    GetCTDate(self, date);

    if ( ( rc = ctdbDateUnpack(date->value, &y, &m, &d) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateUnpack failed.", rc);

    rb_date = rb_funcall(RUBY_CLASS("Date"), rb_intern("new"), 3, 
        INT2FIX(y), INT2FIX(m), INT2FIX(d));

    return rb_date;
}

/*
 *
 */
static VALUE
rb_ct_date_to_i(VALUE self)
{
    ct_date *date;

    GetCTDate(self, date);

    return INT2FIX(date->value);
}

static VALUE
rb_ct_date_get_type(VALUE self)
{
    ct_date *date;

    GetCTDate(self, date);

    return INT2NUM(date->type);
}

void
init_rb_ct_date()
{
    cCTDate = rb_define_class_under(mCT, "Date", rb_cObject);
    
    rb_define_singleton_method(cCTDate, "new", rb_ct_date_new, 3);
    rb_define_singleton_method(cCTDate, "today", rb_ct_date_get_current_date, 0);
    rb_define_method(cCTDate, "to_s", rb_ct_date_to_string, 0);
    rb_define_method(cCTDate, "day",  rb_ct_date_get_day, 0);
    rb_define_method(cCTDate, "mon", rb_ct_date_get_month, 0);
    rb_define_method(cCTDate, "year", rb_ct_date_get_year, 0);
    rb_define_method(cCTDate, "to_date", rb_ct_date_to_date, 0);
    rb_define_method(cCTDate, "to_i", rb_ct_date_to_i, 0);
    rb_define_method(cCTDate, "type", rb_ct_date_get_type, 0);
    /*rb_define_method(cCTDate, "leap_year?", rb_ct_date_is_leap_year, 0);*/
    /*rb_define_method(cCTDate, "day_of_week", rb_ct_date_get_day_of_week, 0);*/
}
