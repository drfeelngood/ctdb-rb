require File.dirname(__FILE__) + '/test_helper'

class TestCTModel < Test::Unit::TestCase
  include TestHelper

  def setup
    CT::Model.session ||= _c
  end

  def test_attribute_methods
    assert_nothing_raised do
      @model = TestModel.new
    end
    assert_instance_of(TestModel, @model)
    
    fixtures[0].each do |k,_|
      assert(@model.has_attribute?(k), "TestModel missing `#{k}' attribute")
    end

    fixtures[0].each do |k,v|
      assert_nothing_raised("#write_attribute(#{k}, #{v})") do
        @model.write_attribute(k, v)
      end
    end

    fixtures[0].each do |k,v|
      assert_equal(@model.read_attribute(k), v, "#read_attribute(#{k})")
    end

    fixtures[0].each do |k,v|
      assert_nothing_raised("#[#{k}] = #{v}") do
        @model.write_attribute(k, v)
      end
    end

    fixtures[0].each do |k,v|
      assert_equal(@model.read_attribute(k), v, "#[#{k}]")
    end
  end

  def test_dirty_attributes
    f = fixtures[0]

    assert_nothing_raised do
      @model = TestModel.new(f)
    end
    assert(@model.dirty_attributes.empty?)
    assert_nothing_raised do
      @model.t_chars = "xxx"
    end
    assert(@model.dirty_attributes.size == 1)
    assert(@model.dirty_attributes.key?("t_chars"))
    assert_nothing_raised do
      @model.t_chars = "aaa"
    end
    assert_equal(@model.dirty_attributes["t_chars"], f["t_chars"])
  end

end
