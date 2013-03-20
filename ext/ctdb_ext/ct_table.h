#ifndef RB_CT_TABLE_H
#define RB_CT_TABLE_H

void init_rb_ct_table();

typedef struct {
    CTHANDLE handle;  
} ct_table;

#define GetCTTable(obj, val) ( val = (ct_table*)DATA_PTR(obj));

#endif
