make:
	g++ -o score Server.cpp TextService.cpp idl_types.cpp idl_constants.cpp -I/home/ubuntu/torch/install/include  -I/usr/local/include/thrift -lthrift  -lthriftnb -levent -lpthread -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_system -lboost_thread -lrt -L/home/ubuntu/torch/install/lib -lluajit -lTH -ldl
clean:
	rm -rf score
