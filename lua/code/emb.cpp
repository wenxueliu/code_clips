/**
 * @author: Ma Tao
 * @bref:
 */

#include <lua.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "common.hpp"

class lua_node
{
private:
    lua_State  *_state;
public:
    lua_node()
        :_state(luaL_newstate())
    {
        assert(_state != NULL);
        luaL_openlibs(_state);
    }
    void execute(const std::string &code)
    {
        if ( luaL_dostring(_state, code.c_str()) != 0)
        {
            std::cout<<luaL_checkstring(_state, -1)<<std::endl;
        }
    }
    void execute_file(const std::string &fname)
    {
        if (luaL_dofile(_state, fname.c_str()) != 0)
        {
            std::cout<<"Error: "<<luaL_checkstring(_state, -1)<<std::endl;
        }
    }
    typedef const std::string cstring;

    int resume(int narg)
    {
        int rt = lua_resume(_state, narg);
        stack_dump(_state);
        if (rt == LUA_ERRRUN ||
            rt == LUA_ERRMEM ||
            rt == LUA_ERRERR)
        {
            throw luaL_checkstring(_state, -1);
        }
        return rt;
    }
    void event_loop(cstring &fname)
    {
        if (luaL_loadfile(_state, fname.c_str()) != 0)
        {
            throw luaL_checkstring(_state, -1);
        }

        for (int i = 0;;++i)
        {
            lua_pushinteger(_state, i);
            int rt = resume(1);
            if (rt == LUA_YIELD)
            {
                continue;
            }
            else
            {
                std::cout<<"Exit"<<std::endl;
                break;
            }
        }

    }
    void init_builtins()
    {
        const luaL_Reg _builtins[] = {
            {"test", lua_test},
            {"int", lua_int},
            {NULL, NULL}
        };
        lua_newtable(_state);
        luaL_register(_state, "emb", _builtins);
    }
private:
    static int lua_test(lua_State *l)
    {
        std::cout<<__func__<<" called"<<std::endl;
        lua_pushstring(l, __func__);
        lua_pushinteger(l, __LINE__);
        return 2;
    }
    static int lua_int(lua_State *l)
    {
        std::cout<<__func__<<" called"<<std::endl;
        return lua_yield(l, 0);
    }

public:
    ~lua_node()
    {
        lua_close(_state);
    }
};

int main(int argc, char *argv[])
{
    lua_node  node;

    try {
        node.execute("print \"hello world\"");
        node.execute_file("bootstrap.lua");
        node.init_builtins();

        node.execute_file("try_emb.lua");
        node.event_loop("try_resume.lua");
    } catch (const char *errmsg) {
        std::cout<<"Error: "<<errmsg<<std::endl;
    }
    return 0;

}