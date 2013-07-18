#ifndef RB_CT_DATETIME_H
#define RB_CT_DATETIME_H

void init_rb_ct_datetime();
void free_rb_ct_datetime();

typedef struct {
    CTDATETIME value;
} ct_date_time;

#define GetCTDateTime(obj, val) ( val = (ct_date_time*)DATA_PTR(obj) );

VALUE ct_date_time_init_with(pCTDATETIME dttm);

#endif
