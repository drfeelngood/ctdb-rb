#ifndef RB_CT_RECORD_H
#define RB_CT_RECORD_H

void init_rb_ct_record();

typedef struct {
    CTHANDLE handle;
    pCTHANDLE table_ptr;
} ct_record;

#define GetCTRecord(obj, val) ( val = (ct_record*)DATA_PTR(obj) );

#endif
