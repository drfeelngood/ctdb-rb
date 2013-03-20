require 'rake/extensiontask'
#require 'rake/testtask'
require 'yard'

$:.unshift(File.dirname(__FILE__) + '/lib')
require 'ctdb'

Rake::ExtensionTask.new('ctdb_ext')

desc "Start the guard process."
task :guard do
  `bundle exec guard start`
end

desc "Build the RubyGem."
task :gem do
  `rm ctdb-*.gem`
  `gem build ctdb.gemspec`
end

desc "Tag the project with the current version."
task :tag do
  `git tag v#{CT::Version}`
end

YARD::Rake::YardocTask.new do |t|
  t.files = ['lib/**/*.rb', 'ext/**/*.c']
end
