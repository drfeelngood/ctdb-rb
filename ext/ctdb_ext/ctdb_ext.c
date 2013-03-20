#include <ctdb_ext.h>

VALUE mCT;
VALUE cCTError;

void 
Init_ctdb_ext(void) 
{
    mCT      = rb_define_module("CT");
    cCTError = rb_define_class_under(mCT, "Error", rb_eStandardError);
    
    // c-treeDB Session Types
    rb_define_const(mCT, "SESSION_CTDB",  INT2FIX(CTSESSION_CTDB));
    rb_define_const(mCT, "SESSION_CTREE", INT2FIX(CTSESSION_CTREE));
    // c-treeDB Find Modes
    rb_define_const(mCT, "FIND_EQ", INT2FIX(CTFIND_EQ));
    rb_define_const(mCT, "FIND_LT", INT2FIX(CTFIND_LT));
    rb_define_const(mCT, "FIND_LE", INT2FIX(CTFIND_LE));
    rb_define_const(mCT, "FIND_GT", INT2FIX(CTFIND_GT));
    rb_define_const(mCT, "FIND_GE", INT2FIX(CTFIND_GE));
    // c-treeDB Lock Modes
    rb_define_const(mCT, "LOCK_FREE",       INT2FIX(CTLOCK_FREE));
    rb_define_const(mCT, "LOCK_READ",       INT2FIX(CTLOCK_READ));
    rb_define_const(mCT, "LOCK_READ_BLOCK", INT2FIX(CTLOCK_READ_BLOCK));
    rb_define_const(mCT, "LOCK_WRITE",      INT2FIX(CTLOCK_WRITE));
    rb_define_const(mCT, "LOCK_WRITE_LOCK", INT2FIX(CTLOCK_WRITE_BLOCK));
    // c-treeDB Table create Modes
    rb_define_const(mCT, "CREATE_NORMAL",    INT2FIX(CTCREATE_NORMAL));
    rb_define_const(mCT, "CREATE_PREIMG",    INT2FIX(CTCREATE_PREIMG));
    rb_define_const(mCT, "CREATE_TRNLOG",    INT2FIX(CTCREATE_TRNLOG));
    rb_define_const(mCT, "CREATE_WRITETHRU", INT2FIX(CTCREATE_WRITETHRU));
    rb_define_const(mCT, "CREATE_CHECKLOCK", INT2FIX(CTCREATE_CHECKLOCK));
    rb_define_const(mCT, "CREATE_NORECBYT",  INT2FIX(CTCREATE_NORECBYT));
    rb_define_const(mCT, "CREATE_NOROWID",   INT2FIX(CTCREATE_NOROWID));
    rb_define_const(mCT, "CREATE_CHECKREAD", INT2FIX(CTCREATE_CHECKREAD));
    rb_define_const(mCT, "CREATE_HUGEFILE",  INT2FIX(CTCREATE_HUGEFILE));
    rb_define_const(mCT, "CREATE_NODELFLD",  INT2FIX(CTCREATE_NODELFLD));
    rb_define_const(mCT, "CREATE_NONULFLD",  INT2FIX(CTCREATE_NONULFLD));
    // c-treeDB Table open modes
    rb_define_const(mCT, "OPEN_NORMAL",    INT2FIX(CTOPEN_NORMAL));
    rb_define_const(mCT, "OPEN_DATAONLY",  INT2FIX(CTOPEN_DATAONLY));
    rb_define_const(mCT, "OPEN_EXCLUSIVE", INT2FIX(CTOPEN_EXCLUSIVE));
    rb_define_const(mCT, "OPEN_PERMANENT", INT2FIX(CTOPEN_PERMANENT));
    rb_define_const(mCT, "OPEN_CORRUPT",   INT2FIX(CTOPEN_CORRUPT));
    rb_define_const(mCT, "OPEN_CHECKLOCK", INT2FIX(CTOPEN_CHECKLOCK));
    rb_define_const(mCT, "OPEN_CHECKREAD", INT2FIX(CTOPEN_CHECKREAD));
    rb_define_const(mCT, "OPEN_READONLY",  INT2FIX(CTOPEN_READONLY));
    // c-treeDB Table Permissions
    rb_define_const(mCT, "OPF_READ",   INT2FIX(OPF_READ));
    rb_define_const(mCT, "OPF_WRITE",  INT2FIX(OPF_WRITE));
    rb_define_const(mCT, "OPF_DEF",    INT2FIX(OPF_DEF));
    rb_define_const(mCT, "OPF_DELETE", INT2FIX(OPF_DELETE));
    rb_define_const(mCT, "OPF_ALL",    INT2FIX(OPF_ALL));
    rb_define_const(mCT, "OPF_NOPASS", INT2FIX(OPF_NOPASS));
    rb_define_const(mCT, "GPF_NONE",   INT2FIX(GPF_NONE));
    rb_define_const(mCT, "GPF_READ",   INT2FIX(GPF_READ));
    rb_define_const(mCT, "GPF_WRITE",  INT2FIX(GPF_WRITE));
    rb_define_const(mCT, "GPF_DEF",    INT2FIX(GPF_DEF));
    rb_define_const(mCT, "GPF_DELETE", INT2FIX(GPF_DELETE));
    rb_define_const(mCT, "GPF_NOPASS", INT2FIX(GPF_NOPASS));
    rb_define_const(mCT, "WPF_NONE",   INT2FIX(WPF_NONE));
    rb_define_const(mCT, "WPF_READ",   INT2FIX(WPF_READ));
    rb_define_const(mCT, "WPF_WRITE",  INT2FIX(WPF_WRITE));
    rb_define_const(mCT, "WPF_DEF",    INT2FIX(WPF_DEF));
    rb_define_const(mCT, "WPF_DELETE", INT2FIX(WPF_DELETE));
    rb_define_const(mCT, "WPF_NOPASS", INT2FIX(WPF_NOPASS));
    // c-treeDB Index Types
    rb_define_const(mCT, "INDEX_FIXED",   INT2FIX(CTINDEX_FIXED));
    rb_define_const(mCT, "INDEX_LEADING", INT2FIX(CTINDEX_LEADING));
    rb_define_const(mCT, "INDEX_PADDING", INT2FIX(CTINDEX_PADDING));
    rb_define_const(mCT, "INDEX_LEADPAD", INT2FIX(CTINDEX_LEADPAD));
    rb_define_const(mCT, "INDEX_ERROR",   INT2FIX(CTINDEX_ERROR));
    // c-treeDB Segment modes
    rb_define_const(mCT, "SEG_SCHSEG",     INT2FIX(CTSEG_SCHSEG));
    rb_define_const(mCT, "SET_USCHSEG",    INT2FIX(CTSEG_USCHSEG));
    rb_define_const(mCT, "SEG_VSCHSEG",    INT2FIX(CTSEG_VSCHSEG));
    rb_define_const(mCT, "SEG_UVSCHSEG",   INT2FIX(CTSEG_UVSCHSEG));
    rb_define_const(mCT, "SEG_SCHSRL",     INT2FIX(CTSEG_SCHSRL));
    rb_define_const(mCT, "SEG_ALTSEG",     INT2FIX(CTSEG_ALTSEG));
    rb_define_const(mCT, "SEG_ENDSEG",     INT2FIX(CTSEG_ENDSEG));
    rb_define_const(mCT, "SEG_REGSEG",     INT2FIX(CTSEG_REGSEG));
    rb_define_const(mCT, "SEG_INTSEG",     INT2FIX(CTSEG_INTSEG));
    rb_define_const(mCT, "SEG_UREGSEG",    INT2FIX(CTSEG_UREGSEG));
    rb_define_const(mCT, "SEG_SRLSEG",     INT2FIX(CTSEG_SRLSEG));
    rb_define_const(mCT, "SEG_VARSEG",     INT2FIX(CTSEG_VARSEG));
    rb_define_const(mCT, "SEG_UVARSEG",    INT2FIX(CTSEG_UVARSEG));
    rb_define_const(mCT, "SEG_SGNSEG",     INT2FIX(CTSEG_SGNSEG));
    rb_define_const(mCT, "SEG_FLTSEG",     INT2FIX(CTSEG_FLTSEG));
    rb_define_const(mCT, "SEG_DECSEG",     INT2FIX(CTSEG_DECSEG));
    rb_define_const(mCT, "SEG_BCDSEG",     INT2FIX(CTSEG_BCDSEG));
    rb_define_const(mCT, "SEG_DESCENDING", INT2FIX(CTSEG_DESCENDING));

    // c-treeDB Field Types
    rb_define_const(mCT, "BOOL",      INT2FIX(CT_BOOL));
    rb_define_const(mCT, "TINYINT",   INT2FIX(CT_TINYINT));
    rb_define_const(mCT, "UTINYINT",  INT2FIX(CT_UTINYINT));
    rb_define_const(mCT, "SMALLINT",  INT2FIX(CT_SMALLINT));
    rb_define_const(mCT, "USMALLINT", INT2FIX(CT_USMALLINT));
    rb_define_const(mCT, "INTEGER",   INT2FIX(CT_INTEGER));
    rb_define_const(mCT, "UINTEGER",  INT2FIX(CT_UINTEGER));
    rb_define_const(mCT, "MONEY",     INT2FIX(CT_MONEY));
    rb_define_const(mCT, "DATE",      INT2FIX(CT_DATE));
    rb_define_const(mCT, "TIME",      INT2FIX(CT_TIME));
    rb_define_const(mCT, "FLOAT",     INT2FIX(CT_FLOAT));
    rb_define_const(mCT, "DOUBLE",    INT2FIX(CT_DOUBLE));
    rb_define_const(mCT, "TIMESTAMP", INT2FIX(CT_TIMESTAMP));
    rb_define_const(mCT, "EFLOAT",    INT2FIX(CT_EFLOAT));
    rb_define_const(mCT, "BINARY",    INT2FIX(CT_BINARY));
    rb_define_const(mCT, "CHARS",     INT2FIX(CT_CHARS));
    rb_define_const(mCT, "FPSTRING",  INT2FIX(CT_FPSTRING));
    rb_define_const(mCT, "F2STRING",  INT2FIX(CT_F2STRING));
    rb_define_const(mCT, "F4STRING",  INT2FIX(CT_F4STRING));
    rb_define_const(mCT, "BIGINT",    INT2FIX(CT_BIGINT));
    rb_define_const(mCT, "NUMBER",    INT2FIX(CT_NUMBER));
    rb_define_const(mCT, "CURRENCY",  INT2FIX(CT_CURRENCY));
    rb_define_const(mCT, "PSTRING",   INT2FIX(CT_PSTRING));
    rb_define_const(mCT, "VARBINARY", INT2FIX(CT_VARBINARY));
    rb_define_const(mCT, "LVB",       INT2FIX(CT_LVB));
    rb_define_const(mCT, "VARCHAR",   INT2FIX(CT_VARCHAR));
    rb_define_const(mCT, "LVC",       INT2FIX(CT_LVC));
    
    // c-ctreeDB Table alter modes
    rb_define_const(mCT, "DB_ALTER_NORMAL",   INT2FIX(CTDB_ALTER_NORMAL));
    rb_define_const(mCT, "DB_ALTER_INDEX",    INT2FIX(CTDB_ALTER_INDEX));
    rb_define_const(mCT, "DB_ALTER_FULL",     INT2FIX(CTDB_ALTER_FULL));
    rb_define_const(mCT, "DB_ALTER_PURGEDUP", INT2FIX(CTDB_ALTER_PURGEDUP));
    // c-treeDB Table status
    rb_define_const(mCT, "DB_REBUILD_NONE",     INT2FIX(CTDB_REBUILD_NONE));
    rb_define_const(mCT, "DB_REBUILD_DODA",     INT2FIX(CTDB_REBUILD_DODA));
    rb_define_const(mCT, "DB_REBUILD_RESOURCE", INT2FIX(CTDB_REBUILD_RESOURCE));
    rb_define_const(mCT, "DB_REBUILD_INDEX",    INT2FIX(CTDB_REBUILD_INDEX));
    rb_define_const(mCT, "DB_REBUILD_ALL",      INT2FIX(CTDB_REBUILD_ALL));
    rb_define_const(mCT, "DB_REBUILD_FULL",     INT2FIX(CTDB_REBUILD_FULL));

    init_rb_ct_session();
    init_rb_ct_table();
    init_rb_ct_field();
    init_rb_ct_index();
    init_rb_ct_segment();
    init_rb_ct_record();
    init_rb_ct_date();
    init_rb_ct_time();
}
