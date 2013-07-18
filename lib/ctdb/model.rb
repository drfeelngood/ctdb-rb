require 'forwardable'

module CT
  class Model

    module Finders
      extend Forwardable

      def_delegators :query, :each, :all, :first, :last, :count

      # Helper method to quickly construt a Query object.
      # @param [Hash] options
      # @see CT::Query
      def query(options={})
        query = Query.new(table)
        options[:transformer] ||= begin
          lambda { |ct_record|
            instance = allocate
            instance.init_with(ct_record)
            instance
          }
        end
        query.merge(options)
        query
      end

      # Retrieve a given record or set of records based on the given criteria.
      # @see CT::Query
      def find(options={})
        query(options)
      end

      # Seek a record by index and index segment values
      # @param [Symbol, #to_s] index_name
      def find_by(index_name, *values)
        index_segments = {}
        index = table.get_index(index_name.to_s)
        index.segments.each_with_index do |segment, i|
          index_segments[:"#{segment.field_name}"] = values[i]
        end
        query.index(index_name).index_segments(index_segments)
      end

    end

    extend Finders

    @@session_handler = nil unless defined?(@@session_handler)

    # Define CT::Session logon configuration
    # @param [Hash] hash Configuration definition
    # @see CT::Session#new
    # @see CT::Session#logon
    # @example
    #   CT::Model.session = { username: "",  password: "", engine: "FairComs",
    #                         mode: CT::SESSION_CTREE }
    def self.session=(hash) # TODO: , scope=:default)
      hash.symbolize_keys!
      vars = [:engine, :username, :password, :mode]
      unless vars.all? { |k| hash.key?(k) }
        raise CT::Error.new("Missing required CT::Session variables. " +
                            vars.join(', '))
      end

      _session = CT::Session.new(hash[:mode])
      _session.logon(hash[:engine], hash[:username], hash[:password])
      @@session_handler = CT::SessionHandler.new(_session)
    end

    # @return [CT::SessionHandler]
    def self.session# TODO: (scope=:default)
      @@session_handler
    end

    # Set the table name
    # @param [Symbol, #to_s] value
    def self.table_name=(value)
      @table_name = value && value.to_s
    end

    # Get the table name
    # @return [String]
    def self.table_name
      @table_name
    end

    # Set the fully qualified path to the table
    # @param [String] value
    def self.table_path=(value)
      @table_path = value && value.to_s
    end

    # Get the table path
    # @return [String]
    def self.table_path
      @table_path
    end

    # @overload primary_index=(name)
    #   Define the models CT::Table default index
    #   @param [Symbol] name The index name
    # @overload primary_index=(name, increment)
    #   Define the models CT::Table default index and the index segment in which
    #     manual incrementation occurs.
    #   @param [Symbol] name The index name
    #   @param [Symbol] increment The index segment field name
    def self.primary_index=(*args)
      args = args.shift
      index_name, opts = ( args.is_a?(Array) ? args : [ args, {} ] )
      @primary_index             = {}
      @primary_index[:name]      = index_name && index_name.to_s
      @primary_index[:increment] = opts[:increment] ? opts[:increment].to_s : nil
    end

    def self.primary_index
      @primary_index ||= { name: table.default_index.name, increment: nil }
    end

    # Aquire the current sessions table handle for this model
    # @return [CT::Table]
    # @raise [CT::Error] if the CT::Model#session has not been defined
    def self.table
      if session.nil?
        raise CT::Error.new("[4003] No session handle.  You must define a " + 
                            "CT::Model#session with CT::Model.session={}")
      end
      session.open_table(table_path, table_name)
    end

    def self.create(attribs=nil)
      me = new(attribs)
      me.save
    end

    # @!group Persistence

    def self.create(attribs=nil, &block)
      if attribs.is_a?(Array)
        attribs.collect { |a| create(a, &block) }
      else
        model = new(attribs, &block)
        model.save
        model
      end
    end

    def new_record?
      @new_record
    end

    def destroyed?
      @destroyed
    end

    def persisted?
      !(new_record? || destroyed?)
    end

    def save
      result = new_record? ? create_record : update_record
      result != false
    end

    def destroy
      if persisted?
        record = Query.new(table) 
                      .index(primary_index[:name]) 
                      .index_segments(primary_index_segments)
                      .eq

        record.delete! unless record.nil?
      end
      @destroyed = true
    end

    # @!endgroup

    # @!attribute [r] attributes
    #   @return [Hash]
    attr_reader :attributes
    # @!attribute [r] new_record
    #   @return [Boolean]
    attr_reader :new_record
    # @!attribute [r] dirty_attributes
    #   @return [Hash]
    attr_reader :dirty_attributes

    # Initialize a new object with optional attributes.
    # @param [Hash] attribs
    def initialize(attribs=nil)
      @new_record = true 

      initialize_internals
      initialize_attributes
      update_attributes(attribs) unless attribs.nil?
      yield self if block_given?
    end

    def init_with(ct_record)
      initialize_internals
      initialize_attributes

      @attributes.each do |field_name, _|
        begin
          write_attribute(field_name, ct_record.get_field(field_name)) 
        rescue NotImplementedError => e
          warn(e.message)
        end
      end
      @new_record = false
    end

    # @see CT::Model.table
    def table
      self.class.table
    end

    # @see CT::Model.primary_index
    def primary_index
      self.class.primary_index
    end

    # Retrieve a collection of defined attribute names
    # @return [Array]
    def attribute_names
      @attributes.keys
    end

    # Is the given attribute defined for the model
    # @param [String. #to_s] name
    def has_attribute?(name)
      @attributes.key?(name.to_s)
    end
    alias :respond_to? :has_attribute?

    # Get the given attribures value
    # @param [String, #to_s] name
    # @raise [CT::UnknownAttributeError] if the attribute is not defined
    def read_attribute(name)
      raise CT::UnknownAttributeError.new(name) unless has_attribute?(name)
      @attributes[name.to_s]
    end
    alias :[] :read_attribute

    # Set the value of a given attribute
    # @param [String, #to_s] name
    # @param [Object] value
    # @raise [CT::UnknownAttributeError] if the attribute is not defined
    def write_attribute(name, value)
      raise CT::UnknownAttributeError.new(name) unless has_attribute?(name)
      name = name.to_s

      @attributes[name] = value #value.is_a?(String) ? value.strip : value
    end
    alias :[]= :write_attribute

    # Set a list of attributes
    # @param [Hash] attribs Field key => value
    def update_attributes(attribs = {})
      attribs.each { |name, value| write_attribute(name, value) }
    end

    # Bump the given attributes value
    # @param [String, #to_s]
    def increment(attribute, by = 1)
      attribute = attribute.to_s
      self[attribute] ||= 0
      self[attribute] += by
      self
    end

    private

      def initialize_internals
        @attributes = {}
        @dirty_attributes = {}
        @destroyed = false
      end

      def initialize_attributes
        table.get_fields.each do |field|
          @attributes[field.name] = nil
          self.class.class_eval do
            define_method(field.name) { read_attribute(field.name) }
            define_method("#{field.name}=") do |value|
              begin
                _value = read_attribute(field.name)
                _value = _value.duplicable? ? _value.clone : _value
              rescue TypeError, NoMethodError
              end
              unless @dirty_attributes.include?(field.name)
                @dirty_attributes[field.name] = _value
              end
              write_attribute(field.name, value)
            end
          end
        end
      end

      # Populate a Hash of field => value for each segment of the primary index
      # @return [Hash]
      def primary_index_segments
        table.get_index(primary_index[:name]).segments.inject({}) do |h, s|
          v = read_attribute(s.field_name)
          h[s.field_name] = v unless v.nil?
          h
        end
      end

      def create_record
        record = CT::Record.new(table).clear
        @dirty_attributes.each do |key,  _|
          begin
            record.set_field(key, @attributes[key])
          rescue NotImplementedError => e
            warn(e.message)
          end
        end

        if primary_index[:increment] && 
            ( field = table.get_field(primary_index[:increment]) ) &&
            ( record.get_field(primary_index[:increment]).nil? )

          last_record = Query.new(table)
                             .index(primary_index[:name])
                             .index_segments(primary_index_segments)
                             .last

          value = if last_record
            last_record.get_field(field.name) + 1
          elsif field.unsigned_integer?
            1
          elsif field.signed_integer?
            0
          else
            raise CT::Error.new("Unhandled primary index increment field type")
          end

          record.set_field(field.name, value)
        end

        record.write!
        @new_record = false
        return true
      end

      # * Read the record
      # * Aquire a write lock
      # * Update record fields
      # * Write record
      def update_record
        record = Query.new(table)
                      .index(primary_index[:name])
                      .index_segments(primary_index_segments)
                      .eq

        expire_at = Time.now.to_f + (0.5 * 10)
        until record.lock(CT::LOCK_WRITE)
          if expire_at > Time.now.to_f
            raise CT::LockTimeoutError.new
          end
          sleep(0.5)
          next
        end

        @dirty_attributes.each do |key, _|
          begin
            record.set_field(key, @attributes[key])
          rescue NotImplementedError => e
            warn(e.message)
          end
        end
        record.write
        record.lock(CT::LOCK_FREE)
        @new_record, @dirty_attributes = false, {}
        return true
      end

  end
end
