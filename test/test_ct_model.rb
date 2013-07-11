require File.dirname(__FILE__) + '/test_helper'

class TestCTModel < Test::Unit::TestCase
  include TestHelper

  def setup
    CT::Model.session ||= _c
    @fixture ||= fixtures[0]
    @model = TestModel.new(@fixture)
  end

  def test_attribute_methods
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

  def test_dirty_attributes
    assert(@model.dirty_attributes.empty?)
    assert_nothing_raised do
      @model.chars = "xxx"
    end
    assert(@model.dirty_attributes.size == 1)
    assert(@model.dirty_attributes.key?("chars"))
    assert_nothing_raised do
      @model.chars = "aaa"
    end
    assert_equal(@model.dirty_attributes["chars"], @fixture["chars"])
  end

  def test_count
    assert_instance_of(Fixnum, TestModel.count)
  end

  def test_create
    @model = TestModel.new
    assert_equal(false, @model.persisted?)
    assert(@model.save)
    assert(@model.persisted?)
  end

  #def test_all
    #assert_instance_of(Array, TestModel.all)
    #TestModel.all.each_with_index do |m, n|
      #assert_equal(n+1, m.uinteger)
    #end
  #end

  def test_save
    @model = TestModel.last
    @model.varchar = "Hello World" 
    assert(@model.save)
    @model = TestModel.last
    assert_equal("Hello World", @model.varchar)
  end

  def test_destroy
    @model = TestModel.last
    @model.destroy
    assert(@model.destroyed?)
  end

end
