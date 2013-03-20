#ifndef RB_CT_FIELD_H
#define RB_CT_FIELD_H

void init_rb_ct_field();
VALUE rb_ct_field_new(VALUE klass, CTHANDLE field_handle);

typedef struct {
    CTHANDLE handle;
} ct_field;

#define GetCTField(obj, val) ( val = (ct_field*)DATA_PTR(obj) );

#endif
