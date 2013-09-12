#ifndef RB_CT_DATETIME_H
#define RB_CT_DATETIME_H

typedef struct {
    CTDATETIME value;
    CTDATE_TYPE date_type;
    CTTIME_TYPE time_type;
} ct_date_time;

#define GetCTDateTime(obj, val) ( val = (ct_date_time*)DATA_PTR(obj) );

void init_rb_ct_date_time();
void free_rb_ct_date_time();
VALUE ct_date_time_init_with(pCTDATETIME dttm);
VALUE ct_date_time_init_with2(pCTDATETIME dttm, CTDATE_TYPE date_type,
                                                CTTIME_TYPE time_type);
#endif
