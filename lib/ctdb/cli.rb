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

    private

      def logon(server, username, password)
        @session = CT::Session.new(CT::SESSION_CTREE)
        @session.logon(server, username, password)
      end

      def logout
        @session.logout
      end

  end
end
