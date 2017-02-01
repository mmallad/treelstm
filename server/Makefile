make:
	g++ -std=c++11 -o score Server.cpp TextService.cpp text_types.cpp text_constants.cpp -I/home/ubuntu/torch/install/include  -I/usr/local/include/thrift -lthrift  -lthriftnb -levent -lpthread -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_system -lboost_thread -lrt -L/home/ubuntu/torch/install/lib -lluajit -lTH -ldl
clean:
	rm -rf score
