require 'rake/extensiontask'
require 'rspec/core/rake_task'
require 'yard'

$:.unshift(File.dirname(__FILE__) + '/lib')
require 'ctdb'

Rake::ExtensionTask.new('ctdb_ext')

RSpec::Core::RakeTask.new(:spec)

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

desc "Run test suite"
task :test do
  sh("cd test && sh test_suite.sh")
end

YARD::Rake::YardocTask.new do |t|
  t.files = ['lib/**/*.rb', 'ext/**/*.c']
end

task :default => :spec
