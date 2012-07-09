require 'test/unit'
require 'clj'

class CLJTest < Test::Unit::TestCase

  def test_read
    assert_equal 'TODO', CLJ.read
  end

end
