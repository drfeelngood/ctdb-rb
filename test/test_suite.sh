#!/bin/bash

rm test_ctdb_rb.*
ruby test_ct_data_types.rb
ruby test_ct_session.rb
ruby test_ct_table.rb
ruby test_ct_field.rb
ruby test_ct_record.rb
ruby test_ct_query.rb
ruby test_ct_model.rb
