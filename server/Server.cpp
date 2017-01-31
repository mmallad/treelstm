//Lua c headers
/*extern "C" {
        #include "lua.h"
        #include "lauxlib.h"
        #include "lualib.h"
}*/
#include "lua.hpp"
#include "TextService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TNonblockingServer.h>
#include <fstream>
#include <iostream>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using boost::shared_ptr;
lua_State *L;
class TextServiceHandler : virtual public TextServiceIf {
 public:
  TextServiceHandler() {
    // Your initialization goes here
  }

  double getSimilarityScore(const std::string& ls, const std::string& rs) {
	lua_getglobal(L, "getScore"); 
	lua_pushstring(L, ls.c_str());
	lua_pushstring(L, rs.c_str());
	if (lua_pcall(L, 2, 1, 0)){
		std::cout << "Error calling lua function." << std::endl;
		return -1.0;
  	}
	return lua_tonumber(L, -1);
  }
};

int main(int argc, char **argv) {
  int port = 9191;
  shared_ptr<TextServiceHandler> handler(new TextServiceHandler());
  shared_ptr<TProcessor> processor(new TextServiceProcessor(handler));
  shared_ptr<TMultiplexedProcessor> multiplexing(new TMultiplexedProcessor());
  multiplexing.get()->registerProcessor("TextService", processor);
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(6);
  shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  TNonblockingServer tNonblockingServer(multiplexing, protocolFactory, port, threadManager);	
  //Starting Lua Script
  //lua_State *L;
  L = luaL_newstate();
  //luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);
  luaL_openlibs(L);
  if(luaL_loadfile(L, "relatedness/server.lua")){
	std::cout << "Error while loading file. Maybe its due to file was not found. " <<  lua_tostring(L, -1)  << std::endl;
	lua_close(L);
	return 1;
  }
  if (lua_pcall(L, 0, 0, 0)){
	std::cout << "Could not load given script file. Please check script. " <<  lua_tostring(L, -1) << std::endl;
	lua_close(L);
        return 1;
  }
  std::cout << "Lua script loaded and initialized successfully. Now starting thrift server." << std::endl;	
  std::cout << "Server started on " << port << " port" << std::endl;
  tNonblockingServer.serve();
  //TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  //server.serve();
  lua_close(L);
  return 0;
}

