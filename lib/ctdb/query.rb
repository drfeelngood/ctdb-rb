require 'forwardable'

module CT
  class Query

    class SymbolOperator

      # @!attribute [r] field_name
      #   @return [String, #to_s]
      attr_reader :field_name
      # @!attribute [r] operator
      attr_reader :operator

      # @param [Symbol]
      # @param [Symbol] operator
      def initialize(field_name, operator)
        @field_name = field_name && field_name.to_sym
        @operator   = operator && operator.to_sym
      end
    
    end

    #class Filter
      #attr_reader :expression

      ## @param [String, Array, Hash] expression
      #def initialize(expression)
        #unless [String, Array, Hash].include?(expression.class)
          #raise "Unhandled CT::Query::Filter#expression"
        #end
        #@expression = expression
      #end

      #def to_s
      #end

      #private

        #def strncmp(field_name, str)
          #"strncmp(#{field_name}, \"#{str}\", #{str.length})"
        #end
    
    #end
    
    #include Enumerable
    extend Forwardable

    def_delegator :@record, :set?, :record_set?
    def_delegator :@record, :next, :next_record

    def_delegators :@record, :clear, 
                             :find, 
                             :set_field, 
                             :write!,
                             :default_index, 
                             :default_index=, 
                             :set_on, 
                             :set_off,
                             :count
    
    OPTS = [ :find_mode,
             :index,
             :index_segments,
             :limit,
             :offset,
             :fields,
             :filter,
             :endif,
             :transformer ].freeze


    # @!attribute [r] table 
    #   @return [CT::Table] Table handle
    attr_reader :table
    # @!attribute [r] record
    #   @return [CT::Record] Record handle
    attr_reader :record 
    # @!attribute [r] options
    #   @return [Hash] Collection of CT::Record search properties 
    attr_reader :options

    # @param [CT::Table] table
    # @param [Hash] opts Optional CT::Record finder properties 
    # @option opts [String] :index The default index
    # @option opts [String] :index_segments The default index segments populated
    # @option opts [Fixnum] :limit
    # @option opts [Fixnum] :offset
    # @option opts [String] :filter
    # @option opts [Proc] :transformer
    def initialize(table, options={})
      @table   = table
      @record  = CT::Record.new(@table).clear
      @options = options
    end

    def initialize_copy(original)
      @options = original.options.dup
    end

    # @!group Finders

    # Find a record equal to the current record target
    # @return [CT::Query, nil] The query object or nil if no record is found.
    def eq
      eq!
    rescue CT::RecordNotFound
      nil
    end

    # @see CT::Query#eq
    # @raise [CT::RecordNotFound] if no matching records are found 
    def eq!
      prepare
      find(CT::FIND_EQ)
      cursor
    rescue CT::Error # TODO: Should we be specific on the errors we trap?
      raise CT::RecordNotFound.new
    end

    def set
      set!
    rescue CT::RecordNotFound
      nil
    end

    def set!
      prepare
      
      bytes = 0
      options[:index_segments].each do |field, value|
        segment = default_index.get_segment(field.to_s)
        bytes += segment.field.length
        bytes -= 1 if segment.absolute_byte_offset?
      end if options[:index_segments]
      
      set_on(bytes)
      
      @record.first.nil? ? raise( CT::RecordNotFound.new ) : self
    end
  
    # Find a record greater than the current record target
    # @return [CT::Query, nil] The query object or nil if no records found.
    def gt
      prepare
      find(CT::FIND_GT)
    
      cursor
    end

    # Find a record greater then or equal to the current record target
    # @return [CT::Query, nil] The query object or nil if no records found.
    def ge
      prepare
      find(CT::FIND_GE)
    
      cursor
    end

    # Find a record less than the current record target
    # @return [CT::Query, nil] The query object or nil if not records found.
    def lt
      prepare
      find(CT::FIND_LT)
    
      cursor
    end
    
    # Find a record less than or equal to the current record target
    # @return [CT::Query, nil] The query object or nil if not records found.
    def le(opts={})
      prepare
      find(CT::FIND_LE)
    
      cursor
    end

    def first
      prepare
      record.first

      cursor
    end

    def last
      prepare
      @record.last
    
      cursor
    end

    def each(&block)
      @record.first unless record_set? 

      begin
        yield( cursor )
      end while next_record 
      
      cursor
    end

    def all
      [].tap do |objects|
        each { |obj| objects << obj } 
      end
    end

    # @!endgroup
    
    def cursor
      if record && options[:transformer]
        options[:transformer].call(@record)
      else
        record
      end
    end

    # @!group Criteria

    def index(name)
      options[:index] = name
      self
    end

    def index_segments(criteria={})
      options[:index_segments] = criteria
      self
    end

    # @param [Fixnum] n
    def limit(n=nil)
      options[:limit] = n
      self
    end

    # @param [Fixnum] n
    def offset(n=nil)
      options[:offset] = n
      self
    end

    def fields(*args)
      options[:fields] = args
      self
    end

    # @param [String, Hash] expression Query filter expression
    def filter(expression)
      options[:filter] = expression
      self
    end

    def endif(criteria={})
      options[:endif] = criteria
      self
    end
    
    # @!endgroup

    def inspect
      "#<#{self.class.name}, @table=#{@table.name}, @options=#{@options}>"
    end

    def merge(opts={})
      opts.each do |key, value|
        key = key.to_sym
        unless OPTS.include?(key)
          raise InvalidQuery.new("Unknown CT::Query#option `#{key}'")
        end
        options[key] = value
      end
      self
    end

    def prepare
      self.default_index = options[:index].to_s if options[:index]

      if options[:index_segments]
        options[:index_segments].each do |field, value|
          set_field(field.to_s, value)
        end
      end

      validate!

      if options[:filter]
        record.filter = options[:filter].to_s
      end
    end

    private

      def validate!
        return unless @options.key?(:index_segments) && @options[:index_segments]
        
        # Require an index of index segments given.
        unless @options[:index]
          raise InvalidQuery.new("You must define an index if " +
                                 "index_segments specified.")
        end
        
        # Make sure the supplied index segments actual exist for the given
        # index.
        unless @options[:index_segments].all? { |k,_|
          !@record.default_index.get_segment(k.to_s).nil? 
        }
          raise InvalidQuery.new("Index segments supplied are out of the " +
                                 "scope of `#{@options[:index]}'")
        end
      end

  end

end
