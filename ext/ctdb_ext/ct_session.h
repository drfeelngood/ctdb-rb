#ifndef RB_CT_SESSION_H
#define RB_CT_SESSION_H

void init_rb_ct_session();

typedef struct {
    CTHANDLE handle;
} ct_session;

#define GetCTSession(obj, val) ( val = (ct_session*)DATA_PTR(obj) );

#endif
