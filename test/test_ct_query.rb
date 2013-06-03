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
    assert_nothing_raised do
      @query.index(:"#{_c[:index_name]}")
    end
    assert_equal(@query.options[:index], :"#{_c[:index_name]}")
    assert_nothing_raised do
      @query.index_segments(:uinteger => 1) 
    end
    assert_equal(@query.options[:index_segments], { :uinteger => 1 })
  end

  def test_find_eq
    @record = @query.index(:"#{_c[:index_name]}")
                    .index_segments(:uinteger => 1)
                    .eq
    assert_instance_of(CT::Record, @record)
  end

  def test_find_eq!
    assert_nothing_raised do
      @query.index(:"#{_c[:index_name]}")
            .index_segments(:uinteger => 1)
            .eq!
    end
  end

  def test_find_set
    assert_nothing_raised do
      @record = @query.set
    end
    assert_instance_of(CT::Record, @record)
  end

  def test_find_gt
    assert_nothing_raised do
      @record = @query.index(:"#{_c[:index_name]}")
                      .index_segments(:uinteger => 2)
                      .gt
    end
    assert_instance_of(CT::Record, @record)
    assert(@record.get_field('uinteger') > 2)
  end

  def test_find_ge
    assert_nothing_raised do
      @record = @query.index(:"#{_c[:index_name]}")
                      .index_segments(:uinteger => 2)
                      .ge
    end
    assert_instance_of(CT::Record, @record)
    assert_equal(2, @record.get_field('uinteger'))

    assert_not_nil(@record.next)
    assert_equal(3, @record.get_field('uinteger'))
  end

  def test_find_lt
    assert_nothing_raised do
      @record = @query.index(:"#{_c[:index_name]}")
                      .index_segments(:uinteger => 2)
                      .lt
    end
    assert_instance_of(CT::Record, @record)
    assert_equal(1, @record.get_field('uinteger'))
  end

  def test_find_le
    assert_nothing_raised do
      @record = @query.index(:"#{_c[:index_name]}")
                      .index_segments(:uinteger => 2)
                      .le
    end
    assert_instance_of(CT::Record, @record)
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
    @query.each do |record|
      assert_instance_of(CT::Record, record)
    end
  end

  #def test_all
    #collection = @query.all
    
    #assert_instance_of(Array, collection)
    #collection.each_with_index do |record, n|
      #assert_equal(n+1, record.get_field('uinteger'))
    #end
  #end

  #def test_filter
    #x = fixtures[1]

    #@record = @query.filter("strncmp(varchar, \"#{x['varchar']}\", " +
      #"#{x['varchar'].length + 1}) == 0").set

    #assert_instance_of(CT::Record, @record)
    #assert_equal(2, @record.get_field('uinteger'))
  #end

end
