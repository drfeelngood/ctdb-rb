# ctdb-rb 

A no fluff extension implementing the FairCom c-tree ctdb sdk.

## Install

Make sure you have our internal gem repository added to your gem sources.

    $ gem install ctdb -- --with-faircom-dir=/path/to/c-treeACE

### Session

A c-treeACE session can be started by creating an instace of `CT::Session` and
calling `#logon`:

```ruby
session = CT::Session.new(CT::SESSION_CTREE)
session.logon("host", "user", "pass")
```

### Table

Once a session is active, you can begin to open tables.

```ruby
table = CT::Table.new(session)
table.path = "/path/to/table"
table.open("table_name", CT::OPEN_NORMAL)
```

### Record

Now that we have a table open, lets locate a record based on a unique index.

```ruby
record = CT::Record.new(table)
record.clear
record.set_default_index("foo_ndx")
record.set_field_as_unsigned('id', 123)
record.find(CT::FIND_EQ)

puts record.get_field_as_unsigned('id') # => 123
```

Record sets!

```ruby
record = CT::Record.new(table)
record.clear
record.default_index = "foo_ndx"
record.set_field_as_string("foo", "bar")

bytes = 0
record.get_default_index.segments.each do |segment|
  bytes += segment.field.length
  bytes -= 1 if segment.absolute_byte_offset? # => Old school
end
record.set_on(bytes)
if record.first
  begin
    puts record.inspect
  end while record.next
end
```

### Cleanup

Be sure to close any tables and the session.

```ruby
table.close
session.logout
```

## CT::Model

"The" cTree ORM

```ruby
require 'ctdb'

class Foo < CT::Model
  self.table_path = "/path/to"
  self.table_name = :foos
  self.primary_key = :bar_ndx, increment: :sequence
end

CT::Model.session = { engine: "FAIRCOMS", username: "", password: "" }

foo = Foo.find(index: :foo_ndx).index_segments(bar: 1234, sequence: 2).eq
```

## CT::Query

Interface to perform record queries.

#### Options

* index
* index_segments
* limit
* offset
* filter
* transformer

```ruby
record = CT::Query.new(table).index(:bar_ndx).index_segments(sequence: 5).eq
```

## Contributing

* Fork the project
* Create a topic branch
* Hack away
* Submit a pull request

**Note:** Before you get started, scope out the documentation to get familiar with
the project.  You can do this by running `rake yard`, then open doc/index.html 
with your preferred browser.  Also, if your lazy like me run `rake guard`.  This 
will automagically compile the c extension after you modify the ctdb_skd.c file. 
This speeds up the process when debugging.    
