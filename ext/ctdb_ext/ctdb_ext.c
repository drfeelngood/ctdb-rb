#include <ctdb_ext.h>

VALUE mCT;
VALUE cCTError;

void 
Init_ctdb_ext(void) 
{
    mCT      = rb_define_module("CT");
    cCTError = rb_define_class_under(mCT, "Error", rb_eStandardError);
   
    // c-treeDB Session Types
    rb_define_const(mCT, "SESSION_CTDB",  INT2NUM(CTSESSION_CTDB));
    rb_define_const(mCT, "SESSION_CTREE", INT2NUM(CTSESSION_CTREE));
    // c-treeDB Find Modes
    rb_define_const(mCT, "FIND_EQ", INT2NUM(CTFIND_EQ));
    rb_define_const(mCT, "FIND_LT", INT2NUM(CTFIND_LT));
    rb_define_const(mCT, "FIND_LE", INT2NUM(CTFIND_LE));
    rb_define_const(mCT, "FIND_GT", INT2NUM(CTFIND_GT));
    rb_define_const(mCT, "FIND_GE", INT2NUM(CTFIND_GE));
    // c-treeDB Lock Modes
    rb_define_const(mCT, "LOCK_FREE",       INT2NUM(CTLOCK_FREE));
    rb_define_const(mCT, "LOCK_READ",       INT2NUM(CTLOCK_READ));
    rb_define_const(mCT, "LOCK_READ_BLOCK", INT2NUM(CTLOCK_READ_BLOCK));
    rb_define_const(mCT, "LOCK_WRITE",      INT2NUM(CTLOCK_WRITE));
    rb_define_const(mCT, "LOCK_WRITE_LOCK", INT2NUM(CTLOCK_WRITE_BLOCK));
    // c-treeDB Table create Modes
    rb_define_const(mCT, "CREATE_NORMAL",    INT2NUM(CTCREATE_NORMAL));
    rb_define_const(mCT, "CREATE_PREIMG",    INT2NUM(CTCREATE_PREIMG));
    rb_define_const(mCT, "CREATE_TRNLOG",    INT2NUM(CTCREATE_TRNLOG));
    rb_define_const(mCT, "CREATE_WRITETHRU", INT2NUM(CTCREATE_WRITETHRU));
    rb_define_const(mCT, "CREATE_CHECKLOCK", INT2NUM(CTCREATE_CHECKLOCK));
    rb_define_const(mCT, "CREATE_NORECBYT",  INT2NUM(CTCREATE_NORECBYT));
    rb_define_const(mCT, "CREATE_NOROWID",   INT2NUM(CTCREATE_NOROWID));
    rb_define_const(mCT, "CREATE_CHECKREAD", INT2NUM(CTCREATE_CHECKREAD));
    rb_define_const(mCT, "CREATE_HUGEFILE",  INT2NUM(CTCREATE_HUGEFILE));
    rb_define_const(mCT, "CREATE_NODELFLD",  INT2NUM(CTCREATE_NODELFLD));
    rb_define_const(mCT, "CREATE_NONULFLD",  INT2NUM(CTCREATE_NONULFLD));
    // c-treeDB Table open modes
    rb_define_const(mCT, "OPEN_NORMAL",    INT2NUM(CTOPEN_NORMAL));
    rb_define_const(mCT, "OPEN_DATAONLY",  INT2NUM(CTOPEN_DATAONLY));
    rb_define_const(mCT, "OPEN_EXCLUSIVE", INT2NUM(CTOPEN_EXCLUSIVE));
    rb_define_const(mCT, "OPEN_PERMANENT", INT2NUM(CTOPEN_PERMANENT));
    rb_define_const(mCT, "OPEN_CORRUPT",   INT2NUM(CTOPEN_CORRUPT));
    rb_define_const(mCT, "OPEN_CHECKLOCK", INT2NUM(CTOPEN_CHECKLOCK));
    rb_define_const(mCT, "OPEN_CHECKREAD", INT2NUM(CTOPEN_CHECKREAD));
    rb_define_const(mCT, "OPEN_READONLY",  INT2NUM(CTOPEN_READONLY));
    // c-treeDB Table Permissions
    rb_define_const(mCT, "OPF_READ",   INT2NUM(OPF_READ));
    rb_define_const(mCT, "OPF_WRITE",  INT2NUM(OPF_WRITE));
    rb_define_const(mCT, "OPF_DEF",    INT2NUM(OPF_DEF));
    rb_define_const(mCT, "OPF_DELETE", INT2NUM(OPF_DELETE));
    rb_define_const(mCT, "OPF_ALL",    INT2NUM(OPF_ALL));
    rb_define_const(mCT, "OPF_NOPASS", INT2NUM(OPF_NOPASS));
    rb_define_const(mCT, "GPF_NONE",   INT2NUM(GPF_NONE));
    rb_define_const(mCT, "GPF_READ",   INT2NUM(GPF_READ));
    rb_define_const(mCT, "GPF_WRITE",  INT2NUM(GPF_WRITE));
    rb_define_const(mCT, "GPF_DEF",    INT2NUM(GPF_DEF));
    rb_define_const(mCT, "GPF_DELETE", INT2NUM(GPF_DELETE));
    rb_define_const(mCT, "GPF_NOPASS", INT2NUM(GPF_NOPASS));
    rb_define_const(mCT, "WPF_NONE",   INT2NUM(WPF_NONE));
    rb_define_const(mCT, "WPF_READ",   INT2NUM(WPF_READ));
    rb_define_const(mCT, "WPF_WRITE",  INT2NUM(WPF_WRITE));
    rb_define_const(mCT, "WPF_DEF",    INT2NUM(WPF_DEF));
    rb_define_const(mCT, "WPF_DELETE", INT2NUM(WPF_DELETE));
    rb_define_const(mCT, "WPF_NOPASS", INT2NUM(WPF_NOPASS));
    // c-treeDB Index Types
    rb_define_const(mCT, "INDEX_FIXED",   INT2NUM(CTINDEX_FIXED));
    rb_define_const(mCT, "INDEX_LEADING", INT2NUM(CTINDEX_LEADING));
    rb_define_const(mCT, "INDEX_PADDING", INT2NUM(CTINDEX_PADDING));
    rb_define_const(mCT, "INDEX_LEADPAD", INT2NUM(CTINDEX_LEADPAD));
    rb_define_const(mCT, "INDEX_ERROR",   INT2NUM(CTINDEX_ERROR));
    // c-treeDB Segment modes
    rb_define_const(mCT, "SEG_SCHSEG",     INT2NUM(CTSEG_SCHSEG));
    rb_define_const(mCT, "SET_USCHSEG",    INT2NUM(CTSEG_USCHSEG));
    rb_define_const(mCT, "SEG_VSCHSEG",    INT2NUM(CTSEG_VSCHSEG));
    rb_define_const(mCT, "SEG_UVSCHSEG",   INT2NUM(CTSEG_UVSCHSEG));
    rb_define_const(mCT, "SEG_SCHSRL",     INT2NUM(CTSEG_SCHSRL));
    rb_define_const(mCT, "SEG_ALTSEG",     INT2NUM(CTSEG_ALTSEG));
    rb_define_const(mCT, "SEG_ENDSEG",     INT2NUM(CTSEG_ENDSEG));
    rb_define_const(mCT, "SEG_REGSEG",     INT2NUM(CTSEG_REGSEG));
    rb_define_const(mCT, "SEG_INTSEG",     INT2NUM(CTSEG_INTSEG));
    rb_define_const(mCT, "SEG_UREGSEG",    INT2NUM(CTSEG_UREGSEG));
    rb_define_const(mCT, "SEG_SRLSEG",     INT2NUM(CTSEG_SRLSEG));
    rb_define_const(mCT, "SEG_VARSEG",     INT2NUM(CTSEG_VARSEG));
    rb_define_const(mCT, "SEG_UVARSEG",    INT2NUM(CTSEG_UVARSEG));
    rb_define_const(mCT, "SEG_SGNSEG",     INT2NUM(CTSEG_SGNSEG));
    rb_define_const(mCT, "SEG_FLTSEG",     INT2NUM(CTSEG_FLTSEG));
    rb_define_const(mCT, "SEG_DECSEG",     INT2NUM(CTSEG_DECSEG));
    rb_define_const(mCT, "SEG_BCDSEG",     INT2NUM(CTSEG_BCDSEG));
    rb_define_const(mCT, "SEG_DESCENDING", INT2NUM(CTSEG_DESCENDING));

    // c-treeDB Field Types
    rb_define_const(mCT, "BOOL",      INT2NUM(CT_BOOL));
    rb_define_const(mCT, "TINYINT",   INT2NUM(CT_TINYINT));
    rb_define_const(mCT, "UTINYINT",  INT2NUM(CT_UTINYINT));
    rb_define_const(mCT, "SMALLINT",  INT2NUM(CT_SMALLINT));
    rb_define_const(mCT, "USMALLINT", INT2NUM(CT_USMALLINT));
    rb_define_const(mCT, "INTEGER",   INT2NUM(CT_INTEGER));
    rb_define_const(mCT, "UINTEGER",  INT2NUM(CT_UINTEGER));
    rb_define_const(mCT, "MONEY",     INT2NUM(CT_MONEY));
    rb_define_const(mCT, "DATE",      INT2NUM(CT_DATE));
    rb_define_const(mCT, "TIME",      INT2NUM(CT_TIME));
    rb_define_const(mCT, "FLOAT",     INT2NUM(CT_FLOAT));
    rb_define_const(mCT, "DOUBLE",    INT2NUM(CT_DOUBLE));
    rb_define_const(mCT, "TIMESTAMP", INT2NUM(CT_TIMESTAMP));
    rb_define_const(mCT, "EFLOAT",    INT2NUM(CT_EFLOAT));
    rb_define_const(mCT, "BINARY",    INT2NUM(CT_BINARY));
    rb_define_const(mCT, "CHARS",     INT2NUM(CT_CHARS));
    rb_define_const(mCT, "FPSTRING",  INT2NUM(CT_FPSTRING));
    rb_define_const(mCT, "F2STRING",  INT2NUM(CT_F2STRING));
    rb_define_const(mCT, "F4STRING",  INT2NUM(CT_F4STRING));
    rb_define_const(mCT, "BIGINT",    INT2NUM(CT_BIGINT));
    rb_define_const(mCT, "NUMBER",    INT2NUM(CT_NUMBER));
    rb_define_const(mCT, "CURRENCY",  INT2NUM(CT_CURRENCY));
    rb_define_const(mCT, "PSTRING",   INT2NUM(CT_PSTRING));
    rb_define_const(mCT, "VARBINARY", INT2NUM(CT_VARBINARY));
    rb_define_const(mCT, "LVB",       INT2NUM(CT_LVB));
    rb_define_const(mCT, "VARCHAR",   INT2NUM(CT_VARCHAR));
    rb_define_const(mCT, "LVC",       INT2NUM(CT_LVC));
    
    // c-ctreeDB Table alter modes
    rb_define_const(mCT, "DB_ALTER_NORMAL",   INT2NUM(CTDB_ALTER_NORMAL));
    rb_define_const(mCT, "DB_ALTER_INDEX",    INT2NUM(CTDB_ALTER_INDEX));
    rb_define_const(mCT, "DB_ALTER_FULL",     INT2NUM(CTDB_ALTER_FULL));
    rb_define_const(mCT, "DB_ALTER_PURGEDUP", INT2NUM(CTDB_ALTER_PURGEDUP));
    // c-treeDB Table status
    rb_define_const(mCT, "DB_REBUILD_NONE",     INT2NUM(CTDB_REBUILD_NONE));
    rb_define_const(mCT, "DB_REBUILD_DODA",     INT2NUM(CTDB_REBUILD_DODA));
    rb_define_const(mCT, "DB_REBUILD_RESOURCE", INT2NUM(CTDB_REBUILD_RESOURCE));
    rb_define_const(mCT, "DB_REBUILD_INDEX",    INT2NUM(CTDB_REBUILD_INDEX));
    rb_define_const(mCT, "DB_REBUILD_ALL",      INT2NUM(CTDB_REBUILD_ALL));
    rb_define_const(mCT, "DB_REBUILD_FULL",     INT2NUM(CTDB_REBUILD_FULL));

    // c-treeDB date and time types
    rb_define_const(mCT, "DATE_MDCY", INT2NUM(CTDATE_MDCY));
    rb_define_const(mCT, "DATE_MDY",  INT2NUM(CTDATE_MDY));
    rb_define_const(mCT, "DATE_DMCY", INT2NUM(CTDATE_DMCY));
    rb_define_const(mCT, "DATE_DMY",  INT2NUM(CTDATE_DMY));
    rb_define_const(mCT, "DATE_CYMD", INT2NUM(CTDATE_CYMD));
    rb_define_const(mCT, "DATE_YMD",  INT2NUM(CTDATE_YMD));
    rb_define_const(mCT, "TIME_HMSP", INT2NUM(CTTIME_HMSP));
    rb_define_const(mCT, "TIME_HMP",  INT2NUM(CTTIME_HMP));
    rb_define_const(mCT, "TIME_HMS",  INT2NUM(CTTIME_HMS));
    rb_define_const(mCT, "TIME_HM",   INT2NUM(CTTIME_HM));
    rb_define_const(mCT, "TIME_MIL",  INT2NUM(CTTIME_MIL));

    init_rb_ct_session();
    init_rb_ct_table();
    init_rb_ct_field();
    init_rb_ct_index();
    init_rb_ct_segment();
    init_rb_ct_record();
    init_rb_ct_date();
    init_rb_ct_time();
    init_rb_ct_date_time();
}
