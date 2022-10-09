#include "base.h"
#include "gtest/gtest.h"

TEST(lua, load_lua_script) { 



    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(CheckLua(L, luaL_dofile(L, "../src/lua_scripts/example.lua")) ){
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1)) {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
            ASSERT_FLOAT_EQ(a_in_cpp, 118);
        }
    } else {
        ASSERT_TRUE(false);
    }

    lua_close(L);
  
}
