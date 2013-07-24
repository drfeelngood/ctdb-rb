require File.join(File.dirname(__FILE__), 'test_helper')

class TestCTSession < Test::Unit::TestCase
  include TestHelper

  def test_logon_logout
    assert_nothing_raised { @session = CT::Session.new(CT::SESSION_CTREE) }
    assert_equal(false, @session.active?)
    assert_raise(ArgumentError){ @session.logon }
    assert_nothing_raised do
      @session.logon(_c[:engine], _c[:username], _c[:password])
    end
    assert(@session.active?)
    assert_nothing_raised { @session.logout }
    assert_equal(false, @session.active?)
  end

  def test_path_prefix
    path = "/foo/bar"

    assert_nothing_raised { @session = CT::Session.new(CT::SESSION_CTREE) }
    assert_nothing_raised { @session.path_prefix = path }
    assert_equal(path, @session.path_prefix)
  end

  def test_connection_attributes
    assert_nothing_raised { @session = CT::Session.new(CT::SESSION_CTREE) }
    assert_raise(ArgumentError){ @session.logon }
    assert_nothing_raised do
      @session.logon(_c[:engine], _c[:username], _c[:password])
    end
    assert_equal(_c[:engine], @session.server_name)
    assert_equal(_c[:username], @session.username)
    assert_equal(_c[:password], @session.password)
    assert_nothing_raised { @session.logout }
  end
  
  def test_locking
    assert_nothing_raised { @session = CT::Session.new(CT::SESSION_CTREE) }
    assert_raise(ArgumentError){ @session.logon }
    assert_nothing_raised do
      @session.logon(_c[:engine], _c[:username], _c[:password])
    end
    assert_equal(false, @session.locked?)
    assert_nothing_raised { @session.read_lock! }
    assert(@session.locked?)
    assert_nothing_raised { @session.free_all_locks! }
    assert_nothing_raised { @session.logout }
  end

end
