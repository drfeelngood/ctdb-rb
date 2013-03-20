module CT
  class Date
  
    def inspect
      "<CT::Date:#{object_id} @format=\"#{human_format} \" @value=\"#{to_s}\" >"
    end

    def human_format
      "TODO"
    end

    def <=>(other) 
    end

    def ===(other)
    end

  end
end
