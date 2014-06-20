


#define DEFINE_check(x,NAME) \
static MDB_##x* check_##x(lua_State *L, int index) { \
MDB_##x* y; \
luaL_checktype(L, index, LUA_TUSERDATA); \
y = *(MDB_##x**)luaL_checkudata(L, index, NAME); \
if (y == NULL) lua_type_error(L,index,NAME); \
return y; \
}



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
