class Hash

  def symbolize_keys!
    clone.tap do |hash|
      hash.keys.each do |key|
        self[(key.to_sym rescue key) || key] = delete(key)
      end
    end
  end

end
