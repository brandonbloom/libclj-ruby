require 'stringio'

module CLJ

  def self.read(o)
    case o
    when String
      read_string(o)
    when IO
      read_io(o)
    else
      raise TypeError
    end
  end

  def self.read_string(s)
    read_io(StringIO.new(s))
  end

end
