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
    assert_nothing_raised { @r0.set_field_as_bool('bool', x['bool']) }
    assert_nothing_raised { @r0.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r0.set_field_as_signed('tinyint', x['tinyint']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('utinyint', x['utinyint']) }
    assert_nothing_raised { @r0.set_field_as_signed('smallint', x['smallint']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('usmallint', x['usmallint']) }
    assert_nothing_raised { @r0.set_field_as_signed('integer', x['integer']) }
    assert_nothing_raised { @r0.set_field_as_unsigned('uinteger', x['uinteger']) }
    # assert_nothing_raised { @r0.set_field_as_money('t_money', x['t_uinteger']) }
    assert_nothing_raised { @r0.set_field_as_date('date', x['date']) }
    assert_nothing_raised { @r0.set_field_as_time('time', x['time']) }
    # assert_nothing_raised { @r0.set_field_as_float('t_float', x['t_float']) }
    # assert_nothing_raised {  t_double: }
    # assert_nothing_raised {  t_timestamp: }
    # assert_nothing_raised {  t_efloat: }
    # assert_nothing_raised {  t_binary: }
    assert_nothing_raised { @r0.set_field_as_string('chars', x['chars']) }
    assert_nothing_raised { @r0.set_field_as_string('fpstring', x['fpstring']) }
    assert_nothing_raised { @r0.set_field_as_string('f2string', x['f2string']) }
    assert_nothing_raised { @r0.set_field_as_string('f4string', x['f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r0.set_field_as_string('pstring', x['pstring']) }
    # assert_nothing_raised { @r0.set_field_as_blob('varbinary', x['t_varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r0.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r0.write! }

    x = fixtures[1]
    assert_nothing_raised { @r1 = CT::Record.new(@table).clear }
    assert_nothing_raised { @r1.set_field_as_bool('bool', x['bool']) }
    assert_nothing_raised { @r1.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r1.set_field_as_signed('tinyint', x['tinyint']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('utinyint', x['utinyint']) }
    assert_nothing_raised { @r1.set_field_as_signed('smallint', x['smallint']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('usmallint', x['usmallint']) }
    assert_nothing_raised { @r1.set_field_as_signed('integer', x['integer']) }
    assert_nothing_raised { @r1.set_field_as_unsigned('uinteger', x['uinteger']) }
    # assert_nothing_raised { @r1.set_field_as_money('money', x['uinteger']) }
    assert_nothing_raised { @r1.set_field_as_date('date', x['date']) }
    # assert_nothing_raised {  t_time:  }
    # assert_nothing_raised { @r1.set_field_as_float('t_float', x['t_float']) }
    # assert_nothing_raised {  t_double: }
    # assert_nothing_raised {  t_timestamp: }
    # assert_nothing_raised {  t_efloat: }
    # assert_nothing_raised {  t_binary: }
    assert_nothing_raised { @r1.set_field_as_string('chars', x['chars']) }
    assert_nothing_raised { @r1.set_field_as_string('fpstring', x['fpstring']) }
    assert_nothing_raised { @r1.set_field_as_string('f2string', x['f2string']) }
    assert_nothing_raised { @r1.set_field_as_string('f4string', x['f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r1.set_field_as_string('pstring', x['pstring']) }
    # assert_nothing_raised { @r1.set_field_as_blob('varbinary', x['varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r1.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r1.write! }

    x = fixtures[2]
    assert_nothing_raised { @r2 = CT::Record.new(@table).clear }
    assert_nothing_raised { @r2.set_field_as_bool('bool', x['bool']) }
    assert_nothing_raised { @r2.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r2.set_field_as_signed('tinyint', x['tinyint']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('utinyint', x['utinyint']) }
    assert_nothing_raised { @r2.set_field_as_signed('smallint', x['smallint']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('usmallint', x['usmallint']) }
    assert_nothing_raised { @r2.set_field_as_signed('integer', x['integer']) }
    assert_nothing_raised { @r2.set_field_as_unsigned('uinteger', x['uinteger']) }
    # assert_nothing_raised { @r2.set_field_as_money('money', x['uinteger']) }
    assert_nothing_raised { @r2.set_field_as_date('date', x['date']) }
    # assert_nothing_raised {  time:  }
    # assert_nothing_raised { @r2.set_field_as_float('float', x['float']) }
    # assert_nothing_raised {  double: }
    # assert_nothing_raised {  timestamp: }
    # assert_nothing_raised {  efloat: }
    # assert_nothing_raised {  binary: }
    assert_nothing_raised { @r2.set_field_as_string('chars', x['chars']) }
    assert_nothing_raised { @r2.set_field_as_string('fpstring', x['fpstring']) }
    assert_nothing_raised { @r2.set_field_as_string('f2string', x['f2string']) }
    assert_nothing_raised { @r2.set_field_as_string('f4string', x['f4string']) }
    # assert_nothing_raised {  t_bigint: 2652295 }
    # assert_nothing_raised {  t_number: 12345 }
    # assert_nothing_raised {  t_currency: }
    assert_nothing_raised { @r2.set_field_as_string('pstring', x['pstring']) }
    # assert_nothing_raised { @r2.set_field_as_blob('varbinary', x['varbinary']) }
    # assert_nothing_raised {  t_lvb: }
    assert_nothing_raised { @r2.set_field_as_string('varchar', x['varchar']) }
    assert_nothing_raised { @r2.write! }

    assert_equal(3, @r2.count)
  end

  def test_find_eq
    x = fixtures[1]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }

    assert_nothing_raised { @rec.set_field_as_unsigned('uinteger', x['uinteger']) }
    assert_nothing_raised { @rec.find(CT::FIND_EQ) }
    assert_equal(x['uinteger'], @rec.get_field('uinteger'))
    assert_equal(x['varchar'],  @rec.get_field('varchar'))
    
  end

  def test_find_first
    x = fixtures[0]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }
    assert_nothing_raised { @index = @rec.default_index }
    assert_instance_of(CT::Index, @index)
    assert_equal(_c[:index_name], @index.name)
    assert_nothing_raised { @rec.first }

    assert_equal(x['bool'],      @rec.get_field_as_bool('bool'))
    assert_equal(x['tinyint'],   @rec.get_field_as_signed('tinyint'))
    assert_equal(x['utinyint'],  @rec.get_field_as_unsigned('utinyint'))
    assert_equal(x['smallint'],  @rec.get_field_as_signed('smallint'))
    assert_equal(x['usmallint'], @rec.get_field_as_unsigned('usmallint'))
    assert_equal(x['integer'],   @rec.get_field_as_signed('integer'))
    assert_equal(x['uinteger'],  @rec.get_field_as_unsigned('uinteger'))
    # t_money: "1,000.00"
    #assert_equal(x['date'],      @rec.get_field_as_date('date'))
    # time: 
    # float: 2.75
    # double:
    # timestamp:
    # efloat:
    # binary:
    assert_equal(pad(x['chars'], 31), @rec.get_field_as_string('chars'))
    # assert_equal(x['fpstring'],  @rec.get_field_as_string('fpstring'))
    # assert_equal(x['f2string'],  @rec.get_field_as_string('f2string'))
    # assert_equal(x['f4string'],  @rec.get_field_as_string('f4string'))
    # bigint: 2652295
    # number: 12345
    # assert_equal(x['number'], @rec.get_field_as_string)
    # currency:
    # pstring: " "
    # varbinary:
    # lvb:
    assert_equal(x['varchar'], @rec.get_field_as_string('varchar'))
  end

  def test_find_last
    x = fixtures[2]
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }

    assert_nothing_raised { @rec.last }
    assert_equal(x['bool'],      @rec.get_field_as_bool('bool'))
    assert_equal(x['tinyint'],   @rec.get_field_as_signed('tinyint'))
    assert_equal(x['utinyint'],  @rec.get_field_as_unsigned('utinyint'))
    assert_equal(x['smallint'],  @rec.get_field_as_signed('smallint'))
    assert_equal(x['usmallint'], @rec.get_field_as_unsigned('usmallint'))
    assert_equal(x['integer'],   @rec.get_field_as_signed('integer'))
    assert_equal(x['uinteger'],  @rec.get_field_as_unsigned('uinteger'))
  end

  def test_navigation
    assert_nothing_raised { @rec = CT::Record.new(@table) }
    assert_nothing_raised { @rec.clear }
    assert_nothing_raised { @rec.first }
    assert_equal(fixtures[0]['uinteger'], @rec.get_field_as_unsigned('uinteger'))
    assert_nothing_raised { @rec.next }
    assert_equal(fixtures[1]['uinteger'], @rec.get_field_as_unsigned('uinteger'))
    assert_nothing_raised { @rec.next }
    assert_equal(fixtures[2]['uinteger'], @rec.get_field_as_unsigned('uinteger'))
    assert_nil(@rec.next) # => end of file
    assert_nothing_raised { @rec.prev }
    assert_equal(fixtures[1]['uinteger'], @rec.get_field_as_unsigned('uinteger'))
    assert_nothing_raised { @rec.prev }
    assert_equal(fixtures[0]['uinteger'], @rec.get_field_as_unsigned('uinteger'))
    assert_nil(@rec.prev) # => end of file
  end

  #def test_record_set
    #assert_nothing_raised { @rec = CT::Record.new(@table) }
    #assert_nothing_raised { @rec.clear }
  #end

  private

    def pad(val, len)
      val.ljust(len, ' ')
    end

end
