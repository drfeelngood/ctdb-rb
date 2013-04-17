require 'mkmf'

#def osx?
  #!!(RUBY_PLATFORM =~ /darwin/)
#end

#def linux?
  #!!(RUBY_PLATFORM =~ /linux/)
#end

if enable_config("debug")
  $defs.push("-DDEBUG")
end

errors = []
errors << "'ctdbsdk.h'" unless find_header('ctdbsdk.h')
errors << "'mtclient'"  unless find_library('mtclient', 'ctdbAllocSession')

unless errors.empty?
  puts "Error: missing dependencies: #{errors.join(',')}"
  exit
end

create_makefile("ctdb_ext")
