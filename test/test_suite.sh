#!/bin/bash

ps -e|grep ctreesql|grep -v grep >/dev/null
if [ $? -ne 0 ]
then
    echo "ERROR: ctreesql server is not running"
    exit 1
fi

rm test_ctdb_rb.*
ruby test_ct_data_types.rb
ruby test_ct_session.rb
ruby test_ct_table.rb
ruby test_ct_field.rb
ruby test_ct_record.rb
ruby test_ct_query.rb
ruby test_ct_model.rb
