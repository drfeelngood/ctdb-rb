module CT
  class Date
  
    def inspect
      "<CT::Date:#{object_id} @type=\"#{human_type}, \" @value=\"#{to_s}\" >"
    end

    def human_type
      case self.type
      when CT::DATE_MDCY then "CTDATE_MDCY"
      when CT::DATE_MDY  then "CTDATE_MDY"
      when CT::DATE_DMCY then "CTDATE_DMCY"
      when CT::DATE_DMY  then "CTDATE_DMY"
      when CT::DATE_CYMD then "CTDATE_CYMD"
      when CT::DATE_YMD  then "CTDATE_YMD"
      else
        "WTF?"
      end
    end
      
  end
end
