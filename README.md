# ctdb-rb 

A no fluff extension implementing the FairCom c-tree ctdb sdk.

## Install

```
$ gem install ctdb
```

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

```ruby
require 'ctdb'

class Person < CT::Model
  self.table_path = "/path/to"
  self.table_name = :people
  self.primary_key = :index_on_email
end

# Configure c-tree session
CT::Model.session = { 
    engine: "FAIRCOMS", 
    username: "", 
    password: "",
    mode: CT::SESSION_CTREE
}

# Find a record by index
person = Person.find_by(:index_on_email, "dan@dj-agiledev.com")
```

## CT::Query

Simple interface to perform `CT::Record` queries.

#### Options

* find_mode
* index
* index_segments
* limit
* offset
* filter
* endif
* transformer

```ruby

person = CT::Query.new(table)
                  .index(:email_ndx)
                  .index_segments(email: "dan@dj-agiledev.com")
                  .eq

# Server side filtering.  Find all people in the great state of Oklahoma
people = CT::Query.new(table)
                  .filter(%Q{strncmp(state, "OK", 2) == 0})
                  .set
```

## Contributing

* Fork the project
* Create a topic branch
* Hack away
* Submit a pull request
