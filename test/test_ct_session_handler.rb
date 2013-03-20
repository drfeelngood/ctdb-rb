require File.dirname(__FILE__) + '/test_helper'

class CTTestSessionHandler < Test::Unit::TestCase
  include TestHelper

  def setup
    session = CT::Session.new(CT::SESSION_CTREE)
    session.logon(_c[:engine], _c[:user], _c[:password])
    @session_handler = CT::SessionHandler.new(session) 
  end

  def test_open_table
    @ctdb_sdk = nil
    assert_nothing_raised do
      @ctdb_sdk = @session_handler.open_table(TestModel.table_path, 
                                              TestModel.table_name)
    end
    assert_equal(1, @session_handler.open_tables.size)
    
    assert_nothing_raised do
      @ctdb_sdk = @session_handler.open_table(TestModel.table_path, 
                                              TestModel.table_name)
    end
    assert_equal(1, @session_handler.open_tables.size)
  end

end
