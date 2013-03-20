#ifndef RB_CT_SEGMENT_H
#define RB_CT_SEGMENT_H

void init_rb_ct_segment();
VALUE rb_ct_segment_new(VALUE klass, CTHANDLE segment_handle);

typedef struct {
    CTHANDLE handle;
} ct_segment;

#define GetCTSegment(obj, val) ( val = (ct_segment*)DATA_PTR(obj) );

#endif
