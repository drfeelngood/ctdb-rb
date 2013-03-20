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
  end

  def teardown
    @table.close
    @session.logout
  end

  def test_create
    x = fixtures[0]
    assert_nothing_raised { @r0 = CT::Record.new(@table).clear }
    assert_nothing_raised { @r0.set_field_as_bool('t_bool', x['t_bool']) }
    assert_nothing_raised { @r0.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r0.set_field_as_signed('t_tinyint', x['t_tinyint']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('t_utinyint', x['t_utinyint']) }
    assert_nothing_raised { @r0.set_field_as_signed('t_smallint', x['t_smallint']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('t_usmallint', x['t_usmallint']) }
    assert_nothing_raised { @r0.set_field_as_signed('t_integer', x['t_integer']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('t_uinteger', x['t_uinteger']) }
    # assert_nothing_raised { @r0.set_field_as_money('t_money', x['t_uinteger']) }
    assert_nothing_raised { @r0.set_field_as_date('t_date', x['t_date']) }
    assert_nothing_raised { @r0.set_field_as_time('t_time', x['t_time']) }
    # assert_nothing_raised { @r0.set_field_as_float('t_float', x['t_float']) }
    # assert_nothing_raised {  t_double: }
    # assert_nothing_raised {  t_timestamp: }
    # assert_nothing_raised {  t_efloat: }
    # assert_nothing_raised {  t_binary: }
    assert_nothing_raised { @r0.set_field_as_string('t_chars', x['t_chars']) }
    assert_nothing_raised { @r0.set_field_as_string('t_fpstring', x['t_fpstring']) }
    assert_nothing_raised { @r0.set_field_as_string('t_f2string', x['t_f2string']) }
    assert_nothing_raised { @r0.set_field_as_string('t_f4string', x['t_f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r0.set_field_as_string('t_pstring', x['t_pstring']) }
    # assert_nothing_raised { @r0.set_field_as_blob('t_varbinary', x['t_varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r0.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r0.write! }

    x = fixtures[1]
    assert_nothing_raised { @r1 = CT::Record.new(@table).clear }
    assert_nothing_raised { @r1.set_field_as_bool('t_bool', x['t_bool']) }
    assert_nothing_raised { @r1.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r1.set_field_as_signed('t_tinyint', x['t_tinyint']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('t_utinyint', x['t_utinyint']) }
    assert_nothing_raised { @r1.set_field_as_signed('t_smallint', x['t_smallint']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('t_usmallint', x['t_usmallint']) }
    assert_nothing_raised { @r1.set_field_as_signed('t_integer', x['t_integer']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('t_uinteger', x['t_uinteger']) }
    # assert_nothing_raised { @r1.set_field_as_money('t_money', x['t_uinteger']) }
    assert_nothing_raised { @r1.set_field_as_date('t_date', x['t_date']) }
    # assert_nothing_raised {  t_time:  }
    # assert_nothing_raised { @r1.set_field_as_float('t_float', x['t_float']) }
    # assert_nothing_raised {  t_double: }
    # assert_nothing_raised {  t_timestamp: }
    # assert_nothing_raised {  t_efloat: }
    # assert_nothing_raised {  t_binary: }
    assert_nothing_raised { @r1.set_field_as_string('t_chars', x['t_chars']) }
    assert_nothing_raised { @r1.set_field_as_string('t_fpstring', x['t_fpstring']) }
    assert_nothing_raised { @r1.set_field_as_string('t_f2string', x['t_f2string']) }
    assert_nothing_raised { @r1.set_field_as_string('t_f4string', x['t_f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r1.set_field_as_string('t_pstring', x['t_pstring']) }
    # assert_nothing_raised { @r1.set_field_as_blob('t_varbinary', x['t_varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r1.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r1.write! }

    x = fixtures[2]
    assert_nothing_raised { @r2 = CT::Record.new(@table).clear }
    assert_nothing_raised { @r2.set_field_as_bool('t_bool', x['t_bool']) }
    assert_nothing_raised { @r2.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r2.set_field_as_signed('t_tinyint', x['t_tinyint']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('t_utinyint', x['t_utinyint']) }
    assert_nothing_raised { @r2.set_field_as_signed('t_smallint', x['t_smallint']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('t_usmallint', x['t_usmallint']) }
    assert_nothing_raised { @r2.set_field_as_signed('t_integer', x['t_integer']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('t_uinteger', x['t_uinteger']) }
    # assert_nothing_raised { @r2.set_field_as_money('t_money', x['t_uinteger']) }
    assert_nothing_raised { @r2.set_field_as_date('t_date', x['t_date']) }
    # assert_nothing_raised {  t_time:  }
    # assert_nothing_raised { @r2.set_field_as_float('t_float', x['t_float']) }
    # assert_nothing_raised {  t_double: }
    # assert_nothing_raised {  t_timestamp: }
    # assert_nothing_raised {  t_efloat: }
    # assert_nothing_raised {  t_binary: }
    assert_nothing_raised { @r2.set_field_as_string('t_chars', x['t_chars']) }
    assert_nothing_raised { @r2.set_field_as_string('t_fpstring', x['t_fpstring']) }
    assert_nothing_raised { @r2.set_field_as_string('t_f2string', x['t_f2string']) }
    assert_nothing_raised { @r2.set_field_as_string('t_f4string', x['t_f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r2.set_field_as_string('t_pstring', x['t_pstring']) }
    # assert_nothing_raised { @r2.set_field_as_blob('t_varbinary', x['t_varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r2.set_field_as_string('t_varchar', x['t_varchar']) }
    assert_nothing_raised { @r2.write! }

    assert_equal(3, @r2.count)
  end

  def test_find_eq
    x = fixtures[1]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }

    assert_nothing_raised { @rec.set_field_as_unsigned('t_uinteger', x['t_uinteger']) }
    assert_nothing_raised { @rec.find(CT::FIND_EQ) }
    assert_equal(x['t_uinteger'], @rec.get_field('t_uinteger'))
    assert_equal(x['t_varchar'],  @rec.get_field('t_varchar'))
    
  end

  def test_find_first
    x = fixtures[0]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }
    assert_nothing_raised { @index = @rec.default_index }
    assert_instance_of(CT::Index, @index)
    assert_equal(_c[:index_name], @index.name)
    assert_nothing_raised { @rec.first }

    assert_equal(x['t_bool'],      @rec.get_field_as_bool('t_bool'))
    assert_equal(x['t_tinyint'],   @rec.get_field_as_signed('t_tinyint'))
    assert_equal(x['t_utinyint'],  @rec.get_field_as_unsigned('t_utinyint'))
    assert_equal(x['t_smallint'],  @rec.get_field_as_signed('t_smallint'))
    assert_equal(x['t_usmallint'], @rec.get_field_as_unsigned('t_usmallint'))
    assert_equal(x['t_integer'],   @rec.get_field_as_signed('t_integer'))
    assert_equal(x['t_uinteger'],  @rec.get_field_as_unsigned('t_uinteger'))
    # t_money: "1,000.00"
    #assert_equal(x['t_date'],      @rec.get_field_as_date('t_date'))
    # t_time: 
    # t_float: 2.75
    # t_double:
    # t_timestamp:
    # t_efloat:
    # t_binary:
    assert_equal(pad(x['t_chars'], 31), @rec.get_field_as_string('t_chars'))
    # assert_equal(x['t_fpstring'],  @rec.get_field_as_string('t_fpstring'))
    # assert_equal(x['t_f2string'],  @rec.get_field_as_string('t_f2string'))
    # assert_equal(x['t_f4string'],  @rec.get_field_as_string('t_f4string'))
    # t_bigint: 2652295
    # t_number: 12345
    # assert_equal(x['t_number'], @rec.get_field_as_string)
    # t_currency:
    # t_pstring: " "
    # t_varbinary:
    # t_lvb:
    assert_equal(x['t_varchar'], @rec.get_field_as_string('t_varchar'))
  end

  def test_find_last
    x = fixtures[2]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }

    assert_nothing_raised { @rec.last }
    assert_equal(x['t_bool'],      @rec.get_field_as_bool('t_bool'))
    assert_equal(x['t_tinyint'],   @rec.get_field_as_signed('t_tinyint'))
    assert_equal(x['t_utinyint'],  @rec.get_field_as_unsigned('t_utinyint'))
    assert_equal(x['t_smallint'],  @rec.get_field_as_signed('t_smallint'))
    assert_equal(x['t_usmallint'], @rec.get_field_as_unsigned('t_usmallint'))
    assert_equal(x['t_integer'],   @rec.get_field_as_signed('t_integer'))
    assert_equal(x['t_uinteger'],  @rec.get_field_as_unsigned('t_uinteger'))
  end

  def test_navigation
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }
    assert_nothing_raised { @rec.first }
    assert_equal(fixtures[0]['t_uinteger'], @rec.get_field_as_unsigned('t_uinteger'))
    assert_nothing_raised { @rec.next }
    assert_equal(fixtures[1]['t_uinteger'], @rec.get_field_as_unsigned('t_uinteger'))
    assert_nothing_raised { @rec.next }
    assert_equal(fixtures[2]['t_uinteger'], @rec.get_field_as_unsigned('t_uinteger'))
    assert_nil(@rec.next) # => end of file
    assert_nothing_raised { @rec.prev }
    assert_equal(fixtures[1]['t_uinteger'], @rec.get_field_as_unsigned('t_uinteger'))
    assert_nothing_raised { @rec.prev }
    assert_equal(fixtures[0]['t_uinteger'], @rec.get_field_as_unsigned('t_uinteger'))
    assert_nil(@rec.prev) # => end of file
  end

  def test_record_set
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }
    
  end

  # def test_unique_index_error
  #   # Make sure our unique index works
  #   assert_nothing_raised { @rec.clear }
  #   assert_nothing_raised { @rec.set_field_as_bool('t_bool', x['t_bool']) }
  #   assert_nothing_raised { @rec.set_field_as_signed('t_integer', x['t_integer']) }
  #   assert_nothing_raised { @rec.set_field_as_unsigned('t_uinteger', x['t_uinteger']) }
  #   assert_nothing_raised { @rec.set_field_as_string('t_f4string', x['t_f4string']) }
  #   assert_nothing_raised { @rec.set_field_as_string('t_varchar', x['t_varchar']) }
  #   assert_raise(CT::Error) { @rec.write! }
  # end

  private

  def fixtures
    @fixtures ||= YAML.load_file(File.join(File.dirname(__FILE__), 'fixture.yml'))
  end

  def pad(val, len)
    val.ljust(len, ' ')
  end

end
