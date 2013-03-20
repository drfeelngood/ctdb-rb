module CT
  class Session

    def read_lock!
      self.lock!(CT::LOCK_READ)
    end

    def write_lock!
      self.lock!(CT::LOCK_WRITE)
    end

    def suspend_locks!
      self.lock!(CT::LOCK_SUSPEND)
    end

    def free_all_locks!
      self.lock!(CT::LOCK_FREE)
    end

  end
end
