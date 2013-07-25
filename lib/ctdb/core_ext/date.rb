require 'date'

class Date
  
  # Convert Date to a CT::Date
  def to_ctdb
    CT::Date.new(self.year, self.month, self.day)
  end

end

class DateTime
  
  def to_ctdb
    CT::DateTime.new( self.year, self.month, self.day, 
                      self.hour, self.min, self.sec )
  end

end
