#ifndef RB_CT_DATE_H
#define RB_CT_DATE_H

void init_rb_ct_date();
void free_rb_ct_date(void *ptr);

typedef struct {
    CTDATE value;
    CTDATE_TYPE type; 
} ct_date;

#define GetCTDate(obj, val) ( val = (ct_date*)DATA_PTR(obj) );

#endif
