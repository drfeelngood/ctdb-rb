require File.join(File.dirname(__FILE__), 'test_helper')
require 'yaml'

class TestCTRecord < Test::Unit::TestCase
  include TestHelper

  def setup
    @session = CT::Session.new(CT::SESSION_CTREE)
    @session.logon(_c[:engine], _c[:username], _c[:password])
    @table = CT::Table.new(@session)
    @table.path = _c[:table_path]
    @table.open(_c[:table_name], CT::OPEN_NORMAL)
    @r = nil
  end

  def teardown
    @table.close
    @session.logout
  end

  def test_create
    (0..2).each do |n|
      x  = fixtures[n]
      m  = "fixture #{n}"
      @r = CT::Record.new(@table).clear
      assert_nothing_raised(m) { @r.set_field("uinteger",  x["uinteger"]  ) }
      assert_nothing_raised(m) { @r.set_field("integer",   x["integer"]   ) }
      assert_nothing_raised(m) { @r.set_field("bool",      x["bool"]      ) }
      assert_nothing_raised(m) { @r.set_field("tinyint",   x["tinyint"]   ) }
      assert_nothing_raised(m) { @r.set_field("utinyint",  x["utinyint"]  ) }
      assert_nothing_raised(m) { @r.set_field("smallint",  x["smallint"]  ) }
      assert_nothing_raised(m) { @r.set_field("usmallint", x["usmallint"] ) }
      assert_nothing_raised(m) { @r.set_field("money",     x["money"]     ) }
      assert_nothing_raised(m) { @r.set_field("date",      x["date"]      ) }
      assert_nothing_raised(m) { @r.set_field("time",      x["time"]      ) }
      assert_nothing_raised(m) { @r.set_field("float",     x["float"]     ) }
      assert_nothing_raised(m) { @r.set_field("double",    x["double"]    ) }
      assert_nothing_raised(m) { @r.set_field("timestamp", x["timestamp"] ) }
      #assert_nothing_raised(m) { @r.set_field("efloat",    x["efloat"]    ) }
      assert_nothing_raised(m) { @r.set_field("binary",    x["binary"]    ) }
      assert_nothing_raised(m) { @r.set_field("chars",     x["chars"]     ) }
      assert_nothing_raised(m) { @r.set_field("fpstring",  x["fpstring"]  ) }
      assert_nothing_raised(m) { @r.set_field("f2string",  x["f2string"]  ) }
      assert_nothing_raised(m) { @r.set_field("f4string",  x["f4string"]  ) }
      assert_nothing_raised(m) { @r.set_field("bigint",    x["bigint"]    ) }
      #assert_nothing_raised(m) { @r.set_field("number",    x["number"]    ) }
      assert_nothing_raised(m) { @r.set_field("currency",  x["currency"]  ) }
      assert_nothing_raised(m) { @r.set_field("pstring",   x["pstring"]   ) }
      #assert_nothing_raised(m) { @r.set_field("varbinary", x["varbinary"] ) }
      #assert_nothing_raised(m) { @r.set_field("lvb",       x["lvb"]       ) }
      assert_nothing_raised(m) { @r.set_field("varchar",   x["varchar"]   ) }
      assert_nothing_raised { @r.write! }
    end

    assert_equal(3, @r.count)
  end

  def test_find_eq
    x = fixtures[1]
    assert_nothing_raised { @r = CT::Record.new(@table) }
    assert_nothing_raised { @r.clear }

    assert_nothing_raised { @r.set_field('uinteger', x['uinteger']) }
    assert_nothing_raised { @r.find(CT::FIND_EQ) }
    assert_equal(x["uinteger"],  @r.get_field("uinteger"))
    assert_equal(x["integer"],   @r.get_field("integer"))
    assert_equal(x["bool"],      @r.get_field("bool"))
    assert_equal(x["tinyint"],   @r.get_field("tinyint"))
    assert_equal(x["utinyint"],  @r.get_field("utinyint"))
    assert_equal(x["smallint"],  @r.get_field("smallint"))
    assert_equal(x["usmallint"], @r.get_field("usmallint"))
    assert_equal(x["money"],     @r.get_field("money"))
    assert_equal(x["float"],     @r.get_field("float"))
    assert_equal(x["double"],    @r.get_field("double"))

    #assert_equal(x["efloat"]    )
    #assert_equal(x["binary"]    )
    assert_equal(x["chars"],     @r.get_field("chars"))
    assert_equal(x["fpstring"],  @r.get_field("fpstring"))
    assert_equal(x["f2string"],  @r.get_field("f2string"))
    assert_equal(x["f4string"],  @r.get_field("f4string"))
    assert_equal(x["bigint"],    @r.get_field("bigint"))
    #assert_equal(x["number"]    )
    assert_equal(x["currency"],  @r.get_field("currency"))
    assert_equal(x["pstring"],   @r.get_field("pstring"))
    #assert_equal(x["varbinary"] )
    #assert_equal(x["lvb"]       )
    assert_equal(x["varchar"],   @r.get_field("varchar"))
    
    date = x["date"].to_ctdb
    time = x["time"].to_ctdb
    datetime = x["timestamp"].to_ctdb
    
    assert_equal(date.to_s, @r.get_field("date").to_s)
    assert_equal(time.hour, @r.get_field("time").hour)
    assert_equal(time.min,  @r.get_field("time").min)
    assert_equal(time.sec,  @r.get_field("time").sec)
  end

  def test_find_first
    x = fixtures[0]
    assert_nothing_raised { @r = CT::Record.new(@table) }
    assert_nothing_raised { @r.clear }
    assert_nothing_raised { @r.first }
    
    assert_equal(x["uinteger"],  @r.get_field("uinteger"))
    assert_equal(x["integer"],   @r.get_field("integer"))
    assert_equal(x["bool"],      @r.get_field("bool"))
    assert_equal(x["tinyint"],   @r.get_field("tinyint"))
    assert_equal(x["utinyint"],  @r.get_field("utinyint"))
    assert_equal(x["smallint"],  @r.get_field("smallint"))
    assert_equal(x["usmallint"], @r.get_field("usmallint"))
    assert_equal(x["money"],     @r.get_field("money"))
    assert_equal(x["float"],     @r.get_field("float"))
    assert_equal(x["double"],    @r.get_field("double"))
    #assert_equal(x["efloat"]    )
    #assert_equal(x["binary"]    )
    assert_equal(x["chars"],     @r.get_field("chars"))
    assert_equal(x["fpstring"],  @r.get_field("fpstring"))
    assert_equal(x["f2string"],  @r.get_field("f2string"))
    assert_equal(x["f4string"],  @r.get_field("f4string"))
    assert_equal(x["bigint"],    @r.get_field("bigint"))
    #assert_equal(x["number"]    )
    assert_equal(x["currency"],  @r.get_field("currency"))
    assert_equal(x["pstring"],   @r.get_field("pstring"))
    #assert_equal(x["varbinary"] )
    #assert_equal(x["lvb"]       )
    assert_equal(x["varchar"],   @r.get_field("varchar"))
   

    date = x["date"].to_ctdb
    time = x["time"].to_ctdb
    datetime = x["timestamp"].to_ctdb
    
    assert_equal(date.to_s, @r.get_field("date").to_s)
    assert_equal(time.hour, @r.get_field("time").hour)
    assert_equal(time.min,  @r.get_field("time").min)
    assert_equal(time.sec,  @r.get_field("time").sec)
  end

  def test_find_last
    x = fixtures[2]
    assert_nothing_raised { @r = CT::Record.new(@table) }
    assert_nothing_raised { @r.clear }
    assert_nothing_raised { @r.last }
    
    assert_equal(x["uinteger"],  @r.get_field("uinteger"))
    assert_equal(x["integer"],   @r.get_field("integer"))
    assert_equal(x["bool"],      @r.get_field("bool"))
    assert_equal(x["tinyint"],   @r.get_field("tinyint"))
    assert_equal(x["utinyint"],  @r.get_field("utinyint"))
    assert_equal(x["smallint"],  @r.get_field("smallint"))
    assert_equal(x["usmallint"], @r.get_field("usmallint"))
    assert_equal(x["money"],     @r.get_field("money"))
    assert_equal(x["float"],     @r.get_field("float"))
    assert_equal(x["double"],    @r.get_field("double"))
    #assert_equal(x["efloat"]    )
    #assert_equal(x["binary"]    )
    assert_equal(x["chars"],     @r.get_field("chars"))
    assert_equal(x["fpstring"],  @r.get_field("fpstring"))
    assert_equal(x["f2string"],  @r.get_field("f2string"))
    assert_equal(x["f4string"],  @r.get_field("f4string"))
    assert_equal(x["bigint"],    @r.get_field("bigint"))
    #assert_equal(x["number"]    )
    assert_equal(x["currency"],  @r.get_field("currency"))
    assert_equal(x["pstring"],   @r.get_field("pstring"))
    #assert_equal(x["varbinary"] )
    #assert_equal(x["lvb"]       )
    assert_equal(x["varchar"],   @r.get_field("varchar"))
    
    date = x["date"].to_ctdb
    time = x["time"].to_ctdb
    datetime = x["timestamp"].to_ctdb

    assert_equal(date.to_s, @r.get_field("date").to_s)
    assert_equal(time.hour, @r.get_field("time").hour)
    assert_equal(time.min,  @r.get_field("time").min)
    assert_equal(time.sec,  @r.get_field("time").sec)
  end

  def test_navigation
    assert_nothing_raised { @r = CT::Record.new(@table) }
    assert_nothing_raised { @r.clear }
    assert_nothing_raised { @r.first }
    assert_equal(fixtures[0]['uinteger'], @r.get_field('uinteger'))
    assert_nothing_raised { @r.next }
    assert_equal(fixtures[1]['uinteger'], @r.get_field('uinteger'))
    assert_nothing_raised { @r.next }
    assert_equal(fixtures[2]['uinteger'], @r.get_field('uinteger'))
    assert_nil(@r.next) # => end of file
    assert_nothing_raised { @r.prev }
    assert_equal(fixtures[1]['uinteger'], @r.get_field('uinteger'))
    assert_nothing_raised { @r.prev }
    assert_equal(fixtures[0]['uinteger'], @r.get_field('uinteger'))
    assert_nil(@r.prev) # => end of file
  end

  #def test_record_set
    #assert_nothing_raised { @r = CT::Record.new(@table) }
    #assert_nothing_raised { @r.clear }
  #end

  def test_record_locking
    assert_nothing_raised { @r = CT::Record.new(@table) }
    assert_nothing_raised { @r.clear }
    assert_nothing_raised { @r.first }
    assert_equal(CT::LOCK_FREE, @r.lock_mode)
    assert_equal("CTLOCK_FREE", @r.human_lock_mode)
    assert_equal(false, @r.locked?)
    assert(@r.lock(CT::LOCK_READ))
    assert_equal(CT::LOCK_READ, @r.lock_mode)
    assert_equal("CTLOCK_READ", @r.human_lock_mode)
    assert(@r.locked?)
    assert(@r.read_locked?)
    assert_equal(false, @r.write_locked?)
    assert(@r.lock(CT::LOCK_WRITE))
    assert_equal(CT::LOCK_WRITE, @r.lock_mode)
    assert_equal("CTLOCK_WRITE", @r.human_lock_mode)
    assert(@r.locked?)
    assert(@r.write_locked?)
    assert_equal(false, @r.read_locked?)
  end

end
