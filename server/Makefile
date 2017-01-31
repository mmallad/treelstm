make:
	g++ -o server Server.cpp TextService.cpp idl_types.cpp idl_constants.cpp -I/home/ubuntu/lua-5.3.3/install/include  -I/usr/local/include/thrift -lthrift  -lthriftnb -levent -lpthread -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_system -lboost_thread -lrt /home/ubuntu/lua-5.3.3/install/lib/liblua.a -ldl
clean:
	rm -rf server
