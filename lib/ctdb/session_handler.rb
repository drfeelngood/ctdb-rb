module CT
  class SessionHandler
    
    # !@attribute [r] session
    # @return [CT::Session] The active session
    attr_reader :session
    # !@attribute [r] tables
    # @return [Hash] Collection of CT::Table objects for the given session.
    attr_reader :tables

    def initialize(session)
      @session = session
      @tables  = {} 
    end

    # Retrieve or create a CT::Table resource handle
    # @param [String] path Dirname of absolute table path
    # @param [Symbol, #to_s] name Table name
    # @return [CT::Table]
    def open_table(path, name)
      name = name.to_s
      @tables[File.join(path, name)] ||= begin
        table = CT::Table.new(@session)
        table.path = path
        table.open(name, CT::OPEN_NORMAL)
      end
    end

    # @return [Array] List of all open tables
    def open_tables
      @tables.keys
    end

  end
end
