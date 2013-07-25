#ifndef RB_CT_DATE_H
#define RB_CT_DATE_H

typedef struct {
    CTDATE value;
    CTDATE_TYPE type; 
} ct_date;

#define GetCTDate(obj, val) ( val = (ct_date*)DATA_PTR(obj) );

void init_rb_ct_date();
void free_rb_ct_date(void *ptr);
VALUE ct_date_init_with(pCTDATE dt);
VALUE ct_date_init_with2(pCTDATE dt, CTDATE_TYPE type);

#endif
