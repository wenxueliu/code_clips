#include <lua.hpp>
#include <iostream>

inline static void stack_dump(lua_State *l)

{

    for (int i = 1; i <= lua_gettop(l); ++i)

    {

        switch (lua_type(l, i))

        {

            case LUA_TNUMBER:

                std::cout<<"["<<i<<"]: "<<"Number("<<luaL_checkinteger(l, i)<<")"<<std::endl;

                break;

            case LUA_TSTRING:

                std::cout<<"["<<i<<"]: "<<"String("<<luaL_checkstring(l, i)<<")"<<std::endl;

                break;


            default:

                std::cout<<"["<<i<<"]: "<<"Object of "<<luaL_typename(l, i)<<std::endl;

                break;
        }
    }
}

extern "C"  //C API是针对C函数编写的，当C++语言要调用时，要采用extern "C"的形式

int luaopen_dump(lua_State *l)

{

    lua_pushinteger(l, -2);

    lua_pushstring(l, "abc");

    stack_dump(l);

    lua_pop(l, 2);

    return 0;
}
