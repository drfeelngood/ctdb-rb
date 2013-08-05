module CT
  class Record

    def inspect
      "<CT::Record:#{object_id} @default_index=\"#{self.default_index.name}\">"
    end

    def human_lock_mode
      case self.lock_mode
      when CT::LOCK_WRITE then "CTLOCK_WRITE"
      when CT::LOCK_READ  then "CTLOCK_READ"
      when CT::LOCK_FREE  then "CTLOCK_FREE"
      end
    end

    def offset=(n)
      self.seek(n)
    end

    def write_lock!
      self.lock!(CT::LOCK_WRITE) 
    end

    def write_locked?
      self.lock_mode == CT::LOCK_WRITE
    end

    def with_write_lock!(timeout=nil, sleeptime=0.5, &block)
      expire_at = ( timeout.nil? ? (::Time.now.to_f * (0.5 * 10)) : timeout )
      until self.lock(CT::LOCK_WRITE)
        if expire_at > Time.now.to_f
          raise CT::LockTimeoutError.new
        end

        sleep(sleeptime)
        next
      end
      yield
    ensure
      self.unlock 
    end

    def read_lock!
      self.lock!(CT::LOCK_READ)
    end

    def read_locked?
      self.lock_mode == CT::LOCK_READ
    end

  end
end
