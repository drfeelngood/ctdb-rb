#include <ctdb_ext.h>
#include <ct_session.h> 

VALUE cCTSession;

extern VALUE mCT;
extern VALUE cCTError;

static void 
free_rb_ct_session(void *ptr) {
    ct_session *session = (ct_session *)ptr;

    if ( ctdbIsActiveSession(session->handle) )
        ctdbLogout(session->handle);

    ctdbFreeSession(session->handle);
    xfree(session);
}

static VALUE
rb_ct_session_new(VALUE klass, VALUE mode)
{
    VALUE obj;
    ct_session *session;

    Check_Type(mode, T_FIXNUM);
    
    obj = Data_Make_Struct(klass, ct_session, 0, free_rb_ct_session, session);

    // Allocate a new session for logon only. No session or database dictionary
    // files will be used. No database functions can be used with this session 
    // mode.
    if ( ( session->handle = ctdbAllocSession(FIX2INT(mode)) ) == NULL )
        rb_raise(cCTError, "ctdbAllocSession failed.");

    rb_obj_call_init(obj, 0, NULL); // CT::Session.initialize

    return obj;
}

/*
 * Retrieve the active state of a table.  A table is active if it is open.
 */
static VALUE 
rb_ct_session_is_active(VALUE self)
{
    ct_session *session;

    GetCTSession(self, session);

    return ctdbIsActiveSession(session->handle) ? Qtrue : Qfalse;
}

/*
 * Perform a session-wide lock.
 *
 * @param [Fixnum] mode A c-treeDB lock mode.
 */
static VALUE
rb_ct_session_lock(VALUE self, VALUE mode)
{
    ct_session *session;

    Check_Type(mode, T_FIXNUM);

    GetCTSession(self, session);

    if ( ctdbLock(session->handle, FIX2INT(mode)) == CTDBRET_OK ) 
        return Qtrue;
    else
        return Qfalse;
}

/*
 * @see #lock
 * @raise [CT::Error] ctdbLock failed.
 */
static VALUE
rb_ct_session_lock_bang(VALUE self, VALUE mode)
{
    ct_session *session;

    Check_Type(mode, T_FIXNUM);

    GetCTSession(self, session);
    
    if ( ctdbLock(session->handle, FIX2INT(mode)) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbLock failed.", 
            ctdbGetError(session->handle));

    return Qtrue;
}

/* 
 * Check to see if a lock is active
 */
static VALUE
rb_ct_session_is_locked(VALUE self)
{
    ct_session *session;
    
    GetCTSession(self, session);
    
    return ctdbIsLockActive(session->handle) == YES ? Qtrue : Qfalse;
}

/*
 * Logon to c-tree Server or c-treeACE instance session.
 *
 * @param [String] host c-tree Server name or c-treeACE instance name.
 * @param [String] username 
 * @param [String] password
 * @raise [CT::Error] ctdbLogon failed.
 */
static VALUE 
rb_ct_session_logon(VALUE self, VALUE engine, VALUE user, VALUE password)
{
    pTEXT e, u, p;
    ct_session *session;

    Check_Type(engine, T_STRING);
    Check_Type(user, T_STRING);
    Check_Type(user, T_STRING);
    
    GetCTSession(self, session);

    e = RSTRING_PTR(engine);
    u = RSTRING_PTR(user);
    p = RSTRING_PTR(password);

    if ( ctdbLogon(session->handle, e, u, p) != CTDBRET_OK)
        rb_raise(cCTError, "[%d] ctdbLogon failed.", 
            ctdbGetError(session->handle));

    return self;
}

/* Logout from a c-tree Server session or from a c-treeACE instance.
 */
