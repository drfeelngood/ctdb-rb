module CT
  class DateTime

    def inspect
      "#<CT::DateTime:#{object_id} @value=\"#{to_s}\" >"
    end

    def to_s
      "#{self.date} #{self.time}"
    end

  end
end
