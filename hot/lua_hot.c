
#include "lua.h"
#include "lauxlib.h"

#if !defined __APPLE__
#include "lj_obj.h"
#else
#include "lobject.h"
#include "lstate.h"
#endif

#if !defined __APPLE__
static int hot_swaplfunc(lua_State *L)
{
    TValue *o1, *o2;
    MRef ptr;

    luaL_checktype(L, 1, LUA_TFUNCTION);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    
    o1 = L->base;
    o2 = L->base + 1;

    ptr = (&gcval(o2)->fn)->l.pc;
    (&gcval(o2)->fn)->l.pc = (&gcval(o1)->fn)->l.pc;
    (&gcval(o1)->fn)->l.pc = ptr;

    lua_settop(L, 0);
    return 0;
}
#else
static int hot_swaplfunc(lua_State *L)
{
    StkId o1, o2;
    Proto *p1, *p2;

    luaL_checktype(L, 1, LUA_TFUNCTION);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    o1 = L->base;
    o2 = L->base + 1;

    p1 = o1->value.gc->cl.l.p;
    p2 = o2->value.gc->cl.l.p;
    o1->value.gc->cl.l.p = p2;
    o2->value.gc->cl.l.p = p1;
    
    lua_settop(L, 0);
    return 0;
}
#endif

static const struct luaL_Reg hot_funcs[] = 
{
    { "swaplfunc", hot_swaplfunc},
	{ NULL, NULL }
};

LUALIB_API void luaopen_hot(lua_State *L)
{  
    luaL_register(L, "hot", hot_funcs);
}