require 'stringio'

module CLJ

  def self.reader
    Reader.new
  end

  def self.read_io(io)
    self.read_ext(reader, io)
  end

  def self.read_string(s)
    read_io(StringIO.new(s))
  end

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

end
