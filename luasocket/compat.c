#include "luasocket.h"
#include "compat.h"


/*
** Adapted from Lua 5.2
*/
void luasocket_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
#if LUA_VERSION_NUM==501
  luaL_checkstack(L, nup+1, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    lua_pushstring(L, l->name);
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -(nup+1));
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
#else
  luaL_setfuncs(L, l, nup);
#endif
}

/*
** Duplicated from Lua 5.2
*/
void *luasocket_testudata (lua_State *L, int ud, const char *tname) {
#if LUA_VERSION_NUM==501
  void *p = lua_touserdata(L, ud);
  if (p != NULL) {  /* value is a userdata? */
    if (lua_getmetatable(L, ud)) {  /* does it have a metatable? */
      luaL_getmetatable(L, tname);  /* get correct metatable */
      if (!lua_rawequal(L, -1, -2))  /* not the same? */
        p = NULL;  /* value is a userdata with wrong metatable */
      lua_pop(L, 2);  /* remove both metatables */
      return p;
    }
  }
  return NULL;  /* value is not a userdata with a metatable */
#else
  return luaL_testudata(L, ud, tname);
#endif
}

