


#define DEFINE_check(x,NAME) \
static MDB_##x* check_##x(lua_State *L, int index) { \
MDB_##x* y; \
luaL_checktype(L, index, LUA_TUSERDATA); \
y = *(MDB_##x**)luaL_checkudata(L, index, NAME); \
if (y == NULL) lua_type_error(L,index,NAME); \
return y; \
}

//MDB_env env = check_env(L,3)

---将 src[index] push 到 dst 栈顶
void MoveValue(lua_State *src, lua_State *dst, int index)
{
    int type = lua_type(src, index);
    const char *str=NULL;
    size_t len=0;
    switch (type)
    {
        case LUA_TSTRING:
            str = luaL_checklstring(src, index, &len);
            lua_pushlstring(dst, str, len);
            break;
        case LUA_TBOOLEAN:
            len = lua_toboolean(src, index);
            lua_pushboolean(dst, len);
            break;
        case LUA_TNUMBER:
            len = luaL_checkinteger(src, index);
            lua_pushinteger(dst, len);
            break;
        case LUA_TNIL:
            lua_pushnil(dst);
            break;
        case LUA_TTABLE:
            MoveTable(src,dst,index);
            break;
        default:
            THROW_EXCEPTION_EX("not supported argument");
            break;
    }

}



//================iter for userdata====================

//foo.c
static int nextFooId = 0;
struct Foo {
   int id;
   char name[ 256 ];
};

static const char* foo_metatable = "foo";

int foo_iter( lua_State* L )
{
   if (++nextFooId >= 10)
      return 0;

   // create and initialize foo
   Foo* foo = ( Foo* )lua_newuserdata( L, sizeof( Foo ) );
   foo->id = nextFooId;
   sprintf(foo->name, "Foo %d", foo->id);

   // set metatable for foo
   luaL_getmetatable( L, foo_metatable );
   lua_setmetatable( L, -2 );
   return 1;
}


int foo_list( lua_State* L )
{
   lua_pushcclosure( L, foo_iter, 1 );
   return 1;
}

int foo_name( lua_State* L )
{
   Foo* f = ( Foo* )luaL_checkudata( L, 1, foo_metatable );
   lua_pushstring( L, f->name );
   return 1;
}

int foo_id( lua_State* L )
{
   Foo* f = ( Foo* )luaL_checkudata( L, 1, foo_metatable );
   lua_pushinteger( L, f->id );
   return 1;
}

int foo_bar( lua_State* L )
{
   lua_pushboolean( L, rand()%2 );
   return 1;
}

int foo_close( lua_State* L ) { return 0;/*omitted. this part works*/ }

extern "C" int luaopen_foo( lua_State* L )
{
   const luaL_Reg foo_methods[] = { 
      { "name", foo_name },
      { "id", foo_id },
      { "bar", foo_bar },
      { "__gc", foo_close },
      { NULL, NULL }
   };
   luaL_newmetatable( L, foo_metatable );
   luaL_setfuncs( L, foo_methods, 0 );

   // copy the metatable to the top of the stack 
   // and set it as the __index value in the metatable
   lua_pushvalue(L, -1);
   lua_setfield( L, -2, "__index");

   const luaL_Reg foo[] = { 
      { "list", foo_list },
      { NULL, NULL },
   };
   luaL_newlib( L, foo );

   return 1;
}


test.lua 

	foo = require 'foo'

	for f in foo.list() do
	   if f:bar() then
		  print("success")
	   else
		  print("failed")
	   end
	   print(string.format( "%d: %s", f:id(), f:name()))
	end
