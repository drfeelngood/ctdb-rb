module CT
  class Time
  
    def inspect
      "#<CT::Time:#{object_id} @type=\"#{human_type}\". @value=\"#{to_s}\" >" 
    end

    def human_type
      case self.type
      when CT::TIME_HMSP then "CTTIME_HMSP"
      when CT::TIME_HMP  then "CTTIME_HMP"
      when CT::TIME_HMS  then "CTTIME_HMS"
      when CT::TIME_HM   then "CTTIME_HM"
      when CT::TIME_MIL  then "CTTIME_MIL"
      else
        "WTF?"
      end
    end

  end
end
