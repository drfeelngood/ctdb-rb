require 'forwardable'

module CT
  class Query

    class SymbolOperator 

      attr_reader :field_name
      attr_reader :operator

      # @param [Symbol]
      # @param [Symbol] operator
      def initialize(field_name, operator)
        @field_name = field_name && field_name.to_sym
        @operator   = operator && operator.to_sym
      end
    
    end

    class Filter
      attr_reader :expression 

      # @param [String, Array, Hash] expression
      def initialize(expression)
        unless [String, Array, Hash].include?(expression.class)
          raise "Unhandled CT::Query::Filter#expression"
        end
        @expression = expression
      end

      def to_s
        @expression
      end

    end
    
    #include Enumerable
    extend Forwardable

    #def_delegator :@record, :first
    #def_delegator :@record, :last
    #def_delegator :@record, :lock_

    OPTS = [ :find_mode,
             :index, 
             :index_segments,
             :limit, 
             :offset, 
             :fields, 
             :filter, 
             :endif ].freeze

    attr_reader :model
    attr_reader :table
    attr_reader :record
    attr_reader :options
    attr_accessor :init_offset

    # @param [Constant] model
    # @param [CT::Table] table 
    def initialize(model, table, options={})
      @model    = model
      @table    = table
      @record   = CT::Record.new(@table).clear
      @options  = options
    end

    def eq
      query = clone.merge(find_mode: :eq)
      query.prepare
      query.record.find(CT::FIND_EQ)
      query.init_object
    rescue 
      nil
    end

    def eq!
      eq || raise( CT::RecordNotFound.new )
    end

    def set
      query = clone.merge(find_mode: :set)
      query.prepare
      bytes = 0
      query.options[:index_segments].each do |field, value|
        segment = query.record.default_index.get_segment(field.to_s)
        bytes += segment.field.length
        bytes -= 1 if segment.absolute_byte_offset?
      end
      query.record.set_on(bytes)
      query
    end
   
    def gt
      query = clone.merge(find_mode: :gt)
      query.prepare
      query.find(CT::FIND_GT)
    end

    def ge
      query = clone.merge(find_mode: :ge)
      query.prepare
      query.find(CT::FIND_GE)
    end

    def lt
      query = clone.merge(find_mode: :lt)
      query.prepare
      query.find(CT::FIND_LT)
    end
    
    def le
      query = clone.merge(find_mode: :le)
      query.prepare
      query.find(CT::FIND_LE)
    end

    def count
      query = clone
      query.record.count
    end

    def first
      # TODO: raise of set_on? == false
      query = clone
      query.record.first
      query.init_object
    end

    def last
      # TODO: raise of set_on? == false
      query = clone
      query.record.last
      query.init_object
    end

    def each(&block)
      query = clone
      
      if query.options[:find_mode] == :set
        return query if query.record.first.nil?
      end

      begin
        yield( query.init_object ) 
      end while query.record.next
      
      query
    end

    def all(&block)
      [].tap do |objects|
        each { |object| objects << object }
      end
    end

    def index(name)
      clone.tap { |query| query.options[:index] = name }
    end

    def index_segments(criteria={})
      clone.tap { |query| query.options[:index_segments] = criteria }
    end

    # @param [Fixnum] n
    def limit(n=nil)
      clone.tap { |query| query.options[:limit] = n }
    end

    # @param [Fixnum] n
    def offset(n=nil)
      clone.tap { |query| query.options[:offset] = n }
    end

    def fields(*args)
      clone.tap { |query| query.options[:fields] = *args }
    end

    # @param [String, Hash] expression Query filter expression
    def filter(expression)
      clone.tap { |query| query.options[:filter] = Filter.new(expression) }
    end

    def endif(criteria={})
      clone.tap { |query| query.options[:endif] = criteria }
    end

    def inspect
      "#<#{self.class.name}, @model=#{@model}, @table=#{@table.name}, " +
        "@options=#{@options}>"
    end

    def merge(opts={})
      opts.each do |key, value| 
        key = key.to_sym
        unless OPTS.include?(key)
          raise InvalidQuery.new("Unknown CT::Query#option `#{key}'")
        end
        @options[key] = value
      end
      self
    end

    def prepare 
      @record.default_index = @options[:index].to_s if @options[:index]
     
      if @options[:index_segments]
        @options[:index_segments].each do |field, value|
          @record.set_field(field.to_s, value)
        end
      end

      validate!

      if @options[:filter]
        @record.filter = @options[:filter].to_s
      end
    end

    def init_object
      obj = model.new
      obj.attributes.each do |key, _|
        begin
          obj.write_attribute(key, @record.get_field(key))
        rescue Exception => e
          puts "BUG: (#{e.class}) #{e.message}"
        end
      end
      obj
    end

    private

      def validate!
         
        if @options[:index_segments] 
          # Require an index of index segments given.
          unless @options[:index] 
            raise "You must define an index if index_segments specified."
          end
          # Make sure the supplied index segments actual exist for the given
          # index.
          unless @options[:index_segments].all? { |k,_|
            !@record.default_index.get_segment(k.to_s).nil? 
          }
            raise "Index segments supplied are out of the scope of " + 
              "`#{@options[:index]}'"
          end
        end

      rescue StandardError => e
        raise InvalidQuery.new(e.message)
      end

  end
end
