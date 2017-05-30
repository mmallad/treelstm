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
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;
//using namespace std;
using boost::shared_ptr;
lua_State *L;
class TextServiceHandler : virtual public TextServiceIf {
private:
	std::ifstream *inputFile;
public:
  
  TextServiceHandler(std::string const& inputFilePath) {
    	// Your initialization goes here
	this->inputFile = new std::ifstream(inputFilePath);	
  }

  ~TextServiceHandler(){
	if(this->inputFile != NULL)
		delete this->inputFile;
  }
  double getSimilarityScore(const std::string& ls, const std::string& rs) {
	lua_getglobal(L, "getScore"); 
	std::string tls = ls;
	std::string trs = rs;
	boost::trim(tls);
	boost::trim(trs);
	lua_pushstring(L, tls.c_str());
	lua_pushstring(L, trs.c_str());
	if (lua_pcall(L, 2, 1, 0)){
		std::cout << "Error calling lua function." << std::endl;
		return -1.0;
  	}
	return lua_tonumber(L, -1);
  }
  int32_t getSimilarityScoreByIndex(const std::string& ls, const std::vector<Suubjects> & subjects) {
  	double score = -1.0;
	int32_t index = -1;
	int32_t i = 0;
	for(Suubjects subject: subjects){
		for(std::string text : subject.text){
			double tempScore = this->getSimilarityScore(ls, text);
			if(tempScore > 3.5 && tempScore > score){
				score = tempScore;
				index = i;
			}
		}
		i++;
	}
	return index;
  }
  inline std::vector<std::string> split(std::string str, char delimiter) {
  	std::vector<std::string> internal;
  	std::stringstream ss(str);
  	std::string tok;
  	while(getline(ss, tok, delimiter)) {
    		internal.push_back(tok);
  	}
  	return internal;
  }
  void getLabel(std::string& _return, const std::string& ls) {
    // Your implementation goes here
    std::string inputLine; 
    double score = -1.0;
    std::string label = "NO_LABEL";
    while (std::getline(*this->inputFile, inputLine)){
        std::vector<std::string> parts = this->split(inputLine, '\t');
	std::string rs = parts.front();
	std::string tls = ls;
	boost::trim(rs);
	boost::trim(tls);
	if(boost::iequals(tls, rs)){
		label = parts.back();
		break;
	}
	//std::cout << "Input : " << parts.front() << " Label : " << parts.back() << std::endl;
	double tempScore = this->getSimilarityScore(ls, parts.front());
	//std::cout << "Input : " << parts.front() << " Label : " << parts.back() << " Score : " << tempScore << std::endl;
	//double tempScore = this->getSimilarityScore(ls, parts.front());
	if(tempScore >= 3 && tempScore > score){
		score = tempScore;
		//std::cout << "Score : " << score << " " << std::endl;
		label = parts.back();
		if(tempScore >= 4.5) break;
	}
    }
    //std::cout << "Label : " << label << std::endl;
    _return = label;
  }
};

int main2(int argc, char **argv){
	TextServiceHandler service("/home/innovisto/input_sen.txt");
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
	//std::cout << "Score : " << service.getSimilarityScore("I want to have dinner", "I am going to have dinner tonight") << std::endl;	
	//delete service;
	std::string result;
	service.getLabel(result, "Dinner");
	std::cout << "Result : " << result << std::endl;
	lua_close(L);
	return 0;
}
int main(int argc, char **argv) {
  int port = 9191;
  std:: string inputFilePath = "/usr/local/iloop/treelstm/data/input_sen.txt";
  shared_ptr<TextServiceHandler> handler(new TextServiceHandler(inputFilePath));
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

