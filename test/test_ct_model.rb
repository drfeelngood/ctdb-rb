require File.dirname(__FILE__) + '/test_helper'

class TestCTModel < Test::Unit::TestCase
  include TestHelper

  def setup
    CT::Model.session ||= _c
  end

  def test_save
    f  = fixtures[0]
    f1 = fixtures[1]

    assert_nothing_raised{ @model = TestModel.new(f) }

    reset_record(@model) if record_exists?(@model)

    # Confirm No record Exists
    assert_raise(CT::Error) { get_record_attribute(@model, 't_varchar') }

    # Initial call to save creates a new record.
    assert_nothing_raised{ @model.save }
    assert_equal(f['t_varchar'], get_record_attribute(@model, 't_varchar'))

    # Second call to save updates the existing record.
    @model.t_varchar = f1['t_varchar']
    assert_nothing_raised{ @model.save }
    assert_equal(f1['t_varchar'], get_record_attribute(@model, 't_varchar'))

    # Call to save with no dirty attributes
    assert_nothing_raised{ @model.save }

    reset_record(@model)
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

  def reset_record(model)
    record = CT::Record.new(model.table).clear
    index = model.table.get_index(model.primary_index[:name])
    index.segments.each do |segment|
      record.set_field(segment.field_name, model[segment.field_name])
    end

    record = record.find(CT::FIND_EQ)
    record.delete!
  end

  def record_exists?(model)
    result = false
    record = CT::Record.new(model.table).clear
    index = model.table.get_index(model.primary_index[:name])
    index.segments.each do |segment|
      record.set_field(segment.field_name, model[segment.field_name])
    end

    begin
      if record.find(CT::FIND_EQ)
        result = true
      end
    rescue
    end
    result
  end

  def get_record_attribute(model, attribute)
    record = CT::Record.new(model.table).clear
    index = model.table.get_index(model.primary_index[:name])
    index.segments.each do |segment|
      record.set_field(segment.field_name, model[segment.field_name])
    end

    record.find(CT::FIND_EQ)

    record.get_field(attribute)
  end

end
