#include "base.h"
#include "gtest/gtest.h"

TEST(lua, struct) { 

    struct Player {
        std::string title;
        std::string name;
        std::string family;
        int level;
    } player ;

     

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if(CheckLua(L, luaL_dofile(L, "../src/lua_scripts/example.lua")) ){

        lua_getglobal(L, "PlayName");
        if (lua_isstring(L, -1)) {
            player.name = lua_tostring(L, -1);
            std::cout << "player.name = " << player.name << std::endl;
            ASSERT_EQ(player.name,"Ciaran");
        }
        lua_pop(L, 1);

        lua_getglobal(L, "PlayerTitle");
        if (lua_isstring(L, -1)) {
            player.title = lua_tostring(L, -1);
            std::cout << "player.title = " << player.title << std::endl;
            ASSERT_EQ(player.title,"Squire");
        }

        lua_getglobal(L, "PlayerFamily");
        if (lua_isstring(L, -1)) {
            player.family = lua_tostring(L, -1);
            std::cout << "player.name = " << player.family << std::endl;
            ASSERT_EQ(player.family,"Wirral");
        }
        lua_pop(L, 2);

        lua_getglobal(L, "PalyerLevel");
        if (lua_isinteger(L, -1)) {
            player.level = lua_tointeger(L, -1);
            std::cout << "player.level = " << player.level << std::endl;
            ASSERT_EQ(player.level, 20);
        }


        lua_getglobal(L, "player");

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

    lua_close(L);
  
}
