module CT
  class Field
  
    def inspect
      "<CT::Field:#{object_id} @name=\"#{self.name}\" @type=\"#{self.human_type}\">"
    end

    def string?
      self.type == CT::CHARS || self.type == CT::FPSTRING || 
          self.type == CT::F2STRING || self.type == CT::F4STRING ||
          self.type == CT::PSTRING || self.type == CT::VARCHAR ||
          self.type == CT::LVB || self.type == CT::VARBINARY
    end

    def integer?
      self.type == CT::TINYINT || self.type == CT::UTINYINT || 
          self.type == CT::SMALLINT || self.type == CT::USMALLINT ||
          self.type == CT::INTEGER || self.type == CT::UINTEGER ||
          self.type == CT::BIGINT || self.type == NUMBER
    end

    def unsigned_integer?
      self.type == CT::UTINYINT || self.type == CT::USMALLINT ||
          self.type == CT::UINTEGER
    end

    def signed_integer?
      self.type == CT::TINYINT || self.type == CT::SMALLINT ||
          self.type == CT::INTEGER || self.type == CT::BIGINT
    end

    def date?
      self.type == CT::DATE
    end

    def human_type
      case self.type
      when CT::BOOL       then "CT_BOOL" 
      when CT::TINYINT    then "CT_TINYINT"
      when CT::UTINYINT   then "CT_UTINYINT"
      when CT::SMALLINT   then "CT_SMALLINT"
      when CT::USMALLINT  then "CT_USMALLINT"
      when CT::INTEGER    then "CT_INTEGER"
      when CT::UINTEGER   then "CT_UINTEGER"
      when CT::MONEY      then "CT_MONEY"
      when CT::DATE       then "CT_DATE"
      when CT::TIME       then "CT_TIME"
      when CT::FLOAT      then "CT_FLOAT"
      when CT::DOUBLE     then "CT_DOUBLE"
      when CT::TIMESTAMP  then "CT_TIMESTAMP"
      when CT::EFLOAT     then "CT_EFLOAT"
      when CT::CHARS      then "CT_CHARS"
      when CT::FPSTRING   then "CT_FPSTRING"
      when CT::F2STRING   then "CT_F2STRING"
      when CT::F4STRING   then "CT_F4STRING"
      when CT::BIGINT     then "CT_BIGINT"
      when CT::NUMBER     then "CT_NUMBER"
      when CT::CURRENCY   then "CT_CURRENCY"
      when CT::PSTRING    then "CT_PSTRING"
      when CT::VARBINARY  then "CT_VARBINARY"
      when CT::LVB        then "CT_LVB"
      when CT::VARCHAR    then "CT_VARCHAR"
      else
        "WTF?"
      end
    end

    def to_h
      { 
        name:   self.name, 
        type:   self.human_type, 
        length: self.length 
      }
    end

  end
end
