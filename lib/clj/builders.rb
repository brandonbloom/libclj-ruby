require 'set'

module CLJ

  class Builder

    def initialize(parent, value)
      @parent = parent
      @value = value
    end

    attr_accessor :value, :parent

  end

  class RootBuilder < Builder

    def initialize
      super(nil, nil)
      @has_value = false
    end

    def add(x)
      raise 'already got value' if @has_value
      @has_value = true
      @value = x
    end

    def value
      raise 'no value yet' unless @has_value
      @value
    end

  end

  class HashBuilder < Builder

    def initialize(parent)
      super(parent, {})
      @key = nil
      @has_key = false
    end

    def add(x)
      if @has_key
        @value[@key] = x
        @key = nil
        @has_key = false
      else
        @key = x
        @has_key = true
      end
    end

  end

  class ArrayBuilder < Builder

    def initialize(parent)
      super(parent, [])
    end

    def add(x)
      @value.push x
    end

  end

  class SetBuilder < Builder

    def initialize(parent)
      super(parent, Set.new)
    end

    def add(x)
      @value.add x
    end

  end

end
