#include "base.h"
#include "gtest/gtest.h"


TEST(lua, function_simple) { 
 
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(CheckLua(L, luaL_dofile(L, "../src/lua_scripts/example.lua")) ){
        lua_getglobal(L, "AddStuff");
        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L, 3.5f);
            lua_pushnumber(L, 7.1f);

            if( CheckLua(L, lua_pcall(L, 2, 1, 0) ) ) {
                float result = (float)lua_tonumber(L, -1);
                std::cout << "[c++]: Got " << result <<std::endl; 
                ASSERT_FLOAT_EQ(result, 10.6f);
            }
        }
        lua_pop(L, 1);
    } else {
        ASSERT_TRUE(false);
    }

    lua_close(L);
}



TEST(lua_function, return_struct) { 

   struct Player {
        std::string title;
        std::string name;
        std::string family;
        int level;
    } player ;

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(CheckLua(L, luaL_dofile(L, "../src/lua_scripts/example.lua")) ){
        
        lua_getglobal(L, "GetPlayer");
        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L,0);
             
            if( CheckLua(L, lua_pcall(L, 1, 1, 0) ) ) {
                if(lua_istable(L, -1)) {
                   lua_pushstring(L, "Name");
                   lua_gettable(L, -2);
                   player.name = lua_tostring(L,-1);
                   lua_pop(L, 1);
       
                   lua_pushstring(L, "Title");
                   lua_gettable(L, -2);
                   player.title = lua_tostring(L,-1);
                   lua_pop(L, 1);
       
                   lua_pushstring(L, "Family");
                   lua_gettable(L, -2);
                   player.family = lua_tostring(L,-1);
                   lua_pop(L, 1);
       
                   lua_pushstring(L, "Level");
                   lua_gettable(L, -2);
                   player.level = lua_tointeger(L,-1);
                   lua_pop(L, 1);
            
                   ASSERT_EQ(player.level, 30);
       
               }

            } else {
                ASSERT_TRUE(false);
            }
        }
         
    } else {
        ASSERT_TRUE(false);
    }

    lua_close(L);
  
}

int host_c_function(lua_State *L) 
{
    float a = (float)lua_tonumber(L, 1);
    float b = (float)lua_tonumber(L, 2);
    std::cout << "[c++] host_c_function: " << a  << ", " << b << std::endl;
    lua_pushnumber(L, a * b);
    return 1; //return 1 value in lua stack
}

TEST(lua_function, call_c_function) { 

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "HostCFunction", host_c_function);

    if(CheckLua(L, luaL_dofile(L, "../src/lua_scripts/example.lua")) ){
        
        lua_getglobal(L, "DoAThing");
        if (lua_isfunction(L, -1)) {
            lua_pushnumber(L,1.0f);
            lua_pushnumber(L,2.0f);
             
        
            if( CheckLua(L, lua_pcall(L, 2, 1, 0) ) ) {
                float result = (float)lua_tonumber(L, -1);
                std::cout << "[c++]: Got " << result <<std::endl; 
                ASSERT_FLOAT_EQ(result, 22.0f);  
            } else {
                ASSERT_TRUE(false);
            }
        }
        lua_pop(L, 1);
    
    } else {
        ASSERT_TRUE(false);
    }

    lua_close(L);
  
}
