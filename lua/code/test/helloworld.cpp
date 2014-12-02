#include <stdio.h>
#include <lua.hpp>
#include <iostream>

extern "C"
int luaopen_helloworld(lua_State *L)

{

    std::cout << "Hello World!"<< std::endl;

	return 0;

}