static VALUE 
rb_ct_session_logout(VALUE self)
{
    ct_session *session;
    
    GetCTSession(self, session);

    if ( ctdbLogout(session->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbLogout failed.", 
            ctdbGetError(session->handle));
  
    return self;
}

/* Return the user password associated with the session.
 *
 * @return [String]
 * @raise [CT::Error] ctdbGetUserPassword failed.
 */
static VALUE
rb_ct_session_get_password(VALUE self)
{
    pTEXT passwd;
    ct_session *session;
    
    GetCTSession(self, session);

    if ( ( passwd = ctdbGetUserPassword(session->handle) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetUserPassword failed.", 
            ctdbGetError(session->handle));

    return rb_str_new_cstr(passwd);
}

/* Returns the client-side path prefix.
 * 
 * @return [String, nil] The path name or nil if no path is set.
 */
static VALUE
rb_ct_session_get_path_prefix(VALUE self)
{
    pTEXT prefix;
    ct_session *session;

    GetCTSession(self, session);
    prefix = ctdbGetPathPrefix(session->handle);
    
    return prefix ? rb_str_new_cstr(prefix) : Qnil;
}

/*
 * Set the client-side path prefix.
 *
 * @param [String] prefix Path.
 * @raise [CT::Error] ctdbSetPathPrefix failed.
 */
static VALUE
rb_ct_session_set_path_prefix(VALUE self, VALUE prefix)
{
    pTEXT pp;
    ct_session *session;

    GetCTSession(self, session);
    
    switch ( rb_type(prefix) ) {
        case T_STRING :
            pp = ((RSTRING_LEN(prefix) == 0) ? NULL : RSTRING_PTR(prefix));
            break;
        case T_NIL :
            pp = NULL;
            break;
        default :
            rb_raise(rb_eArgError, "Unexpected value type `%s'", 
                rb_obj_classname(prefix));
            break;
    }

    if ( ctdbSetPathPrefix(session->handle, pp) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbSetPathPrefix failed.", 
            ctdbGetError(session->handle));

    return self;
}

/* Releases all session-wide locks
 *
 * @return [Boolean]
 */
static VALUE
rb_ct_session_unlock(VALUE self)
{
    ct_session *session;  
    
    GetCTSession(self, session);
    
    return ctdbUnlock(session->handle) == CTDBRET_OK ? Qtrue : Qfalse;
}

/* 
 *
 * @see #unlock
 * @raise [CT::Error] ctdbUnlock failed.
 */
static VALUE
rb_ct_session_unlock_bang(VALUE self)
{
    ct_session *session;
    
    GetCTSession(self, session);

    if ( ctdbUnlock(session->handle) != CTDBRET_OK )
        rb_raise(cCTError, "[%d] ctdbUnlock failed.", 
            ctdbGetError(session->handle));

    return self;
}

/* Return the user name associated with the session.
 *
 * @return [String]
 * @raise [CT::Error] ctdbGetUserLogonName failed.
 */
static VALUE
rb_ct_session_get_username(VALUE self)
{
    pTEXT name;
    ct_session *session;

    GetCTSession(self, session);

    if ( ( name = ctdbGetUserLogonName(session->handle) ) == NULL )
        rb_raise(cCTError, "[%d] ctdbGetUserLogonName failed.", 
            ctdbGetError(session->handle));

    return rb_str_new_cstr(name);
}


void init_rb_ct_session()
{
    /*
     *mCT = rb_define_module("CT");
     */
    cCTSession = rb_define_class_under(mCT, "Session", rb_cObject);
    
    rb_define_singleton_method(cCTSession, "new", rb_ct_session_new, 1);
    
    rb_define_method(cCTSession, "active?", rb_ct_session_is_active, 0);
    /*
     *rb_define_method(cCTSession, "default_date_type", rb_ct_get_defualt_date_type, 0);
     *rb_define_method(cCTSession, "default_date_type=", rb_ct_set_defualt_date_type, 1);
     */
    rb_define_method(cCTSession, "lock", rb_ct_session_lock, 1);
    rb_define_method(cCTSession, "lock!", rb_ct_session_lock_bang, 1);
    rb_define_method(cCTSession, "locked?", rb_ct_session_is_locked, 0);
    rb_define_method(cCTSession, "logon", rb_ct_session_logon, 3);
    rb_define_method(cCTSession, "logout", rb_ct_session_logout, 0);
    rb_define_method(cCTSession, "password", rb_ct_session_get_password, 0);
    rb_define_method(cCTSession, "path_prefix", rb_ct_session_get_path_prefix, 0);
    rb_define_method(cCTSession, "path_prefix=", rb_ct_session_set_path_prefix, 1);
    rb_define_method(cCTSession, "unlock", rb_ct_session_unlock, 0);
    rb_define_method(cCTSession, "unlock!", rb_ct_session_unlock_bang, 0);
    rb_define_method(cCTSession, "username", rb_ct_session_get_username, 0);
}
