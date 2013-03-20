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

  --with-faicom-dir=/path/to/faircom
EOS
  printf("extconf.rb failure: %s\n", msg)
  exit
end

ct_dir   = with_config('faircom-dir')

api_dir  = case RUBY_PLATFORM
when /darwin/ then %w(ctree.ctdb multithreaded static).join(File::SEPARATOR)
when /linux/  then %w(ctree.ctdb multithreaded dynamic).join(File::SEPARATOR)
else
  puts "Unhandled platform `#{RUBY_PLATFORM}'"
  exit 1
end
  
lib      = "mtclient"
lib_dir  = File.join(ct_dir, 'lib', api_dir)
inc_dirs = [ File.join(ct_dir, 'include', 'sdk', api_dir),
             File.join(ct_dir, 'include') ]

inc_dirs.each do |path|
  fail("FairCom path `#{path}' does not exist") unless File.exist?(path)
  $INCFLAGS << " -I#{path}"
end

fail("FairCom path `#{lib_dir}' does not exist") unless File.exist?(lib_dir)
$LIBPATH  << "#{lib_dir}"
$CFLAGS   << " -fPIC"

p $LIBPATH
p $INCFLAGS

errors = []
errors << "'ctdbsdk.h'" unless have_header('ctdbsdk.h')
errors << "'#{lib}'"  unless have_library(lib, 'ctdbStringFormatToTime')
fail("missing dependencies: #{errors.join(',')}") unless errors.empty?

create_makefile("ctdb_ext")
