require File.join(File.dirname(__FILE__), 'test_helper')

class TestCTField < Test::Unit::TestCase
  include TestHelper

  def setup
    @session = CT::Session.new(CT::SESSION_CTREE)
    @session.logon(_c[:engine], _c[:username], _c[:password])
    @table = CT::Table.new(@session)
    @table.path = _c[:table_path]
    @table.open(_c[:table_name], CT::OPEN_NORMAL)
  end

  def teardown
    @table.close
    @session.logout
  end

  def test_properties
    assert_nothing_raised { @field = @table.get_field_by_name("uinteger") }
    assert_equal(0, @field.number)
    assert_equal(CT::UINTEGER, @field.type)
    assert_equal("CT_UINTEGER", @field.human_type)
    assert_equal(4, @field.length)
    assert(@field.integer?)
    # assert_instance_of(Hash, @field.properties)
    assert_equal( { name: "uinteger",
                    type: "CT_UINTEGER" ,
                    length: 4 }, @field.to_h )

    assert_nothing_raised { @field = @table.get_field_by_name("integer") }
    assert_equal(1, @field.number)
    assert_equal(CT::INTEGER, @field.type)
    assert_equal("CT_INTEGER", @field.human_type)
    assert_equal(4, @field.length)
    assert(@field.integer?)
    assert_equal( { name: "integer",
                    type: "CT_INTEGER" ,
                    length: 4 }, @field.to_h )

    assert_nothing_raised { @field = @table.get_field_by_name("bool") }
    assert_equal(2, @field.number)
    assert_equal(CT::BOOL, @field.type)
    assert_equal("CT_BOOL", @field.human_type)
    assert_equal(1, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("tinyint") }
    assert_equal(3, @field.number)
    assert_equal(CT::TINYINT, @field.type)
    assert_equal("CT_TINYINT", @field.human_type)
    assert_equal(1, @field.length)
    assert(@field.integer?)

    assert_nothing_raised { @field = @table.get_field_by_name("utinyint") }
    assert_equal(4, @field.number)
    assert_equal(CT::UTINYINT, @field.type)
    assert_equal("CT_UTINYINT", @field.human_type)
    assert_equal(1, @field.length)
    assert(@field.integer?)

    assert_nothing_raised { @field = @table.get_field_by_name("smallint") }
    assert_equal(5, @field.number)
    assert_equal(CT::SMALLINT, @field.type)
    assert_equal("CT_SMALLINT", @field.human_type)
    assert_equal(2, @field.length)
    assert(@field.integer?)

    assert_nothing_raised { @field = @table.get_field_by_name("usmallint") }
    assert_equal(6, @field.number)
    assert_equal(CT::USMALLINT, @field.type)
    assert_equal("CT_USMALLINT", @field.human_type)
    assert_equal(2, @field.length)
    assert(@field.integer?)

    assert_nothing_raised { @field = @table.get_field_by_name("money") }
    assert_equal(7, @field.number)
    assert_equal(CT::MONEY, @field.type)
    assert_equal("CT_MONEY", @field.human_type)
    assert_equal(4, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("date") }
    assert_equal(CT::DATE, @field.type)
    assert_equal("CT_DATE", @field.human_type)
    assert_equal(4, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("time") }
    assert_equal(CT::TIME, @field.type)
    assert_equal("CT_TIME", @field.human_type)
    assert_equal(4, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("float") }
    assert_equal(CT::FLOAT, @field.type)
    assert_equal("CT_FLOAT", @field.human_type)
    assert_equal(4, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("double") }
    assert_equal(CT::DOUBLE, @field.type)
    assert_equal("CT_DOUBLE", @field.human_type)
    assert_equal(8, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("timestamp") }
    assert_equal(CT::TIMESTAMP, @field.type)
    assert_equal("CT_TIMESTAMP", @field.human_type)
    assert_equal(8, @field.length)

    #assert_nothing_raised { @field = @table.get_field_by_name("efloat") }
    #assert_equal(CT::EFLOAT, @field.type)
    #assert_equal("CT_EFLOAT", @field.human_type)
    #assert_equal(16, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("binary") }
    assert_equal(CT::BINARY, @field.type)
    #assert_equal("BINARY", @field.human_type)
    assert_equal(256, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("chars") }
    assert_equal(CT::CHARS, @field.type)
    assert_equal("CT_CHARS", @field.human_type)
    assert_equal(32, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("fpstring") }
    assert_equal(CT::FPSTRING, @field.type)
    assert_equal("CT_FPSTRING", @field.human_type)
    assert_equal(2, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("f2string") }
    assert_equal(CT::F2STRING, @field.type)
    assert_equal("CT_F2STRING", @field.human_type)
    assert_equal(4, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("f4string") }
    assert_equal(CT::F4STRING, @field.type)
    assert_equal("CT_F4STRING", @field.human_type)
    assert_equal(8, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("bigint") }
    assert_equal(CT::BIGINT, @field.type)
    assert_equal("CT_BIGINT", @field.human_type)
    assert_equal(8, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("number") }
    assert_equal(CT::NUMBER, @field.type)
    assert_equal("CT_NUMBER", @field.human_type)
    # assert_equal(8, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("currency") }
    assert_equal(CT::CURRENCY, @field.type)
    assert_equal("CT_CURRENCY", @field.human_type)
    assert_equal(8, @field.length)

    assert_nothing_raised { @field = @table.get_field_by_name("pstring") }
    assert_equal(CT::PSTRING, @field.type)
    assert_equal("CT_PSTRING", @field.human_type)
    assert_equal(1, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("varbinary") }
    assert_equal(CT::VARBINARY, @field.type)
    assert_equal("CT_VARBINARY", @field.human_type)
    assert_equal(2, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("lvb") }
    assert_equal(CT::LVB, @field.type)
    assert_equal("CT_LVB", @field.human_type)
    assert_equal(4, @field.length)
    assert(@field.string?)

    assert_nothing_raised { @field = @table.get_field_by_name("varchar") }
    assert_equal(CT::VARCHAR, @field.type)
    assert_equal("CT_VARCHAR", @field.human_type)
    assert_equal(16, @field.length)
    assert(@field.string?)
  end

end
