#include "lua.hpp"
#include <iostream>

int main(){
lua_State *L;
        L = luaL_newstate();
	luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);
        luaL_openlibs(L);
	if(luaL_loadfile(L, "relatedness/server.lua")){
	std::cout << lua_tostring(L, -1) << std::endl; 
}
if (lua_pcall(L, 0, 0, 0)){
        std::cout << "Could not load given script file. Please check script. " <<  lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return 1;
  }
return 0;
}
