module CT
  class Model
    @@session_handler = nil unless defined?(@@session_handler)

    def self.session=(hash) # TODO: , scope=:default)
      hash.symbolize_keys!
      unless [:engine, :username, :password].all? { |k| hash.key?(k) }
        raise CT::Error.new("Missing required CT::Session variablies.")
      end
      
      _session = CT::Session.new(CT::SESSION_CTREE)
      _session.logon(hash[:engine], hash[:username], hash[:password])
      @@session_handler = CT::SessionHandler.new(_session)
    end

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

    # TODO: clean the primary_index= method up.
    def self.primary_index=(*args)
      args = args.shift
      index_name, opts = ( args.is_a?(Array) ? args : [ args, {} ] )
      @primary_index             = {}
      @primary_index[:name]      = index_name && index_name.to_s
      @primary_index[:increment] = opts[:increment].to_s || nil
    end

    def self.primary_index
      @primary_index
    end

    def self.table
      raise "Uninitialized CT::Model#session" if session.nil?
      session.open_table(table_path, table_name)
    end

    # @param [Hash] options CT::Query options
    def self.find(options={})
      Query.new(self, table, options)
    end

    # @param [Hash] index_segments
    def self.where(index_segments={})
      find(index_segments: index_segments)
    end

    attr_reader :attributes

    attr_reader :dirty_attributes

    def initialize(attribs=nil)
      @attributes = {}
      @dirty_attributes = {}
      @new_record = true
      @destroyed  = false

      initialize_attributes
      update_attributes(attribs) unless attribs.nil?
      yield self if block_given?
    end

    def table
      self.class.table
    end

    def primary_index
      self.class.primary_index
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

    # @return [Array]
    def attribute_names
      @attributes.keys
    end

    # @param [String. #to_s] name
    def has_attribute?(name)
      @attributes.key?(name.to_s)
    end
    alias :respond_to? :has_attribute?

    # @param [String, #to_s] name
    def read_attribute(name)
      raise CT::UnknownAttributeError.new(name) unless has_attribute?(name)
      @attributes[name.to_s]
    end
    alias :[] :read_attribute

    # @param [String, #to_s) name
    # @param [Object] value
    def write_attribute(name, value)
      raise CT::UnknownAttributeError.new(name) unless has_attribute?(name)
      name = name.to_s

      @attributes[name] = value #value.is_a?(String) ? value.strip : value
    end
    alias :[]= :write_attribute

    # @param [Hash] attribs
    def update_attributes(attribs = {})
      attribs.each { |name, value| write_attribute(name, value) }
    end

    def increment(attribute, by = 1)
      self[attribute] ||= 0
      self[attribute] += by
      self
    end

    def record_exists?
      index = get_index
      segments = index.segments.collect{|segment| segment.field_name}
      result = false
      record = CT::Record.new(table).clear
      segments.each do |segment|
        record.set_field(segment, attributes[segment])
      end

      begin
        result = record.find(CT::FIND_EQ) != nil
      rescue
      end
      result
    end

    def save
      create_or_update unless self.dirty_attributes.empty? && self.persisted?
    end

    def create_or_update
      new_record? ? create : update
    end

    def update
      index = get_index

      index_segments = index.segments.inject({}) do |hash, segment|
          value = self.read_attribute(segment.field_name)
        hash[segment.field_name] = value unless value.nil?
        hash
      end

      query = Query.new(self.class, table)
                   .index(index.name)
                   .index_segments(index_segments)

      if query.eq
        attributes.each do |field, value|
          next if index_segments.keys.include? field # Don't change the segment since you're updating the record.
          begin
            query.record.set_field(field, value)
          rescue Object => failure
            # TODO: Resolve failures thrown in this loop.
          end
        end

        @dirty_attributes = {} if query.record.write!
      else
        raise CT::RecordNotFound.new
      end
    end

    def create
      index = get_index
      if record_exists? && !index.allow_dups?
        raise CT::Error.new("A record with this index already exists.")
      else
        record = CT::Record.new(self.table).clear

        attributes.each do |field, value|
          begin
            record.set_field(field, value)
          rescue Object => failure
          end
        end

        @new_record = false if record.write!
      end
    end

    # Reset attributes to original values.
    #def reset!
      #@dirty_attributes.each { |field, value| write_attribute(field, value) }
    #end

    private

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

      def get_index
        if self.primary_index
          table.get_index(self.primary_index[:name])
        else
          self.table.indecies.first
        end
      end

      def to_record
        record = CT::Record.new(table).clear
        @attributes.each do |k,v| record.set_field(k, v)
          puts "#{k} => #{v}"
          record.set_field(k, v)
        end
        record
      end

  end
end
