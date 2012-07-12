require 'set'

module CLJ
  class Reader

    def string(s)
      s
    end

    def character(c)
      c
    end

    def keyword(ns, name)
      if ns
        "#{ns}/#{name}".to_sym
      else
        name.to_sym
      end
    end

    def symbol(ns, name)
      if ns
        "#{ns}/#{name}".to_sym
      else
        name.to_sym
      end
    end

    def integer(i)
      i
    end

    def ratio(n, d)
      n / d
    end

    def list_new
      []
    end

    def list_add(l, x)
      l << x
    end

    def vector_new
      []
    end

    def vector_add(v, x)
      v << x
    end

    def map_new
      {}
    end

    def map_add(m, k, v)
      m[k] = v
    end

    def set_new
      Set.new
    end

    def set_add(s, x)
      s.add x
    end

  end
end
