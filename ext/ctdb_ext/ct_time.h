#ifndef RB_CT_TIME_H
#define RB_CT_TIME_H

void init_rb_ct_time();
void free_rb_ct_time(void *ptr);
VALUE ct_time_init_with(pCTTIME tm);

typedef struct {
    CTTIME value;
    CTTIME_TYPE type;
} ct_time;

#define GetCTTime(obj, val) ( val = (ct_time*)DATA_PTR(obj) );

VALUE ct_time_init_with(pCTTIME tm);

#endif
