
#include <iostream>
#include <string>
#include "gtest/gtest.h"

extern "C"
{

#include "lua5.4/lua.h"
#include "lua5.4/lauxlib.h"
#include "lua5.4/lualib.h"

}

static  bool CheckLua(lua_State* L, int r)
{
    if (r != LUA_OK) {
       const char* errormsg = lua_tostring(L, -1);
       std::cout << "errormsg: [" << (errormsg ? errormsg : "nil")  << "]" << std::endl << std::flush;
       return false;
    }
    return true;
}

