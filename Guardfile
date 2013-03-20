# A sample Guardfile
# More info at https://github.com/guard/guard#readme

# Add files and commands to this file, like the example:
#   watch(%r{file/path}) { `command(s)` }
#
guard :shell do
  # watch(/(.*).txt/) {|m| `tail #{m[0]}` }
  watch(%r{ext/ctdb_sdk/ctdb_sdk.c}) do |m|
    `rake compile -- --with-faircom-dir=/opt/osx.v10.6.64bit/`
  end
end
