#ifndef RB_CT_TIME_H
#define RB_CT_TIME_H

typedef struct {
    CTTIME value;
    CTTIME_TYPE type;
} ct_time;

#define GetCTTime(obj, val) ( val = (ct_time*)DATA_PTR(obj) );

void init_rb_ct_time();
void free_rb_ct_time(void *ptr);
VALUE ct_time_init_with(pCTTIME tm);
VALUE ct_time_init_with2(pCTTIME tm, CTTIME_TYPE type);

#endif
