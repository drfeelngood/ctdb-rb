module CT
  class Date
  
    def inspect
      "<CT::Date:#{object_id} @format=\"#{human_format} \" @value=\"#{to_s}\" >"
    end
      
  end
end
