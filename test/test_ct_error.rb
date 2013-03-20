require File.join(File.dirname(__FILE__), 'test_helper')

class TestCTError < Test::Unit::TestCase

  def test_errno
    error = CT::Error.new("[12] Something failed.")
    assert_equal(12, error.errno)
  end

end
