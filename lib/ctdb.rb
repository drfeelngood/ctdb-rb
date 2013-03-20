begin
  require 'ctdb_ext'
rescue LoadError
  warn "WARNING: ctdb_ext not loaded."
end

module CT
  class RecordNotFound < StandardError; end
  class UnknownAttribute < StandardError; end
  class InvalidQuery < StandardError; end
end

require 'ctdb/version'
require 'ctdb/core_ext/duplicable'
require 'ctdb/core_ext/date'
require 'ctdb/core_ext/symbol'
require 'ctdb/core_ext/hash'
require 'ctdb/error'
require 'ctdb/table'
require 'ctdb/field'
require 'ctdb/index'
require 'ctdb/segment'
require 'ctdb/record'
require 'ctdb/session_handler'
require 'ctdb/query'
require 'ctdb/model'
