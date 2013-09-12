#include <ctdb_ext.h>
#include <ct_date_time.h>

VALUE cCTDateTime;

extern VALUE mCT;
extern VALUE cCTError;

void 
free_rb_ct_date_time(void *ptr)
{
    ct_date_time *dt = (ct_date_time *)ptr;
    xfree(dt);
}

VALUE
ct_date_time_init_with(pCTDATETIME dttm)
{
    ct_date_time *datetime;
    VALUE obj;

    obj = Data_Make_Struct(cCTDateTime, ct_date_time, 0, free_rb_ct_date_time,
                           datetime);
    datetime->value = (CTDATETIME)*dttm;
    datetime->date_type = CTDATE_MDCY;
    datetime->time_type = CTTIME_HHMS;

    return obj;
}

VALUE
ct_date_time_init_with2(pCTDATETIME dttm, CTDATE_TYPE date_type, 
                        CTTIME_TYPE time_type)
{
    ct_date_time *datetime;
    VALUE obj;

    obj = Data_Make_Struct(cCTDateTime, ct_date_time, 0, free_rb_ct_date_time,
                           datetime);
    datetime->value = (CTDATETIME)*dttm;
    datetime->date_type = date_type;
    datetime->time_type = time_type;
    
    return obj;
}

VALUE 
rb_ct_date_time_new(VALUE klass, VALUE year, VALUE month, VALUE day, 
                    VALUE hour, VALUE minute, VALUE second)
{
    CTDATETIME dttm;
    CTDBRET rc;
    NINT y, m, d, h, i, s;

    y = FIX2INT(year);
    m = FIX2INT(month);
    d = FIX2INT(day);
    h = FIX2INT(hour);
    i = FIX2INT(minute);
    s = FIX2INT(second);
    
    if ( ( rc = ctdbDateTimePack(&dttm, y, m, d, h, i, s) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateTimePack failed.", rc);

    return ct_date_time_init_with(&dttm);
}

static VALUE
rb_ct_date_time_get_current_date_time(VALUE klass)
{
   CTDATETIME datetime;
   CTDBRET rc;

    if ( ( rc = ctdbCurrentDateTime(&datetime) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCurrentDateTime failed.\n", rc);

    
    return ct_date_time_init_with(&datetime);
}

/*
 * @return [DateTime] Ruby core lib DateTime
 */
static VALUE
rb_ct_date_time_to_datetime(VALUE self)
{
    VALUE parts;
    VALUE offset = INT2NUM(0);

    parts = RSEND(self, "unpack");

    return rb_funcall( RUBY_CLASS("DateTime"), rb_intern("new"), 7,
                       rb_ary_entry(parts, 0),
                       rb_ary_entry(parts, 1),
                       rb_ary_entry(parts, 2),
                       rb_ary_entry(parts, 3),
                       rb_ary_entry(parts, 4),
                       rb_ary_entry(parts, 5),
                       offset );
}

/*
 * Retrieve an Array containing all the components of the DateTime
 * @return [Array] The year, month, day, hour, minute, second
 */
static VALUE
rb_ct_date_time_unpack(VALUE self)
{
    ct_date_time *datetime;
    NINT y, m, d, h, i, s;
    CTDBRET rc;
    VALUE vals;

    GetCTDateTime(self, datetime);

    rc = ctdbDateTimeUnpack(datetime->value, &y, &m, &d, &h, &i, &s); 
    if ( rc != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateTimeUnpack failed.\n", rc);

    vals = rb_ary_new();
    rb_ary_push(vals, INT2FIX(y));
    rb_ary_push(vals, INT2FIX(m));
    rb_ary_push(vals, INT2FIX(d));
    rb_ary_push(vals, INT2FIX(h));
    rb_ary_push(vals, INT2FIX(i));
    rb_ary_push(vals, INT2FIX(s));

    return vals;
}

static VALUE
rb_ct_date_time_get_time(VALUE self)
{
    ct_date_time *datetime;
    CTTIME value;
    CTDBRET rc;

    GetCTDateTime(self, datetime);

    if ( ( rc = ctdbDateTimeGetTime(datetime->value, &value) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateTimeGetTime failed.", rc);

    return ct_time_init_with(&value);
}

static VALUE
rb_ct_date_time_get_date(VALUE self)
{
    ct_date_time *datetime;
    CTDATE value;
    CTDBRET rc;

    GetCTDateTime(self, datetime);

    if ( ( rc = ctdbDateTimeGetDate(datetime->value, &value) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateTimeGetDate failed.", rc);

    return ct_date_init_with(&value);
}

void
init_rb_ct_date_time()
{
    cCTDateTime = rb_define_class_under(mCT, "DateTime", rb_cObject);

    rb_define_singleton_method(cCTDateTime, "new", rb_ct_date_time_new, 6);
    rb_define_singleton_method(cCTDateTime, "now", rb_ct_date_time_get_current_date_time, 0);
    
    rb_define_method(cCTDateTime, "to_datetime", rb_ct_date_time_to_datetime, 0);
    rb_define_method(cCTDateTime, "unpack", rb_ct_date_time_unpack, 0);
    rb_define_method(cCTDateTime, "date", rb_ct_date_time_get_date, 0);
    rb_define_method(cCTDateTime, "time", rb_ct_date_time_get_time, 0);
}
