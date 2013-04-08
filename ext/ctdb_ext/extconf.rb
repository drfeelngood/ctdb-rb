require 'mkmf'

def osx?
  !!(RUBY_PLATFORM =~ /darwin/)
end

def linux?
  !!(RUBY_PLATFORM =~ /linux/)
end

def fail(msg)
  msg << <<-EOS
  
Try using the following options:

  --with-sdk-include=/path/to/faircom/include/sdk
  --with-sdk-lib=/path/to/faircom/lib/sdk
EOS
  printf("extconf.rb failure: %s\n", msg)
  exit
end

$CFLAGS << " -fPIC"

dir_config("sdk")

errors = []
errors << "'ctdbsdk.h'" unless have_header('ctdbsdk.h')
errors << "'mtclient'"  unless have_library('mtclient', 'ctdbAllocSession')
fail("missing dependencies: #{errors.join(',')}") unless errors.empty?

create_makefile("ctdb_ext")
