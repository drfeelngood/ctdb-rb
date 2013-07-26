require File.expand_path('../lib/ctdb/version', __FILE__)

Gem::Specification.new do |s|
  s.name     = "ctdb"
  s.version  = "#{CT::Version}"
  s.summary  = "FairCom c-treeDB ruby SDK"
  s.description = "No fluff c extension implementing the ctdb sdk."
  s.homepage = "https://github.com/drfeelngood/ctdb-rb"
  s.email    = "dan@dj-agiledev.com"
  s.license  = "MIT"

  s.authors  = [ "Daniel Johnston" ]

  s.files       = `git ls-files README.md LICENSE ext lib`.split 
  s.test_files  = `git ls-files test`.split 
  s.extensions  = [ 'ext/ctdb_ext/extconf.rb' ]
  s.executables << 'ctdb'

  s.add_dependency "thor"
end
