module CT
  class Record
  
    def inspect
      "<CT::Record:#{object_id} @default_index=\"#{self.default_index.name}\">"
    end

    def offset=(n)
      self.seek(n)
    end

    def write_lock!
      self.lock!(CT::LOCK_WRITE) 
    end

    def write_locked?
      self.locked? && ( self.lock == CT::LOCK_READ || 
                        self.lock == CT::LOCK_READ_BLOCK ) 
    end

    def read_lock!
      self.lock!(CT::LOCK_READ)
    end

    def read_locked?
      self.locked? && ( self.lock == CT::LOCK_WRITE ||
                        self.lock == CT::LOCK_WRITE_BLOCK )
    end

  end
end
