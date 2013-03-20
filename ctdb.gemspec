require File.dirname(__FILE__) + '/lib/ctdb/version'

Gem::Specification.new do |s|
  s.name     = "ctdb"
  s.version  = "#{CT::Version}"
  s.summary  = "FairCom c-treeDB ruby SDK"
  s.description = "No fluff c extension implementing the ctdb sdk."
  s.homepage = "https://github.com/drfeelngood/ctdb-rb"
  s.email    = "dan@dj-agiledev.com"

  s.authors = [ "Daniel Johnston" ]

  s.files       = Dir['lib/**/*.rb'] + 
                  Dir['ext/ctdb_ext/*.{h,c,rb}']

  s.executables << 'ctdb'
  s.test_files  = Dir['test/*.rb']

  s.extensions  = [ 'ext/ctdb_ext/extconf.rb' ]


  s.add_dependency "thor"
end
