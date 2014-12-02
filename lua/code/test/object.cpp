#include <iostream>
#include <lua.hpp>
#include <string.h>

class test_object
{
public:

    static const char *type_id;

    static int new_object(lua_State *l)
    {

        char **p = (char**)lua_newuserdata(l, sizeof(const char*));

        const int size = 100;

        *p = new char[size];

        memset(*p, 0, size);

        strcpy(*p, "hello world");

        luaL_getmetatable(l, type_id);

        lua_setmetatable(l, -2);

        return 1;
    }

    static int show(lua_State *l)
    {

        const char **p = (const char **)luaL_checkudata(l, 1, type_id);

        std::cout<<"Show: "<<*p<<std::endl;

        return 0;
    }

    static int finalize(lua_State *l)
    {

        const char **p = (const char **)luaL_checkudata(l, 1, type_id);

        delete [] *p;

        *p = NULL;

        return 0;
}

};

const char *test_object::type_id = "test_object";

extern "C"

int luaopen_object(lua_State *l)
{

    const struct luaL_Reg _obj[] = {

    {"new", test_object::new_object},

    {"__gc", test_object::finalize},

    {"finalize", test_object::finalize},

    {NULL, NULL}
    };                                               /*使用luaL_Reg对类中多个函数进行注册*/

    luaL_newmetatable(l, test_object::type_id);   /*创建名为test_object的元表，并将其名字注册到注册表中*/

    luaL_register(l, NULL, _obj);                  /*将注册的_obj[]放入名为test_object的元表中*/

    lua_pushvalue(l, -1);

    lua_setfield(l, -2, "__index");                /*将元表自身作为自身的索引*/

    lua_newtable(l);                                 /*生成要返回的table*/

    lua_pushvalue(l, -2);

    lua_setfield(l, -2, "test_object");             /*将test_object作为其元表*/

    //lua_pop(l,2);

    lua_pushvalue(l, -1);

    lua_setglobal(l, "object");

    /*将该table声明为全局变量"object"，lua中require("object")后，可直接使用"object"引用元表，也可以通过require返回值调用该table*/

    return 1;

}
