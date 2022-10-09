#include "base.h"
#include "gtest/gtest.h"

TEST(lua, load_lua_string) { 


    std::string cmd = "a = 7 + 11 + math.sin(23.7)";

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(CheckLua(L, luaL_dostring(L, cmd.c_str())) ){
        lua_getglobal(L, "a");
        if (lua_isnumber(L, -1)) {
            float a_in_cpp = (float)lua_tonumber(L, -1);
            std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
            ASSERT_FLOAT_EQ(a_in_cpp, 17.009514f);
        }
    } else {
        ASSERT_TRUE(false);
    }

    lua_close(L);
  
}
