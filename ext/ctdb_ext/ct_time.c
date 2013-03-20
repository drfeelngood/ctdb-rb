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
rb_ct_time_new(VALUE klass, VALUE year, VALUE month, VALUE day, VALUE hour, 
        VALUE min, VALUE sec)
{
    ct_time *time;
    CTTIME tm;
    NINT y, m, d, h, i, s;
    VALUE obj;

    Check_Type(year,  T_FIXNUM);
    Check_Type(month, T_FIXNUM);
    Check_Type(day,   T_FIXNUM);
    Check_Type(hour,  T_FIXNUM);
    Check_Type(min,   T_FIXNUM);
    Check_Type(sec,   T_FIXNUM);

    y = FIX2INT(year);
    m = FIX2INT(month);
    d = FIX2INT(day);
    h = FIX2INT(hour);
    i = FIX2INT(min);
    s = FIX2INT(sec);

    ctdbTimePack(&tm, h, i, s);
    
    obj = Data_Make_Struct(klass, ct_time, 0, free_rb_ct_time, time); 
    time->value = tm;
    
    return obj;
}

/*
 * Retrieve the current time
 * @return [CT::Time]
 */
static VALUE
rb_ct_time_now(VALUE klass)
{
    CTDATETIME datetime;
    CTDBRET rc;
    NINT year, month, day, hour, minute, second;
    VALUE rb_time;

    if ( ( rc = ctdbCurrentDateTime(&datetime) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbCurrentDateTime failed.", rc);

    if ( ( rc = ctdbDateTimeUnpack(datetime, &year, &month, &day, &hour, &minute, 
            &second) ) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbDateTimeUnpack failed.", rc);
    
    rb_time = rb_funcall(RUBY_CLASS("Time"), rb_intern("new"), 6, 
        INT2FIX(year), INT2FIX(month), INT2FIX(day), INT2FIX(hour), 
        INT2FIX(minute), INT2FIX(second));

    return rb_time;
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

void
init_rb_ct_time()
{
    cCTTime = rb_define_class_under(mCT, "Time", rb_cObject);
    rb_define_singleton_method(cCTTime, "new", rb_ct_time_new, 6);
    rb_define_singleton_method(cCTTime, "now", rb_ct_time_now, 0);
    rb_define_method(cCTTime, "to_s", rb_ct_time_to_string, 0);
}
