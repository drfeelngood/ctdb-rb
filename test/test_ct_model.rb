require File.dirname(__FILE__) + '/test_helper'

class TestCTModel < Test::Unit::TestCase
  include TestHelper

  def setup
    CT::Model.session ||= _c
    @fixture ||= fixtures[0]
    @fixture[TestModel.primary_index[:increment]] = nil
    @model = nil 
  end

  def test_attribute_helpers
    @model = TestModel.new(@fixture)
    
    @fixture.each do |k,_|
      assert(@model.has_attribute?(k), "TestModel missing `#{k}' attribute")
    end

    @fixture.each do |k,v|
      assert_nothing_raised("#write_attribute(#{k}, #{v})") do
        @model.write_attribute(k, v)
      end
    end

    @fixture.each do |k,v|
      assert_equal(@model.read_attribute(k), v, "#read_attribute(#{k})")
    end

    @fixture.each do |k,v|
      assert_nothing_raised("#[#{k}] = #{v}") do
        @model.write_attribute(k, v)
      end
    end

    @fixture.each do |k,v|
      assert_equal(@model.read_attribute(k), v, "#[#{k}]")
    end
  end
  
  def test_new
    @model = TestModel.new
    assert_equal(false, @model.persisted?)
    assert_equal({}, @model.dirty_attributes)
    assert(@model.new_record?)
  end

  def test_save
    @model = TestModel.new(@fixture)
    assert(@model.new_record?)
    assert_equal(false, @model.persisted?)
    assert(@model.save)
    assert_equal(false, @model.new_record?)
    assert(@model.persisted?)
    assert_equal(false, @model.changed?)
    assert_nothing_raised { @model.varchar = "This is only a test" }
    assert(@model.changed?)
    assert(@model.save)
    assert_equal(false, @model.changed?)
  end

  def test_create
    assert_nothing_raised do
      @model = TestModel.create(@fixture)
    end
    assert_equal(false, @model.new_record?)
    assert(@model.dirty_attributes.empty?)
    assert(@model.persisted?)
  end

  def test_destroy
    @model = TestModel.last
    @model.destroy
    assert(@model.destroyed?)
  end

  def test_find_by
    @model = TestModel.find_by(:index_on_uinteger, {uinteger: 999})
    assert_nil(@model)
  
    @model = TestModel.find_by(:index_on_uinteger, {uinteger: 1})
    assert_not_nil(@model)
    assert_instance_of(TestModel, @model)
    assert_equal(1, @model.uinteger)
    assert(@model.persisted?)
    assert_equal(false, @model.new_record?)
    assert_equal(false, @model.dirty?)
  end

  def test_count
    assert_instance_of(Fixnum, TestModel.count)
  end
  
  def test_first
    @model = TestModel.first
    assert_not_nil(@model)
    assert_instance_of(TestModel, @model)
  end

  def test_last
    @model = TestModel.last
    assert_not_nil(@model)
    assert_instance_of(TestModel, @model)
  end

  def test_all
    assert_instance_of(Array, TestModel.all)
    TestModel.all.each_with_index do |m, n|
      assert_equal(n+1, m.uinteger)
    end
  end

  def test_each
    n = 0
    TestModel.each do |obj|
      assert_instance_of(TestModel, obj)
      assert_equal(n+=1, obj.uinteger)
    end
  end

end
