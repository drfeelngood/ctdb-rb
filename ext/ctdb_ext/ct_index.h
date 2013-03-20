#ifndef RB_CT_INDEX_H
#define RB_CT_INDEX_H

void init_rb_ct_index();
VALUE rb_ct_index_new(VALUE klass, CTHANDLE index_handle);

typedef struct {
    CTHANDLE handle;
} ct_index;

#define GetCTIndex(obj, val) ( val = (ct_index*)DATA_PTR(obj) );

#endif
