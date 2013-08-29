begin
  require 'ctdb_ext'
rescue LoadError
  warn "WARNING: ctdb_ext not loaded."
end

module CT
  class RecordNotFound < StandardError; end
  class RecordNotUnique < StandardError; end
  class UnknownAttribute < StandardError; end
  class InvalidQuery < StandardError; end
end

require 'ctdb/version'
require 'ctdb/core_ext/duplicable'
require 'ctdb/core_ext/date'
require 'ctdb/core_ext/time'
#require 'ctdb/core_ext/symbol'
require 'ctdb/core_ext/hash'
require 'ctdb/date'
require 'ctdb/time'
require 'ctdb/date_time'
require 'ctdb/error'
require 'ctdb/session'
require 'ctdb/table'
require 'ctdb/field'
require 'ctdb/index'
require 'ctdb/segment'
require 'ctdb/record'
require 'ctdb/session_handler'
require 'ctdb/query'
require 'ctdb/model'
