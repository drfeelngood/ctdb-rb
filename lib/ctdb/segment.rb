module CT
  class Segment

    def inspect
      "CT::Segment:#{object_id} @number=%d, @mode=\"%s\", @field_name=\"%s\" >" %
          [ self.number, self.human_mode, self.field_name ]
    end

    def auto_increment?
      self.mode == CT::SEG_SCHSRL || self.mode == CT::SEG_SRLSEG
    end

    def padded?
      self.mode == CT::SEG_UVSCHSEG || self.mode == CT::SEG_VSCHSEG
    end

    def uppercase?
      self.mode == CT::SEG_USCHSEG || self.mode == CT::SEG_UVSCHSEG ||
        self.mode == CT::SEG_UVARSEG || self.mode == CT::SEG_UREGSEG
    end

    def human_mode
      case self.mode
      when CT::SEG_SCHSEG     then "CTSEG_SCHSEG"
      when CT::SET_USCHSEG    then "CTSEG_USCHSEG"
      when CT::SEG_VSCHSEG    then "CTSEG_VSCHSEG"
      when CT::SEG_UVSCHSEG   then "CTSEG_UVSCHSEG"
      when CT::SEG_SCHSRL     then "CTSEG_SCHSRL"
      when CT::SEG_DESCENDING then "CTSEG_DESCENDING"
      when CT::SEG_ALTSEG     then "CTSEG_ALTSEG"
      when CT::SEG_ENDSEG     then "CTSEG_ENDSEG"
      when CT::SEG_REGSEG     then "CTSEG_REGSEG"
      when CT::SEG_INTSEG     then "CTSEG_INTSEG"
      when CT::SEG_UREGSEG    then "CTSEG_UREGSEG"
      when CT::SEG_SRLSEG     then "CTSEG_SRLSEG"
      when CT::SEG_VARSEG     then "CTSEG_VARSEG"
      when CT::SEG_UVARSEG    then "CTSEG_UVARSEG"
      when CT::SEG_SGNSEG     then "CTSEG_SGNSEG"
      when CT::SEG_FLTSEG     then "CTSEG_FLTSEG"
      when CT::SEG_DECSEG     then "CTSEG_DECSEG"
      when CT::SEG_BCDSEG     then "CTSEG_BCDSEG"
      when CT::SEG_DESCENDING then "CTSEG_DESCENDING"
      when CT::SEG_ALTSEG     then "CTSEG_ALTSEG"
      else
        "WTF? #{self.mode}"
      end
    end

    def to_h
      { number:     self.number,
        field_name: self.field_name,
        mode:       self.human_mode,
        absolute_byte_offset: self.absolute_byte_offset? }
    end

  end
end
