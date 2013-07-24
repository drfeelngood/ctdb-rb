require 'thor'
require 'yaml'

module CT
  class CLI < Thor
    method_option :server,   :type => :string, :default => "FAIRCOMS"
    method_option :username, :type => :string, :default => ""
    method_option :password, :type => :string, :default => ""

    desc "schema", "Print a table's schema definition"
    def schema(table_path)
      logon(options['server'], options['username'], options['password'])
      
      table      = CT::Table.new(@session)
      table.path = File.dirname(table_path)
      table.open(File.basename(table_path), CT::OPEN_READONLY)

      puts "There are %d fields in %s/%s" % 
          [ table.field_count, table.path, table.name ]
      puts 
      puts "      Field Name          Field Type  Len"
      puts "     ----------------     ----------  ---"
      table.fields.each do |field|
        puts "%3d) %-20s %-10s %4d" % 
            [ field.number, field.name, field.human_type, field.length ]
      end
      puts
      puts " There are %d index(s)" % table.indecies.count
      puts
      table.indecies.each do |index|
        puts " Index Name - %s" % index.name
        puts index.allow_dups? ? " Dups Allowed" : " No Dups Allowed"
        index.segments.each_with_index do |segment, i|
          if i == 0
            puts segment.absolute_byte_offset? ? " Field Offset" : " Byte Offset"
            puts
            puts "       Seg Field Name        Len  Seg Mode"
            puts "       ------------------    ---  ---------------"
          end
          length = segment.field.length
          length -= 1 if segment.absolute_byte_offset?
          puts "       %-20s %4d  %s" % 
              [ segment.field_name, segment.field.length, segment.human_mode ]
        end
        puts
      end
      
      table.close
    ensure
      logout
    end

    method_option :server,   :type => :string, :default => "FAIRCOMS"
    method_option :username, :type => :string, :default => ""
    method_option :password, :type => :string, :default => ""

    desc "dump_schema", "Print a table's schema definition to a yaml file"
    def dump_schema(table_path, file_path = "")
      logon(options['server'], options['username'], options['password'])

      table      = CT::Table.new(@session)
      table.path = File.dirname(table_path)
      table.open(File.basename(table_path), CT::OPEN_READONLY)

      table_name = File.basename(table_path)
      schema_path = (file_path.empty? ? table_path : file_path + table_name) + '.yml'

      request_file_overwrite_permission(schema_path) if File.exist?(schema_path)

      schema_file = File.open(schema_path, 'w')

      schema_file.puts "name: #{table.name}"
      schema_file.puts "path: '#{table.path}'"

      schema_file.puts "fields:"
      table.fields.each do |field|
        schema_file.puts "  - name: #{field.name}"
        schema_file.puts "    type: '#{field.human_type.sub(/_/, '::')}'"
        schema_file.puts "    length: #{field.length}"
      end

      schema_file.puts "indexes:"
      table.indecies.each do |index|
        schema_file.puts "  - name: #{index.name}"
        schema_file.puts "    allow_dups: '#{index.allow_dups?}'"
        schema_file.puts "    segments:"
        index.segments.each do |segment|
          length = segment.field.length
          length -= 1 if segment.absolute_byte_offset?

          schema_file.puts "      - field_name: #{segment.field_name}"
          schema_file.puts "        length: #{segment.field.length}"
          schema_file.puts "        mode: #{segment.human_mode}"
        end
      end

      table.close
    ensure
      logout
    end

    private

      def logon(server, username, password)
        @session = CT::Session.new(CT::SESSION_CTREE)
        @session.logon(server, username, password)
      end

      def logout
        @session.logout
      end

      def request_file_overwrite_permission(file_path)
        puts "A file already exists at #{file_path}."
        permission =  ask "Are you sure you want to overwrite it? y/n"
        if permission != 'y'
          exit
        end
      end
  end
end
