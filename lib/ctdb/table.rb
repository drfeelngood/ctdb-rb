module CT
  class Table

    def inspect
      "<CT::Table:#{object_id} @name=\"#{self.name}\" @path=\"#{self.path}\">"
    end

    def human_create_mode
      case self.create_mode
      when CT::CREATE_NORMAL    then "CTCREATE_NORMAL"
      when CT::CREATE_PREIMG    then "CTCREATE_PREIMG"
      when CT::CREATE_TRNLOG    then "CTCREATE_TRNLOG"
      when CT::CREATE_WRITETHRU then "CTCREATE_WRITETHRU"
      when CT::CREATE_CHECKLOCK then "CTCREATE_CHECKLOCK"
      when CT::CREATE_NORECBYT  then "CTCREATE_NORECBYT"
      when CT::CREATE_NOROWID   then "CTCREATE_NOROWID"
      when CT::CREATE_CHECKREAD then "CTCREATE_CHECKREAD"
      when CT::CREATE_HUGEFILE  then "CTCREATE_HUGEFILE"
      when CT::CREATE_NODELFLD  then "CTCREATE_NODELFLD"
      when CT::CREATE_NONULFLD  then "CTCREATE_NONULFLD"
      end
    end

    def to_h 
      { name: self.name,
        path: self.path,
        create_mode: self.human_create_mode,
        fields: self.fields.collect { |field| field.to_h },
        indecies: self.indecies.collect { |index| index.to_h } }
    end

  end
end
