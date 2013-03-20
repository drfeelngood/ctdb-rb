require 'test/unit'
require 'turn'
require 'yaml'

$:.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))
require 'ctdb'

module TestHelper

  def _c
    @_c ||= begin
      { engine: "FAIRCOMS", 
        username: "", 
        password: "",
        table_path: File.dirname(__FILE__),
        table_name: 'test_ctdb_sdk',
        index_name: 't_uinteger_ndx' }
    end
  end

  def fixtures
    @fixtures ||= YAML.load_file(File.join(File.dirname(__FILE__), 'fixture.yml'))
  end
  
  class TestModel < CT::Model
    self.table_name    = :test_ctdb_sdk
    self.table_path    = File.expand_path(File.dirname(__FILE__))
    self.primary_index = :t_uinteger_ndx
  end

end

