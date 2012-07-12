require 'bigdecimal'

module CLJ
  class Reader

    def initialize
      @builder = RootBuilder.new
    end

    def value
      throw 'Incomplete read' unless @builder.instance_of? RootBuilder
      @builder.value
    end

    protected

    def string(s)
      s
    end

    def character(s)
      s
    end

    def keyword(s, ns, name)
      if ns
        "#{ns}/#{name}".to_sym
      else
        name.to_sym
      end
    end

    def symbol(s, ns, name)
      case s
      when 'nil'
        nil
      when 'true'
        true
      when 'false'
        false
      else
        if ns
          "#{ns}/#{name}".to_sym
        else
          name.to_sym
        end
      end
    end

    def number(s, numerator, denominator, is_big, is_decimal)
      if is_decimal
        if is_big
          numerator = BigDecimal.new(numerator)
          denominator = BigDecimal.new(denominator)
        else
          numerator = numerator.to_f
          denominator = denominator.to_f
        end
      else
        # to_i auto-promotes to Bignum
        numerator = numerator.to_i
        denominator = denominator.to_i
      end
      if denominator == 1
        numerator
      else
        numerator / denominator #TODO: Ratio.new
      end
    end

    def list_builder
      ArrayBuilder
    end

    def vector_builder
      ArrayBuilder
    end

    def map_builder
      HashBuilder
    end

    def set_builder
      SetBuilder
    end

    def begin_composite(builder_klass)
      @builder = builder_klass.new(@builder)
    end

    def atomic(x)
      @builder.add x
    end

    def end_composite
      parent = @builder.parent
      parent.add @builder.value
      @builder = parent
    end

  end
end
