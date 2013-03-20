module CT
  class Error

    def errno
      /[0-9]+/.match(self.message)[0].to_i
    end

  end
end
