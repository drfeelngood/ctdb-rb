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
    assert_nothing_raised { @table.add_field("t_uinteger",  CT::UINTEGER,    4) }
    assert_nothing_raised { @table.add_field("t_integer",   CT::INTEGER,     4) }
    assert_nothing_raised { @table.add_field("t_bool",      CT::BOOL,        1) }
    assert_nothing_raised { @table.add_field("t_tinyint",   CT::TINYINT,     1) }
    assert_nothing_raised { @table.add_field("t_utinyint",  CT::UTINYINT,    1) }
    assert_nothing_raised { @table.add_field("t_smallint",  CT::SMALLINT,    2) }
    assert_nothing_raised { @table.add_field("t_usmallint", CT::USMALLINT,   2) }
    assert_nothing_raised { @table.add_field("t_money",     CT::MONEY,       4) }
    assert_nothing_raised { @table.add_field("t_date",      CT::DATE,        4) }
    assert_nothing_raised { @table.add_field("t_time",      CT::TIME,        4) }
    assert_nothing_raised { @table.add_field("t_float",     CT::FLOAT,       4) }
    assert_nothing_raised { @table.add_field("t_double",    CT::DOUBLE,      8) }
    assert_nothing_raised { @table.add_field("t_timestamp", CT::TIMESTAMP,   8) }
    assert_nothing_raised { @table.add_field("t_efloat",    CT::EFLOAT,     16) }
    assert_nothing_raised { @table.add_field("t_binary",    CT::BINARY,    256) }
    assert_nothing_raised { @table.add_field("t_chars",     CT::CHARS,      32) }
    assert_nothing_raised { @table.add_field("t_fpstring",  CT::FPSTRING,    1) }
    assert_nothing_raised { @table.add_field("t_f2string",  CT::F2STRING,    2) }
    assert_nothing_raised { @table.add_field("t_f4string",  CT::F4STRING,    4) }
    assert_nothing_raised { @table.add_field("t_bigint",    CT::BIGINT,      8) }
    assert_nothing_raised { @table.add_field("t_number",    CT::NUMBER,      8) }
    assert_nothing_raised { @table.add_field("t_currency",  CT::CURRENCY,    8) }
    assert_nothing_raised { @table.add_field("t_pstring",   CT::PSTRING,     1) }
    assert_nothing_raised { @table.add_field("t_varbinary", CT::VARBINARY,   2) }
    assert_nothing_raised { @table.add_field("t_lvb",       CT::LVB,         4) }
    assert_nothing_raised { @table.add_field("t_varchar",   CT::VARCHAR,    16) }

    assert_nothing_raised { @table.path = _c[:table_path] }
    assert_nothing_raised { @table.create(_c[:table_name], CT::CREATE_NORMAL) }
  end

  def test_create_index
    assert_nothing_raised do
      @table = CT::Table.new(@session)
      @table.path = _c[:table_path]
      @table.open(_c[:table_name], CT::OPEN_NORMAL)
    end
    assert_nothing_raised { @field = @table.get_field("t_uinteger") }
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
    assert_equal(26, @table.field_count)
    assert_instance_of(Array, @table.field_names)
    assert_equal(@table.field_count, @table.field_names.size)
    assert_nothing_raised { @table.close }
  end

end
