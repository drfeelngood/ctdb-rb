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
        mode: CT::SESSION_CTREE,
        table_path: File.dirname(__FILE__),
        table_name: 'test_ctdb_rb',
        index_name: 'index_on_uinteger' }
    end
  end

  def fixtures
    @fixtures ||= YAML.load_file(File.join(File.dirname(__FILE__), 'fixture.yml'))
  end
  
  class TestModel < CT::Model
    self.table_name    = :test_ctdb_rb
    self.table_path    = File.expand_path(File.dirname(__FILE__))
    self.primary_index = :index_on_uinteger, { increment: :uinteger }
  end

end

