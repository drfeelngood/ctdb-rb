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
    assert_equal(@month, @date.mon)
    assert_equal(@day,   @date.day)
    assert_equal(@year,  @date.year)
  end

  def test_today
    rb_date = Date.today
    assert_nothing_raised do
      @date = CT::Date.today
    end
    assert_instance_of(CT::Date, @date)
    assert_equal(rb_date.month, @date.mon)
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
    y, m, d = @date.year, @date.month, @date.day
    assert_nothing_raised do
      @date = @date.to_ctdb
    end
    assert_instance_of(CT::Date, @date)
    assert_equal(y, @date.year)
    assert_equal(m, @date.mon)
    assert_equal(d, @date.day)
  end

end

class TestCTDateTime < Test::Unit::TestCase
  
  def setup
    @month, @day, @year = 1, 10, 1984
    @hour, @min, @sec = 9, 30, 15
    @datetime = nil
  end

end

class TestCTTime < Test::Unit::TestCase

  def setup
    @hour, @min, @sec = 9, 20,15 
    @time = nil
  end

  def test_now
    assert_nothing_raised do
      @time = CT::Time.now
    end
    assert_instance_of(CT::Time, @time)
  end

  def test_unpack
    assert_nothing_raised do
      @time = CT::Time.new(@hour, @min, @sec)
    end
    assert_instance_of(CT::Time, @time)
    parts = @time.unpack
    assert_instance_of(Array, parts)
    assert_equal(3, parts.size)
    assert_equal(@hour, parts[0])
    assert_equal(@min,  parts[1])
    assert_equal(@sec,  parts[2])
  end

  def test_hour_min_and_sec
    assert_nothing_raised do
      @time = CT::Time.new(@hour, @min, @sec)
    end
    assert_instance_of(CT::Time, @time)
    assert_equal(@hour, @time.hour)
    assert_equal(@min,  @time.min)
    assert_equal(@sec,  @time.sec)
  end

  def test_to_time
    assert_nothing_raised do
      @time = CT::Time.now
    end
    assert_instance_of(CT::Time, @time)
    assert_instance_of(Time, @time.to_time)
  end

  def test_from_time
    time = Time.now
    assert_instance_of(CT::Time, time.to_ctdb)
  end

end
