module CT
  class Index

    # Retrieve all fields associated with the index.
    # @return [Array]
    def field_names
      self.segments.collect { |s| s.field_name }
    end

    def inspect
      "<CT::Index:#{object_id} @name=\"#{self.name}\" @field_names=#{self.field_names}>"
    end

    def auto_increment?
      self.segments.any? { |s| s.auto_increment? }
    end

    def fixed_length?
      self.key_type == CT::INDEX_FIXED || self.key_type == CT::INDEX_LEADING
    end

    def variable_length?
      self.key_type == CT::INDEX_PADDING || self.key_type == CT::INDEX_LEADPAD
    end
    
    def human_key_type
      case self.key_type
      when CT::INDEX_FIXED   then "CTINDEX_FIXED"
      when CT::INDEX_LEADING then "CTINDEX_LEADING"
      when CT::INDEX_PADDING then "CTINDEX_PADDING"
      when CT::INDEX_LEADPAD then "CTINDEX_LEADPAD"
      when CT::INDEX_ERROR   then "CTINDEX_ERROR"
      else
        raise CT::Error.new("Unable to translate key type `#{self.key_type}'")
      end
    end

    def to_h
      { name:       self.name,
        key_type:   self.human_key_type,
        allow_dups: self.allow_dups?,
        allow_null: self.allow_null?,
        key_length: self.key_length,
        segments:   self.segments.collect { |segment| segment.to_h } }
    end

  end
end
