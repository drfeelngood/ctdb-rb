require 'mkmf'

if enable_config("debug")
  $defs.push("-DDEBUG")
end

errors = []
errors << "'ctdbsdk.h'" unless find_header('ctdbsdk.h')
errors << "'ctclient'"  unless find_library('ctclient', 'ctdbAllocSession')

unless errors.empty?
  puts "Error: missing dependencies: #{errors.join(',')}"
  exit
end

create_makefile("ctdb_ext")
