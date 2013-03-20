class Symbol
  
  def lt
    CT::Query::SymbolOperator.new(self, :lt)
  end

  def lte
    CT::Query::SymbolOperator.new(self, :lte)
  end

  def gt
    CT::Query::SymbolOperator.new(self, :gt)
  end

  def gte
    CT::Query::SymbolOperator.new(self, :gte)
  end

  def in
    CT::Query::SymbolOperator.new(self, :in)
  end

  def ne
    CT::Query::SymbolOperator.new(self, :ne)
  end

end
