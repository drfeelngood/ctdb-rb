require File.dirname(__FILE__) + '/test_helper'
require 'date'

class TestCTDate < Test::Unit::TestCase

  def setup
    @month, @day, @year = 1, 10, 1984
  end

  def test_new
    assert_nothing_raised do
      @date = CT::Date.new(@year, @month, @day)
    end
    assert_instance_of(CT::Date, @date)
    assert_equal(@month, @date.month)
    assert_equal(@day,   @date.day)
    assert_equal(@year,  @date.year)
  end

  def test_today
    rb_date = Date.today
    assert_nothing_raised do
      @date = CT::Date.today
    end
    assert_instance_of(CT::Date, @date)
    assert_equal(rb_date.month, @date.month)
    assert_equal(rb_date.day,   @date.day)
    assert_equal(rb_date.year,  @date.year)
  end

  def test_to_date
    rb_date = Date.today
    assert_nothing_raised do
      @date = CT::Date.today
    end
    assert_instance_of(CT::Date, @date)
    assert_nothing_raised do
      @date = @date.to_date
    end
    assert_instance_of(Date, @date)
    assert_equal(rb_date, @date)
  end

  def test_from_date
    assert_nothing_raised do
      @date = Date.today
    end
    assert_nothing_raised do
      @date = @date.to_ctdb
    end
    assert_instance_of(CT::Date, @date)
  end

  def test_to_s
    assert_nothing_raised do

    end
  end
end
