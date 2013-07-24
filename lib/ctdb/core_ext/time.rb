class Time

  def to_ctdb
    CT::Time.new(self.hour, self.min, self.sec)
  end

end
