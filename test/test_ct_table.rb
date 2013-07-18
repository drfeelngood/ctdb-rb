require File.join(File.dirname(__FILE__), 'test_helper')

class TestCTTable < Test::Unit::TestCase
  include TestHelper

  def setup
    @session = CT::Session.new(CT::SESSION_CTREE)
    @session.logon(_c[:engine], _c[:username], _c[:password])
  end

  def teardown
    @session.logout
  end

  def test_create
    assert_nothing_raised { @table = CT::Table.new(@session) }
    assert_nothing_raised { @table.add_field("uinteger",  CT::UINTEGER,    4) }
    assert_nothing_raised { @table.add_field("integer",   CT::INTEGER,     4) }
    assert_nothing_raised { @table.add_field("bool",      CT::BOOL,        1) }
    assert_nothing_raised { @table.add_field("tinyint",   CT::TINYINT,     1) }
    assert_nothing_raised { @table.add_field("utinyint",  CT::UTINYINT,    1) }
    assert_nothing_raised { @table.add_field("smallint",  CT::SMALLINT,    2) }
    assert_nothing_raised { @table.add_field("usmallint", CT::USMALLINT,   2) }
    assert_nothing_raised { @table.add_field("money",     CT::MONEY,       4) }
    assert_nothing_raised { @table.add_field("date",      CT::DATE,        4) }
    assert_nothing_raised { @table.add_field("time",      CT::TIME,        4) }
    assert_nothing_raised { @table.add_field("float",     CT::FLOAT,       4) }
    assert_nothing_raised { @table.add_field("double",    CT::DOUBLE,      8) }
    assert_nothing_raised { @table.add_field("timestamp", CT::TIMESTAMP,   8) }
    #assert_nothing_raised { @table.add_field("efloat",    CT::EFLOAT,     16) }
    assert_nothing_raised { @table.add_field("binary",    CT::BINARY,    256) }
    assert_nothing_raised { @table.add_field("chars",     CT::CHARS,      32) }
    assert_nothing_raised { @table.add_field("fpstring",  CT::FPSTRING,    2) }
    assert_nothing_raised { @table.add_field("f2string",  CT::F2STRING,    4) }
    assert_nothing_raised { @table.add_field("f4string",  CT::F4STRING,    8) }
    assert_nothing_raised { @table.add_field("bigint",    CT::BIGINT,      8) }
    assert_nothing_raised { @table.add_field("number",    CT::NUMBER,      8) }
    assert_nothing_raised { @table.add_field("currency",  CT::CURRENCY,    8) }
    assert_nothing_raised { @table.add_field("pstring",   CT::PSTRING,     1) }
    assert_nothing_raised { @table.add_field("varbinary", CT::VARBINARY,   2) }
    assert_nothing_raised { @table.add_field("lvb",       CT::LVB,         4) }
    assert_nothing_raised { @table.add_field("varchar",   CT::VARCHAR,    16) }
    assert_nothing_raised { @table.path = _c[:table_path] }
    assert_nothing_raised { @table.create(_c[:table_name], CT::CREATE_NORMAL) }
  end

  def test_create_index
    assert_nothing_raised do
      @table = CT::Table.new(@session)
      @table.path = _c[:table_path]
      @table.open(_c[:table_name], CT::OPEN_NORMAL)
    end
    assert_nothing_raised { @field = @table.get_field("uinteger") }
    assert_instance_of(CT::Field, @field)
    assert_nothing_raised do 
      @index = @table.add_index(_c[:index_name], CT::INDEX_FIXED)
    end
    assert_instance_of(CT::Index, @index)
    assert_nothing_raised { @index.allow_dups = false }
    assert_nothing_raised { @index.add_segment(@field, CT::SEG_SCHSRL) }
    assert_nothing_raised { @table.alter(CT::DB_ALTER_NORMAL) }
    assert_nothing_raised { @table.close }
    
    assert_nothing_raised do
      @table = CT::Table.new(@session)
      @table.path = _c[:table_path]
      @table.open(_c[:table_name], CT::OPEN_NORMAL)
    end
    assert_nothing_raised do
      @indecies = @table.indecies
    end
    assert_instance_of(Array, @indecies)
    assert_equal(1, @indecies.size)
    assert_nothing_raised do
      @index = @table.get_index(_c[:index_name])    
    end
    assert_instance_of(CT::Index, @index)
    assert_equal(_c[:index_name], @index.name)
    assert_instance_of(Array, @index.segments)
    assert_equal(1, @index.segments.size)
    assert_nothing_raised do
      @segment = @index.segments.first
    end
    assert_instance_of(CT::Segment, @segment)
    assert_nothing_raised { @table.close }
  end

  def test_table_attributes
    assert_nothing_raised { @table = CT::Table.new(@session) }
    assert_nothing_raised { @table.path = _c[:table_path] }
    assert_equal(false, @table.active?)
    assert_equal(false, @table.open?)
    assert_nothing_raised { @table.open(_c[:table_name], CT::OPEN_NORMAL) }
    assert(@table.active?)
    assert(@table.open?)
    assert_instance_of(Fixnum, @table.field_count)
    #assert_equal(26, @table.field_count)
    assert_instance_of(Array, @table.field_names)
    assert_equal(@table.field_count, @table.field_names.size)
    assert_nothing_raised { @table.close }
  end

end
