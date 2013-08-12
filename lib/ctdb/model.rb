require 'forwardable'

module CT
  class Model

    module Querying
      extend Forwardable

      def_delegators :query, :each, :all, :first, :last, :count

      # Helper method to quickly construt a Query object.
      # 
      # @param [Hash] options
      # @see CT::Query
      def query(options={})
        qry = Query.new(table)
        options[:transformer] ||= begin
          lambda { |ct_record|
            instance = allocate
            instance.init_with(ct_record)
            instance
          }
        end
        qry.merge(options)
      end

      # Retrieve a given record or set of records based on the given criteria.
      # 
      # @see CT::Query
      def find(options={})
        query(options)
      end

      # Find a +CT::Model+ by index.
      #
      # @example Find all people by then gender index.
      #   Person.find_by(:gender, "female")
      #
      # @param [Symbol, #to_s] index_name The index name
      # @param [Array] segments Index segment Index segment key values 
      #
      # @return [CT::Model, nil, CT::Query] A model of set of models
      def find_by(index_name, segments={})
        _query = query.index(index_name).index_segments(segments)
        table.get_index(index_name.to_s).allow_dups? ? _query : _query.eq
      end

    end

    extend Querying 

    @@session_handler = nil unless defined?(@@session_handler)

    # Define CT::Session logon configuration
    # 
    # @param [Hash] hash Configuration definition
    # 
    # @see CT::Session#new
    # @see CT::Session#logon
    # 
    # @example
    #   CT::Model.session = { 
    #     username: "",  
    #     password: "", 
    #     engine:   "FairComs",
    #     mode:     CT::SESSION_CTREE 
    #   }
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

    # Access the current +CT::SessionHandler+
    #
    # @return [CT::SessionHandler]
    def self.session# TODO: (scope=:default)
      @@session_handler
    end

    # Set the table name
    #
    # @example Define the table name
    #   self.table_name = :people
    #
    # @param [Symbol, #to_s] value The table name
    def self.table_name=(value)
      @table_name = value && value.to_s
    end

    # Get the table name
    #
    # @return [String]
    def self.table_name
      @table_name
    end

    # Set the fully qualified path to the table
    #
    # @example Set the table path
    #   self.table_path = "/path/to/nowhere"
    #
    # @param [String] value
    def self.table_path=(value)
      @table_path = value && value.to_s
    end

    # Get the table path
    # 
    # @return [String]
    def self.table_path
      @table_path
    end

    # Set the +Model+ primary index.  Takes an optional hash of options.
    #
    # @example Define a primary index
    #   self.primary_index = :email
    #
    # @example Define a composite primary index the contains a manually
    #   incremented field.
    #   self.primary_index = :email, { increment: :uid }
    #
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
    # 
    # @return [CT::Table]
    # 
    # @raise [CT::Error] if the CT::Model#session has not been defined
    def self.table
      if session.nil?
        raise CT::Error.new("[4003] No session handle.  You must define a " + 
                            "CT::Model#session with CT::Model.session={}")
      end
      session.open_table(table_path, table_name)
    end

    # @!group Persistence

    # Create a new record.  This will instantiate a new model object and insert
    # a record.
    #
    # @example Create a new model
    #   Person.create(title: "Mr.")
    #
    # @example Create a collection of models
    #   Person.create([ {title: "Mr."}, {title: "Mrs."}])
    #
    # @param [Hash, Array] attribs The attributes to create a new object/record
    #   or an Array of multiple attributes for multiple objects/records.
    #
    # @return [ CT::Model, Array<CT::Model>] The new record objects created
    def self.create(attribs=nil, &block)
      if attribs.is_a?(Array)
        attribs.collect { |a| create(a, &block) }
      else
        model = new(attribs, &block)
        model.save
        model
      end
    end

    # Returns true if the +CT::Model+ has not been persisted to the table, else
    # false if it has.
    #
    # @example Is the model new?
    #   person.new_record?
    #
    # @return [true, false] True if new, else false if not.
    def new_record?
      @new_record ||= false
    end

    def destroyed?
      @destroyed
    end

    # Checks to see if the model has been saved to the table.  The method 
    # returns false if the model is destroyed.
    #
    # @example Is the model persisted
    #   person.persisted?
    #
    # @return [true, false] True if persisted, false if not.
    def persisted?
      !(new_record? || destroyed?)
    end

    def dirty? 
      !@dirty_attributes.empty?
    end
    alias :changed? :dirty?

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
    # 
    # @param [Hash] attribs
    def initialize(attribs={})
      @new_record = true 
      
      initialize_internals
      initialize_attributes
      update_attributes(attribs)
      yield self if block_given?
    end

    # Initialize a new object based on the given CT::Record.
    # 
    # @param [CT::Record] ct_record
    def init_with(ct_record)
      initialize_internals
      initialize_attributes
      @attributes.keys.each do |field_name|
        write_attribute(field_name, ct_record.get_field(field_name))
      end
      @dirty_attributes = {}
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
    # 
    # @return [Array]
    def attribute_names
      @attributes.keys
    end

    def to_h
      @attributes
    end

    # Is the given attribute defined for the model
    # 
    # @param [String. #to_s] name
    def has_attribute?(name)
      @attributes.key?(name.to_s)
    end
    alias :respond_to? :has_attribute?

    # Get the given attribures value
    # 
    # @param [String, #to_s] name
    # 
    # @raise [CT::UnknownAttributeError] if the attribute is not defined
    def read_attribute(name)
      raise CT::UnknownAttribute.new(name) unless has_attribute?(name)
      @attributes[name.to_s]
    end
    alias :[] :read_attribute

    # Set the value of a given attribute
    # 
    # @param [String, #to_s] name
    # @param [Object] value
    # 
    # @raise [CT::UnknownAttributeError] if the attribute is not defined
    def write_attribute(name, value)
      raise CT::UnknownAttribute.new(name) unless has_attribute?(name)
      name = name.to_s

      unless @dirty_attributes.include?(name)
        begin
          old_value = read_attribute(name)
          old_value = old_value.duplicable? ? old_value.clone : old_value
        rescue TypeError, NoMethodError
        end
        @dirty_attributes[name] = old_value
      end

      @attributes[name] = value
    end
    alias :[]= :write_attribute

    # Set a list of attributes
    # 
    # @param [Hash] attribs Field key => value
    def update_attributes(attribs = {})
      attribs.each { |name, value| write_attribute(name, value) }
    end

    # Bump the given attributes value
    # 
    # @example Increment a numberic attribute
    #   person.increment(:age)
    #
    # @param [String, #to_s]
    def increment(attribute, by = 1)
      attribute = attribute.to_s
      self[attribute] ||= 0
      self[attribute] += by
      self
    end

    def inspect
      inspection = []
      inspection << "primary_index: \"#{primary_index[:name]}\""
      inspection << "index_segments: #{primary_index_segments}"
      inspection << "@new_record=\"#{new_record?}\""
      inspection << "@attributes=#{@attributes}"
      #inspection << "@dirty_attributes=#{@dirty_attributes}"
      "#<#{self.class.name} #{inspection * ', '}>"
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
            define_method(field.name) do
              read_attribute(field.name)
            end
            define_method("#{field.name}=") do |value|
              write_attribute(field.name, value)
            end
          end
        end
      end

      # Populate a Hash of field => value for each segment of the primary index
      # 
      # @return [Hash]
      def primary_index_segments
        table.get_index(primary_index[:name]).segments.inject({}) do |h, s|
          v = read_attribute(s.field_name)
          h[s.field_name] = v unless v.nil?
          h
        end
      end

      def create_record
        if primary_index[:increment] && 
           ( field = table.get_field(primary_index[:increment]) ) &&
           @attributes[field.name].nil?

          last_record = Query.new(table)
                             .index(primary_index[:name])
                             .index_segments(primary_index_segments)
                             .last

          @attributes[field.name] = if last_record
            last_record.get_field(field.name) + 1
          elsif field.unsigned_integer?
            1
          elsif field.signed_integer?
            0
          else
            raise CT::Error.new("Unhandled primary index increment field type")
          end
        end

        record = CT::Record.new(table).clear
        @attributes.each do |field_name, value| 
          record.set_field(field_name, value)
        end
        record.write!

        @dirty_attributes, @new_record = {}, false
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

        record.with_write_lock! do
          @dirty_attributes.each do |key, _|
            record.set_field(key, @attributes[key])
          end
          record.write!
        end

        @new_record, @dirty_attributes = false, {}
        return true
      end

  end
end
