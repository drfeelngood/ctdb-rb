#include <ctdb_ext.h>
#include <ct_time.h>

VALUE cCTTime;

extern VALUE mCT;
extern VALUE cCTError;

void
free_rb_ct_time(void *ptr)
{
    ct_time *time = (ct_time *)ptr;
    xfree(time);
}

VALUE
ct_time_init_with(pCTTIME tm)
{
    ct_time *time;
    VALUE obj;

    obj = Data_Make_Struct(cCTTime, ct_time, 0, free_rb_ct_time, time);
    time->value = (CTTIME)*tm;
    time->type = CTTIME_HHMS;
    
    return obj;
}

VALUE 
ct_time_init_with2(pCTTIME tm, CTTIME_TYPE type)
{
    ct_time *time;
    VALUE obj;

    obj = Data_Make_Struct(cCTTime, ct_time, 0, free_rb_ct_time, time);
    time->value = (CTTIME)*tm;
    time->type  = type;
    
    return obj;
}

/*
 * Create a new instance of CT::Time 
 *
 * @param [Fixnum] year
 * @param [Fixnum] month
 * @param [Fixnum] day
 * @param [Fixnum] hour
 * @param [Fixnum] min
 * @param [Fixnum] sec
 * @return [CT::Time]
 */
static VALUE
rb_ct_time_new(VALUE klass, VALUE hour, VALUE min, VALUE sec)
{
    CTTIME tm;
    NINT h, m, s;

    Check_Type(hour, T_FIXNUM);
    Check_Type(min,  T_FIXNUM);
    Check_Type(sec,  T_FIXNUM);

    h = FIX2INT(hour);
    m = FIX2INT(min);
    s = FIX2INT(sec);

    ctdbTimePack(&tm, h, m, s);
    
    return ct_time_init_with(&tm);
}

/*
 * Retrieve the current time
 * @return [CT::Time]
 */
static VALUE
rb_ct_time_now(VALUE klass)
{
    CTTIME tm;
    CTDBRET rc;
 
    if ( ( rc = ctdbCurrentTime(&tm) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCurrentTime failed.", rc);

    return ct_time_init_with(&tm);
}

/*
 *
 * @return [String]
 */
static VALUE
rb_ct_time_to_string(VALUE self)
{
    ct_time *time;
    char * format;
    CTDBRET rc;
    VRLEN size = 0;
    TEXT str;

    GetCTTime(self, time);
    
    switch ( time->type ) {
      case CTTIME_HMSP :
          format = (char *)"%I:%M:%S %P";
          break;
      case CTTIME_HMP :
          format = (char *)"%I:%M %P";
          break;
      case CTTIME_HMS :
          format = (char *)"%H:%M:%S";
          break;
      case CTTIME_HM :
          format = (char *)"%H:%M";
          break;
      case CTTIME_MIL :
          format = (char *)"%H%M";
          break;
      case CTTIME_HHMSP :
          format = (char *)"%H:%M:%S %P";
          break;
      case CTTIME_HHMP :
          format = (char *)"%H:%M %P";
          break;
      case CTTIME_HHMS :
          format = (char *)"%H:%M:%S";
          break;
      case CTTIME_HHM :
          format = (char *)"%H:%M";
          break;
      default :
          rb_raise(cCTError, "Unknown CT::Time format.");
          break;
    }


    if ( time->value > 0 ) {
        size = (VRLEN)(strlen(format) + 3);
        if( (rc = ctdbTimeToString(time->value, time->type, &str, size) ) != CTDBRET_OK )
          rb_raise(cCTError, "[%d] ctdbTimeToString failed.", rc);
    }

    return rb_str_new_cstr(&str);
}

static VALUE
rb_ct_time_to_time(VALUE self)
{
    VALUE parts;

    parts = RSEND(self, "unpack");

    return rb_funcall( RUBY_CLASS("Time"), rb_intern("new"), 6,
                       INT2FIX(1970),
                       INT2FIX(1),
                       INT2FIX(1),
                       rb_ary_entry(parts, 0),
                       rb_ary_entry(parts, 1),
                       rb_ary_entry(parts, 2) );
}

/*
 * @return [Array]
 */
static VALUE
rb_ct_time_unpack(VALUE self)
{
    ct_time *time;
    NINT h, m, s;
    CTDBRET rc;
    VALUE vals;

    GetCTTime(self, time);

    if ( ( rc = ctdbTimeUnpack(time->value, &h, &m, &s) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbTimeUnpack failed.", rc);

    vals = rb_ary_new2(3);
    rb_ary_store(vals, 0, INT2FIX(h));
    rb_ary_store(vals, 1, INT2FIX(m));
    rb_ary_store(vals, 2, INT2FIX(s));

    return vals;
}

static VALUE
rb_ct_time_get_hour(VALUE self)
{
    ct_time *time;
    NINT h, m, s;
    CTDBRET rc;

    GetCTTime(self, time);

    if ( ( rc = ctdbTimeUnpack(time->value, &h, &m, &s) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbTimeUnpack failed.", rc);

    return INT2FIX(h);
    
}

static VALUE
rb_ct_time_get_min(VALUE self)
{
    ct_time *time;
    NINT h, m, s;
    CTDBRET rc;

    GetCTTime(self, time);

    if ( ( rc = ctdbTimeUnpack(time->value, &h, &m, &s) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbTimeUnpack failed.", rc);

    return INT2FIX(m);
}

static VALUE
rb_ct_time_get_sec(VALUE self)
{
    ct_time *time;
    NINT h, m, s;
    CTDBRET rc;

    GetCTTime(self, time);

    if ( ( rc = ctdbTimeUnpack(time->value, &h, &m, &s) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbTimeUnpack failed.", rc);

    return INT2FIX(s);
}

static VALUE
rb_ct_time_get_type(VALUE self)
{
    ct_time *time;

    GetCTTime(self, time);

    return INT2NUM(time->type);
}

void
init_rb_ct_time()
{
    cCTTime = rb_define_class_under(mCT, "Time", rb_cObject);
    
    rb_define_singleton_method(cCTTime, "new", rb_ct_time_new, 3);
    rb_define_singleton_method(cCTTime, "now", rb_ct_time_now, 0);
    
    rb_define_method(cCTTime, "to_s", rb_ct_time_to_string, 0);
    rb_define_method(cCTTime, "to_time", rb_ct_time_to_time, 0);
    rb_define_method(cCTTime, "unpack", rb_ct_time_unpack, 0);
    rb_define_method(cCTTime, "hour", rb_ct_time_get_hour, 0);
    rb_define_method(cCTTime, "min", rb_ct_time_get_min, 0);
    rb_define_method(cCTTime, "sec", rb_ct_time_get_sec, 0);
    rb_define_method(cCTTime, "type", rb_ct_time_get_type, 0);
}
