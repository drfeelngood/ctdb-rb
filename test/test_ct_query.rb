require File.dirname(__FILE__) + '/test_helper'

class TestQuery < Test::Unit::TestCase
  include TestHelper

  def setup
    @session = CT::Session.new(CT::SESSION_CTREE)
    @session.logon(_c[:engine], _c[:username], _c[:password])
    @table = CT::Table.new(@session)
    @table.path = _c[:table_path]
    @table.open(_c[:table_name], CT::OPEN_NORMAL)
  
    @query = CT::Query.new(@table)
    @record = nil
  end

  def teardown
    @table.close
    @session.logout
  end

  def test_options
    assert_nothing_raised { @query.index(:"#{_c[:index_name]}") }
    assert_equal(@query.options[:index], :"#{_c[:index_name]}")
    assert_nothing_raised { @query.index_segments(uinteger: 1) } 
    assert_equal(@query.options[:index_segments], { uinteger: 1 })
  end

  def test_find_eq
    assert_instance_of(CT::Record, primary_index_query(1).eq)
    assert_nil(primary_index_query(999).eq)
    assert_nothing_raised { primary_index_query(1).eq! }
    assert_raise(CT::RecordNotFound) { primary_index_query(999).eq! }
  end
  
  def test_find_set
    assert_instance_of(CT::Query, filter_query("foo").set) 
    assert_nil(filter_query("bar").set)
    assert_nothing_raised { filter_query("foo").set! }
    assert_raise(CT::RecordNotFound) { filter_query("bar").set! }
  end

  def test_find_gt
    assert_instance_of(CT::Record, ( @record = primary_index_query(2).gt ) )
    assert_equal(3, @record.get_field('uinteger'))
  end

  def test_find_ge
    assert_instance_of(CT::Record, ( @record = primary_index_query(2).ge ) )
    assert_equal(2, @record.get_field('uinteger'))
    assert_not_nil(@record.next)
    assert_equal(3, @record.get_field('uinteger'))
  end

  def test_find_lt
    assert_instance_of(CT::Record, ( @record = primary_index_query(2).lt ) )
    assert_equal(1, @record.get_field('uinteger'))
  end

  def test_find_le
    assert_instance_of(CT::Record, ( @record = primary_index_query(2).le ) )
    assert_equal(2, @record.get_field('uinteger'))
    assert_not_nil(@record.prev)
    assert_equal(1, @record.get_field('uinteger'))
  end

  def test_find_first
    assert_nothing_raised do
      @record = @query.first
    end
    assert_instance_of(CT::Record, @record)
    assert_equal(1, @record.get_field('uinteger'))
    assert_nil(@record.prev)
  end

  def test_find_last
    assert_nothing_raised do
      @record = @query.last
    end
    assert_instance_of(CT::Record, @record)
    assert_equal(3, @record.get_field('uinteger'))
    assert_nil(@record.next)
  end

  def test_each
    n = 0
    @query.each do |record|
      assert_instance_of(CT::Record, record)
      assert_equal(n+=1, record.get_field('uinteger'))
    end
  end

  def test_each_on_empty_set
    assert_nothing_raised do
      @record = @query.filter(%Q[fpstring == 'X']).set
    end
    assert_nil(@record)
  end

  def test_all
    collection = @query.all
    
    assert_instance_of(Array, collection)
    #collection.each do |record|
      #puts record.get_field("uinteger")
    #end
  end

  private

    def primary_index_query(uinteger)
      @query.index(:"#{_c[:index_name]}").index_segments(uinteger: uinteger)
    end

    def filter_query(chars)
      @query.filter(%Q[strncmp(chars, "#{chars}", 3) == 0])
    end

end
