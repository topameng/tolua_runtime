/* 
 * Copyright (c) 2014.9 , topameng(topameng@gmail.com)
 * Use, modification and distribution are subject to the "The MIT License"
*/

#include <string.h>
#if !defined __APPLE__
#include <malloc.h>
#endif
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif


#if !defined(LUA_RIDX_MAINTHREAD)
#define LUA_RIDX_MAINTHREAD	1
#endif

#if !defined(LUA_RIDX_GLOBALS)
#define LUA_RIDX_GLOBALS 2
#endif

#define LUA_RIDX_TRACEBACK 	3
#define LUA_RIDX_UBOX 		4
#define LUA_RIDX_FIXEDMAP	5
#define LUA_RIDX_CHECKVALUE	6
#define LUA_RIDX_PACKVEC3	7
#define LUA_RIDX_UNPACKVEC3	8
#define LUA_RIDX_PACKVEC2	9
#define LUA_RIDX_UNPACKVEC2	10
#define LUA_RIDX_PACKVEC4	11
#define LUA_RIDX_UNPACKVEC4	12
#define LUA_RIDX_PACKQUAT	13
#define LUA_RIDX_UNPACKQUAT	14
#define LUA_RIDX_PACKCLR	15
#define LUA_RIDX_UNPACKCLR	16
#define LUA_RIDX_PACKLAYERMASK      17
#define LUA_RIDX_UNPACKLAYERMASK    18
#define LUA_RIDX_REQUIRE            19
#define LUA_RIDX_INT64              20
#define LUA_RIDX_VPTR               21

#define LUA_NULL_USERDATA 	1
#define TOLUA_NOPEER    	LUA_REGISTRYINDEX 		
#define abs_index(L, i)  ((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : lua_gettop(L) + (i) + 1)


#define FLAG_INDEX_ERROR 1
#define FLAG_INT64       2

static int toluaflags       = 1;
static int tag = 0;  
static int gettag = 0;
static int settag = 0;
static int vptr = 1;

static int _print(lua_State* L, const char* str)
{
    lua_getglobal(L, "print");
    lua_pushstring(L, str);    
    lua_call(L, 1, 0);
    return 0;
}


/*---------------------------tolua extend functions--------------------------------*/
LUALIB_API void* tolua_tag()
{
	return &tag;
}

LUALIB_API void tolua_newudata(lua_State *L, int val)
{
	int* pointer = (int*)lua_newuserdata(L, sizeof(int));    
    lua_pushvalue(L, TOLUA_NOPEER);            
    lua_setfenv(L, -2);                        
	*pointer = val;
}

LUALIB_API int tolua_rawnetobj(lua_State *L, int index)
{
    int *udata = lua_touserdata(L, index);
    
    if (udata != NULL) 
    {
        return *udata;
    }
    else if (lua_istable(L, index))
    {
        lua_pushvalue(L, index);        
        lua_pushlightuserdata(L, &vptr);
        lua_rawget(L, -2);                

        if (lua_isuserdata(L, -1))
        {
            lua_replace(L, index);
            udata = lua_touserdata(L, index);

            if (udata != NULL) 
            {
                return *udata;
            }
        }    
        else
        {
            lua_pop(L, 1);
        }               
    }    

	return -1;
}

LUALIB_API char* tolua_tocbuffer(const char* csBuffer, int sz)
{
	char* buffer = (char*)malloc(sz+1);
	memcpy(buffer, csBuffer, sz);	
	buffer[sz] = '\0';
	return buffer;
}

LUALIB_API void tolua_freebuffer(void* buffer)
{
  free(buffer);
}

LUALIB_API void tolua_getvec2(lua_State* L, int pos, float* x, float* y)
{
	lua_getref(L, LUA_RIDX_UNPACKVEC2);
	lua_pushvalue(L, pos);
	lua_call(L, 1, 2);
	*x = (float)lua_tonumber(L, -2);
	*y = (float)lua_tonumber(L, -1);
	lua_pop(L, 2);
}

LUALIB_API void tolua_getvec3(lua_State* L, int pos, float* x, float* y, float* z)
{
	lua_getref(L, LUA_RIDX_UNPACKVEC3);
	lua_pushvalue(L, pos);
	lua_call(L, 1, 3);
	*x = (float)lua_tonumber(L, -3);
	*y = (float)lua_tonumber(L, -2);
	*z = (float)lua_tonumber(L, -1);
	lua_pop(L, 3);
}

LUALIB_API void tolua_getvec4(lua_State* L, int pos, float* x, float* y, float* z, float* w)
{
	lua_getref(L, LUA_RIDX_UNPACKVEC4);
	lua_pushvalue(L, pos);
	lua_call(L, 1, 4);
	*x = (float)lua_tonumber(L, -4);
	*y = (float)lua_tonumber(L, -3);
	*z = (float)lua_tonumber(L, -2);
	*w = (float)lua_tonumber(L, -1);
	lua_pop(L, 4);
}

LUALIB_API void tolua_getquat(lua_State* L, int pos, float* x, float* y, float* z, float* w)
{
	lua_getref(L, LUA_RIDX_UNPACKQUAT);
	lua_pushvalue(L, pos);
	lua_call(L, 1, 4);
	*x = (float)lua_tonumber(L, -4);
	*y = (float)lua_tonumber(L, -3);
	*z = (float)lua_tonumber(L, -2);
	*w = (float)lua_tonumber(L, -1);
	lua_pop(L, 4);
}

LUALIB_API void tolua_getclr(lua_State* L, int pos, float* r, float* g, float* b, float* a)
{
	lua_getref(L, LUA_RIDX_UNPACKCLR);
	lua_pushvalue(L, pos);
	lua_call(L, 1, 4);
	*r = (float)lua_tonumber(L, -4);
	*g = (float)lua_tonumber(L, -3);
	*b = (float)lua_tonumber(L, -2);
	*a = (float)lua_tonumber(L, -1);
	lua_pop(L, 4);
}

LUALIB_API int tolua_getlayermask(lua_State* L, int pos)
{
    if (lua_isnumber(L, pos))
    {
        return (int)lua_tointeger(L, pos);
    }

    lua_getref(L, LUA_RIDX_UNPACKLAYERMASK);
    lua_pushvalue(L, pos);
    lua_call(L, 1, 1);
    int mask = (int)lua_tointeger(L, -1);
    lua_pop(L, 1);
    return mask;
}

LUALIB_API void tolua_pushvec2(lua_State* L, float x, float y)
{
	lua_getref(L, LUA_RIDX_PACKVEC2);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_call(L, 2, 1);
}

LUALIB_API void tolua_pushvec3(lua_State* L, float x, float y, float z)
{
	lua_getref(L, LUA_RIDX_PACKVEC3);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);
	lua_call(L, 3, 1);
}

LUALIB_API void tolua_pushvec4(lua_State* L, float x, float y, float z, float w)
{
	lua_getref(L, LUA_RIDX_PACKVEC4);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);
	lua_pushnumber(L, w);
	lua_call(L, 4, 1);
}

LUALIB_API void tolua_pushquat(lua_State* L, float x, float y, float z, float w)
{
	lua_getref(L, LUA_RIDX_PACKQUAT);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);
	lua_pushnumber(L, w);
	lua_call(L, 4, 1);
}

LUALIB_API void tolua_pushclr(lua_State* L, float r, float g, float b, float a)
{
	lua_getref(L, LUA_RIDX_PACKCLR);
	lua_pushnumber(L, r);
	lua_pushnumber(L, g);
	lua_pushnumber(L, b);
	lua_pushnumber(L, a);
	lua_call(L, 4, 1);
}

LUALIB_API void tolua_pushlayermask(lua_State* L, int mask)
{
    lua_getref(L, LUA_RIDX_PACKLAYERMASK);
    lua_pushnumber(L, mask);
    lua_call(L, 1, 1);
}


LUA_API const char* tolua_tolstring(lua_State *L, int index, int* len) 
{
    size_t sz;
    const char* ret = lua_tolstring(L, index, &sz);
    *len = (int)sz;
    return ret;
}

LUA_API void  tolua_pushlstring(lua_State *L, const char *s, int l)
{
    lua_pushlstring(L, s, (size_t)l);
}

LUA_API void tolua_newuserdata (lua_State *L, int val)
{
    int* pointer = (int*)lua_newuserdata(L, sizeof(int));
    *pointer = val;
}

LUA_API int tolua_objlen(lua_State *L, int idx)
{
    size_t len = lua_objlen(L, idx);
    return (int)len;
}

LUALIB_API int tolua_loadbuffer(lua_State *L, const char *buff, int sz, const char *name)
{
    return luaL_loadbuffer(L, buff, (size_t)sz, name);
}

static int class_index_event(lua_State* L)
{
	int t = lua_type(L, 1);

    if (t == LUA_TUSERDATA)
    {    	
        lua_getfenv(L,1);

        if (!lua_rawequal(L, -1, TOLUA_NOPEER))     // stack: t k env
        {
            while (lua_istable(L, -1))                       // stack: t k v mt 
            {      
                lua_pushvalue(L, 2); 
                lua_rawget(L, -2);
            
                if (!lua_isnil(L, -1))
                {                    
                    return 1;
                }

                lua_pop(L, 1);
                lua_pushlightuserdata(L, &gettag);          
                lua_rawget(L, -2);                      //stack: obj key env tget
            
                if (lua_istable(L, -1))
                {                    
                    lua_pushvalue(L, 2);                //stack: obj key env tget key
                    lua_rawget(L, -2);                  //stack: obj key env tget func 

                    if (lua_isfunction(L, -1))
                    {                        
                        lua_pushvalue(L, 1);
                        lua_call(L, 1, 1);
                        return 1;
                    }    

                    lua_pop(L, 1);                 
                }

                lua_pop(L, 1); 

                if (lua_getmetatable(L, -1) == 0)               // stack: t k v mt mt
                {
                    lua_pushnil(L);
                }

                lua_remove(L, -2);                              // stack: t k v mt
            }

            /*lua_pushvalue(L, 2); 
            lua_gettable(L, -2);             

            if (!lua_isnil(L, -1))
            {
                return 1;
            }*/            
        };

        lua_settop(L,2);                                        				
    	lua_pushvalue(L, 1);						// stack: obj key obj	

    	while (lua_getmetatable(L, -1) != 0)
    	{        	
        	lua_remove(L, -2);						// stack: obj key mt

			if (lua_isnumber(L,2))                 	// check if key is a numeric value
			{		    
		    	lua_pushstring(L,".geti");
		    	lua_rawget(L,-2);                   // stack: obj key mt func

		    	if (lua_isfunction(L,-1))
		    	{
		        	lua_pushvalue(L,1);
		        	lua_pushvalue(L,2);
		        	lua_call(L,2,1);
		        	return 1;
		    	}
			}
			else
        	{
        		lua_pushvalue(L, 2);			    // stack: obj key mt key
        		lua_rawget(L, -2);					// stack: obj key mt value        

        		if (!lua_isnil(L, -1))
        		{
        	    	return 1;
        		}
                
                lua_pop(L, 1);
				lua_pushlightuserdata(L, &gettag);        	
        		lua_rawget(L, -2);					//stack: obj key mt tget

        		if (lua_istable(L, -1))
        		{
        	    	lua_pushvalue(L, 2);			//stack: obj key mt tget key
        	    	lua_rawget(L, -2);           	//stack: obj key mt tget value 

        	    	if (lua_isfunction(L, -1))
        	    	{
        	        	lua_pushvalue(L, 1);
        	        	lua_call(L, 1, 1);
        	        	return 1;
        	    	}                    
        		}
    		}

            lua_settop(L, 3);
        }

        lua_settop(L, 2);
        int *udata = lua_touserdata(L, 1);

        if (*udata == LUA_NULL_USERDATA)
        {
            return luaL_error(L, "attemp to index %s on a nil value", lua_tostring(L, 2));   
        }
        
        if (toluaflags & FLAG_INDEX_ERROR)
        {
            return luaL_error(L, "field or property %s does not exist", lua_tostring(L, 2));
        }        
    }
    else if(t == LUA_TTABLE)
    {
    	lua_pushvalue(L,1);                          //stack: obj key obj

		while (lua_getmetatable(L, -1) != 0)         //stack: obj key obj mt
    	{
        	lua_remove(L, -2);						// stack: obj key mt

        	lua_pushvalue(L, 2);			    	// stack: obj key mt key
        	lua_rawget(L, -2);						// stack: obj key mt value        

        	if (!lua_isnil(L, -1))
        	{
        		if (lua_isfunction(L, -1))			//cache static function
        		{
        			lua_pushvalue(L, 2);           // stack: obj key mt value key
        			lua_pushvalue(L, -2);          // stack: obj key mt value key value
        			lua_rawset(L, 1);
        		}

        	    return 1;
        	}        	
        	
            lua_pop(L, 1);        
			lua_pushlightuserdata(L, &gettag);        	
        	lua_rawget(L, -2);						//stack: obj key mt tget

        	if (lua_istable(L, -1))
        	{
        	    lua_pushvalue(L, 2);				//stack: obj key mt tget key
        	    lua_rawget(L, -2);           		//stack: obj key mt tget value 

        	    if (lua_isfunction(L, -1))
        	    {        	        	
        	        lua_call(L, 0, 1);
        	        return 1;
        	    }
        	}
    		
        	lua_settop(L, 3);
    	}

        lua_settop(L, 2);

        if (toluaflags & FLAG_INDEX_ERROR)
        {
            return luaL_error(L, "field or property %s does not exist", lua_tostring(L, 2));               
        }      
    }

    lua_pushnil(L);
    return 1;
}

static void storeatubox (lua_State* L, int lo)
{
    lua_getfenv(L, lo);                         // stack: t, k, v

    if (lua_rawequal(L, -1, TOLUA_NOPEER)) 
    {
        lua_pop(L, 1);
        lua_newtable(L);                        // stack: t, k, v, t
        lua_pushvalue(L, -1);                   // stack: t, k, v, t, t        
        lua_setfenv(L, lo);                     // stack: t, k, v, t
    };

    lua_insert(L, -3);
    lua_settable(L, -3);
    lua_pop(L, 1);
}

static int class_newindex_event(lua_State* L)
{
	int t = lua_type(L, 1);

	if (t == LUA_TUSERDATA)
    {
        lua_getfenv(L, 1);

        if (!lua_rawequal(L, -1, TOLUA_NOPEER)) 
        {                
            while (lua_istable(L, -1))                       // stack: t k v mt 
            {       
                lua_pushvalue(L, 2);                        // stack: t k v mt k
                lua_rawget(L, -2);                          // stack: t k v mt value        

                if (!lua_isnil(L, -1))
                {                    
                    lua_pop(L, 1);
                    lua_insert(L, -3);
                    lua_rawset(L, -3);
                    return 0;
                }

                lua_pop(L, 1);                
                lua_pushlightuserdata(L, &settag);              // stack: t k v mt tset
                lua_rawget(L, -2);                   

                if (lua_istable(L, -1)) 
                {
                    lua_pushvalue(L, 2);                         // stack: t k v mt tset k
                    lua_rawget(L, -2);                    

                    if (lua_isfunction(L, -1)) 
                    {                          
                        lua_pushvalue(L, 1); 
                        lua_pushvalue(L, 3); 
                        lua_call(L, 2, 0);
                        return 0;
                    }

                    lua_pop(L, 1);                              // stack: t k v mt tset 
                }  

                lua_pop(L, 1);                                  // stack: t k v mt

                if (lua_getmetatable(L, -1) == 0)               // stack: t k v mt mt
                {
                    lua_pushnil(L);
                }

                lua_remove(L, -2);                              // stack: t k v mt
            }              
        }

        lua_settop(L, 3);
    	lua_getmetatable(L,1);

    	while (lua_istable(L, -1))                			// stack: t k v mt
    	{        	
    		if (lua_isnumber(L, 2))
    		{
				lua_pushstring(L,".seti");
                lua_rawget(L,-2);                      		// stack: obj key mt func

                if (lua_isfunction(L,-1))
                {
                    lua_pushvalue(L,1);
                    lua_pushvalue(L,2);
                    lua_pushvalue(L,3);
                    lua_call(L,3,0);                    
                }

                return 0;
    		}
    		else
        	{
        		lua_pushlightuserdata(L, &settag);
        		lua_rawget(L, -2);                      	// stack: t k v mt tset

        		if (lua_istable(L, -1))
        		{
            		lua_pushvalue(L, 2);
            		lua_rawget(L, -2);                     	// stack: t k v mt tset func

            		if (lua_isfunction(L, -1))
            		{
                		lua_pushvalue(L, 1);
                		lua_pushvalue(L, 3);
                		lua_call(L, 2, 0);
                		return 0;
            		}

            		lua_pop(L, 1);                          // stack: t k v mt tset 
        		}

        		lua_pop(L, 1);                              // stack: t k v mt 

        		if (lua_getmetatable(L, -1) == 0)           // stack: t k v mt mt
        		{
            		lua_pushnil(L);
        		}

        		lua_remove(L, -2);                          // stack: t k v mt 
        	}
    	}

        lua_settop(L, 3);                                       // stack: t k v
        int *udata = lua_touserdata(L, 1);

        if (*udata == LUA_NULL_USERDATA)
        {
            return luaL_error(L, "attemp to index %s on a nil value", lua_tostring(L, 2));   
        }        
                
        storeatubox(L, 1);
	}
	else if (t == LUA_TTABLE)
	{
		lua_getmetatable(L, 1);								// stack: t k v mt 

		while (lua_istable(L, -1))                			// stack: t k v mt 
    	{  		
			lua_pushlightuserdata(L, &settag);				// stack: t k v mt tset
        	lua_rawget(L, -2);       

        	if (lua_istable(L,-1)) 
        	{
            	lua_pushvalue(L,2);  						// stack: t k v mt tset k
            	lua_rawget(L,-2);

            	if (lua_isfunction(L,-1)) 
            	{  
                	lua_pushvalue(L,1); 
                	lua_pushvalue(L,3); 
                	lua_call(L,2,0);
                	return 0;
            	}

            	lua_pop(L, 1);                          	// stack: t k v mt tset 
        	}  

			lua_pop(L, 1);                              	// stack: t k v mt

        	if (lua_getmetatable(L, -1) == 0)           	// stack: t k v mt mt
        	{
            	lua_pushnil(L);
        	}

        	lua_remove(L, -2);                          	// stack: t k v mt
        }

        lua_settop(L, 3); 
        return luaL_error(L, "field or property %s does not exist", lua_tostring(L, 2));        
	}

    return 0;
}

static int enum_index_event (lua_State* L)
{
	lua_getmetatable(L, 1);									//stack: t, k, mt

	if (lua_istable(L, -1))
	{
		lua_pushvalue(L, 2);								//stack: t, k, mt, k
		lua_rawget(L, -2);									//stack: t, k, mt, v

		if (!lua_isnil(L, -1))
		{		
			return 1;
		}

		lua_pop(L, 1);										//stack: t, k, mt
		lua_pushlightuserdata(L, &gettag);		
		lua_rawget(L, -2); 									//stack: t, k, mt, tget

		if (lua_istable(L,-1)) 
		{
            lua_pushvalue(L,2);  							//stack: t k mt tget k
            lua_rawget(L,-2);								//stack: t k mt tget v

           	if (lua_isfunction(L,-1)) 
			{  
				lua_call(L, 0, 1);					
				lua_pushvalue(L,2); 				
				lua_pushvalue(L,-2); 				
				lua_rawset(L, 3);		
				return 1;			
			}

			lua_pop(L, 1);
        }			
	}

	lua_settop(L, 2);
	lua_pushnil(L);
	return 1;	
}

static int enum_newindex_event(lua_State* L)
{	
	luaL_error(L, "the left-hand side of an assignment must be a variable, a property or an indexer");
    return 1;
}

static int static_index_event(lua_State* L)
{    
    lua_pushvalue(L, 2);                    //stack: t key key
    lua_rawget(L, 1);                       //stack: t key value        

    if (!lua_isnil(L, -1))
    {
        return 1;
    }

    lua_pop(L, 1);            
    lua_pushlightuserdata(L, &gettag);      //stack: t key tag    
    lua_rawget(L, 1);                       //stack: t key tget

    if (lua_istable(L, -1))
    {
        lua_pushvalue(L, 2);                //stack: obj key tget key
        lua_rawget(L, -2);                  //stack: obj key tget func 

        if (lua_isfunction(L, -1))
        {                       
            lua_call(L, 0, 1);
            return 1;
        }        
    }
    
    lua_settop(L, 2);
    luaL_error(L, "field or property %s does not exist", lua_tostring(L, 2));    
    return 1;
}

static int static_newindex_event(lua_State* L)
{
    lua_pushlightuserdata(L, &settag);              //stack: t k v tag
    lua_rawget(L, 1);                               //stack: t k v tset

    if (lua_istable(L,-1)) 
    {
        lua_pushvalue(L, 2);                         //stack: t k v tset k
        lua_rawget(L, -2);                           //stack: t k v tset func

        if (lua_isfunction(L, -1))
        {    
            lua_pushvalue(L,1); 
            lua_pushvalue(L,3); 
            lua_call(L,2,0);
            return 0;
        }
    }

    lua_settop(L, 3); 
    luaL_error(L, "field or property %s does not exist", lua_tostring(L, 2));
    return 1;
}

static int vptr_index_event(lua_State* L)
{    
    lua_pushlightuserdata(L, &vptr);
    lua_rawget(L, 1);                                   // stack: t key u
    lua_replace(L, 1);                                 // stack: u key
    lua_pushvalue(L, 1);                                // stack: u key u

    while (lua_getmetatable(L, -1) != 0)
    {
        lua_remove(L, -2);                              // stack: u key mt
        lua_pushvalue(L, 2);                            // stack: u key mt key
        lua_rawget(L, -2);                              // stack: u key mt value        

        if (!lua_isnil(L, -1))
        {
            return 1;
        }
        
        lua_pop(L, 1);        
        lua_pushlightuserdata(L, &gettag);          
        lua_rawget(L, -2);                              //stack: u key mt tget

        if (lua_istable(L, -1))
        {
            lua_pushvalue(L, 2);                        //stack: obj key mt tget key
            lua_rawget(L, -2);                          //stack: obj key mt tget value 

            if (lua_isfunction(L, -1))
            {
                lua_pushvalue(L, 1);
                lua_call(L, 1, 1);
                return 1;
            }
        }

        lua_settop(L, 3);
    }

    lua_settop(L, 2);
    lua_pushnil(L);
    return 1;
}

static int vptr_newindex_event(lua_State* L)
{    
    lua_pushlightuserdata(L, &vptr);
    lua_rawget(L, 1);                                   // stack: t key v u    
    lua_getmetatable(L, -1);

    while (lua_istable(L, -1))                          // stack: u k v mt
    {           
        lua_pushlightuserdata(L, &settag);
        lua_rawget(L, -2);                              // stack: u k v mt tset

        if (lua_istable(L, -1))
        {
            lua_pushvalue(L, 2);
            lua_rawget(L, -2);                          // stack: u k v mt tset func

            if (lua_isfunction(L, -1))
            {
                lua_pushvalue(L, 4);
                lua_pushvalue(L, 3);
                lua_call(L, 2, 0);
                return 0;
            }

            lua_pop(L, 1);                              // stack: t k v mt tset 
        }

        lua_pop(L, 1);                                  // stack: t k v mt 

        if (lua_getmetatable(L, -1) == 0)               // stack: t k v mt mt
        {
            lua_pushnil(L);
        }

        lua_remove(L, -2);                              // stack: t k v mt             
    }

    lua_settop(L, 3);    
    return 1;
}

LUALIB_API bool tolua_isvptrtable(lua_State* L, int index)
{    
    lua_pushlightuserdata(L, &vptr);
    lua_rawget(L, index);
    bool flag = lua_isnil(L, -1) ? false : true;
    lua_pop(L, 1);
    return flag;
}

LUALIB_API void tolua_setindex(lua_State* L)
{
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, class_index_event);
	lua_rawset(L, -3);
}

LUALIB_API void tolua_setnewindex(lua_State* L)
{
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, class_newindex_event);
	lua_rawset(L, -3);
}

LUALIB_API bool tolua_pushudata(lua_State* L, int index)
{
	lua_getref(L, LUA_RIDX_UBOX);			// stack: ubox
	lua_rawgeti(L, -1, index); 				// stack: ubox, obj

	if (!lua_isnil(L, -1))
	{
		lua_remove(L, -2); 					// stack: obj
		return true;
	}

	lua_pop(L, 2);
	return false;
}

LUALIB_API void tolua_pushnewudata(lua_State* L, int metaRef, int index)
{
	lua_getref(L, LUA_RIDX_UBOX);
	tolua_newudata(L, index);
	lua_getref(L, metaRef);
	lua_setmetatable(L, -2);
	lua_pushvalue(L, -1);
	lua_rawseti(L, -3, index);
	lua_remove(L, -2);	
}

LUALIB_API bool tolua_beginmodule(lua_State* L, const char* name)
{
    if (name != NULL)
    {                
        lua_pushstring(L, name);			//stack key
        lua_rawget(L, -2);					//stack value

        if (lua_isnil(L, -1))  
        {
            lua_pop(L, 1);
            lua_newtable(L);				//stack table
            lua_pushstring(L, name);		//stack table name
            lua_pushvalue(L, -2);			//stack table name table
            lua_rawset(L, -4);   			//stack table
            return true;
        }

        return lua_istable(L, -1);						
    }
    else
    {                
        lua_pushvalue(L, LUA_GLOBALSINDEX);
        return true;
    }                
}

static int class_new_event(lua_State* L)
{         
    if (!lua_istable(L, 1))
    {
        return luaL_typerror(L, 1, "table");        
    }

    int count = lua_gettop(L); 
    lua_pushvalue(L,1);  

    while (lua_getmetatable(L,-1))
    {   
        lua_remove(L,-2);                      
        lua_pushstring(L, "New");               
        lua_rawget(L,-2);    

        if (lua_isfunction(L,-1))
        {
            for (int i = 2; i <= count; i++)
            {
                lua_pushvalue(L, i);                    
            }

            lua_call(L, count - 1, 1);
            return 1;
        }

        lua_settop(L,3);
    }    

    return luaL_error(L,"attempt to perform ctor operation failed");    
}

LUALIB_API int tolua_beginclass(lua_State* L, const char* name, int baseType, int ref)
{
    int reference = ref;
    lua_pushstring(L, name);  
    lua_newtable(L);      

    if (ref == 0)        
    {
        lua_newtable(L);
        lua_pushvalue(L, -1);
        reference = luaL_ref(L, LUA_REGISTRYINDEX); 
    }
    else
    {
        lua_getref(L, reference);    
    }

    if (baseType != 0)
    {
        lua_getref(L, baseType);        
        lua_setmetatable(L, -2);
    }
           
    lua_pushlightuserdata(L, &tag);
    lua_pushnumber(L, 1);
    lua_rawset(L, -3);

    lua_pushstring(L, ".name");
    lua_pushvalue(L, -4);
    lua_rawset(L, -3);

    lua_pushstring(L, ".ref");
    lua_pushinteger(L, reference);
    lua_rawset(L, -3);

    lua_pushstring(L, "__call");
    lua_pushcfunction(L, class_new_event);
    lua_rawset(L, -3);

    tolua_setindex(L);
    tolua_setnewindex(L); 
    return reference;
}


LUALIB_API void tolua_endclass(lua_State* L)
{
	lua_setmetatable(L, -2);
    lua_rawset(L, -3);            
}

/*void settablename(lua_State* L, const char* label, const char* name)
{
    lua_pushstring(L, "name");
    char cname[128];
    int l1 = strlen(label);
    int l2 = strlen(name)    ;
    strncat(cname, label, 128 - l1);
    strncat(cname, name, 128 - l1 - l2);
    lua_pushstring(L, cname);
    lua_rawset(L, -3);
}*/

LUALIB_API int tolua_beginenum(lua_State* L, const char* name)
{
	lua_pushstring(L, name);  
    lua_newtable(L);              
    lua_newtable(L);

    lua_pushvalue(L, -1);
    int reference = luaL_ref(L, LUA_REGISTRYINDEX);            
    lua_pushlightuserdata(L, &tag);
    lua_pushnumber(L, 1);
    lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, enum_index_event);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, enum_newindex_event);
	lua_rawset(L, -3);	

	return reference;
}

LUALIB_API void tolua_endenum(lua_State* L)
{
	lua_setmetatable(L, -2);
    lua_rawset(L, -3);    
}

LUALIB_API void tolua_beginstaticclass(lua_State* L, const char* name)
{    
    lua_pushstring(L, name);  
    lua_newtable(L);              
    lua_pushvalue(L, -1);

    lua_pushlightuserdata(L, &tag);
    lua_pushnumber(L, 1);
    lua_rawset(L, -3);

    //lua_pushstring(L, ".name");
    //lua_pushvalue(L, -4);
    //lua_rawset(L, -3);

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, static_index_event);
    lua_rawset(L, -3);

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, static_newindex_event);
    lua_rawset(L, -3);      
}

LUALIB_API void tolua_endstaticclass(lua_State* L)
{
    lua_setmetatable(L, -2);
    lua_rawset(L, -3);    
}

LUALIB_API void tolua_constant(lua_State* L, const char* name, double value)
{
    lua_pushstring(L, name);
    lua_pushnumber(L, value);
    lua_rawset(L,-3);
}

LUALIB_API void tolua_function(lua_State* L, const char* name, lua_CFunction fn)
{
  	lua_pushstring(L, name);
  	lua_pushcfunction(L, fn);
  	lua_rawset(L, -3);
}

LUALIB_API void tolua_variable(lua_State* L, const char* name, lua_CFunction get, lua_CFunction set)
{                
    lua_pushlightuserdata(L, &gettag);
    lua_rawget(L, -2);

    if (!lua_istable(L, -1))
    {
        /* create .get table, leaving it at the top */
        lua_pop(L, 1);
        lua_newtable(L);        
        lua_pushlightuserdata(L, &gettag);
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);
    }

    lua_pushstring(L, name);
    lua_pushcfunction(L, get);
    lua_rawset(L, -3);                  /* store variable */
    lua_pop(L, 1);                      /* pop .get table */

    /* set func */
    if (set != NULL)
    {        
        lua_pushlightuserdata(L, &settag);
        lua_rawget(L, -2);

        if (!lua_istable(L, -1))
        {
            /* create .set table, leaving it at the top */
            lua_pop(L, 1);
            lua_newtable(L);            
            lua_pushlightuserdata(L, &settag);
            lua_pushvalue(L, -2);
            lua_rawset(L, -4);
        }

        lua_pushstring(L, name);
        lua_pushcfunction(L, set);
        lua_rawset(L, -3);                  /* store variable */
        lua_pop(L, 1);                      /* pop .set table */
    }
}

LUALIB_API int toluaL_ref(lua_State* L)
{
	int stackPos = abs_index(L, -1);	
	lua_getref(L, LUA_RIDX_FIXEDMAP);
	lua_pushvalue(L, stackPos);
	lua_rawget(L, -2);

	if (!lua_isnil(L, -1))
	{
		int ref = (int)lua_tointeger(L, -1);
		lua_pop(L, 3);
		return ref;
	}
	else
	{
		lua_pushvalue(L, stackPos);
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_pushvalue(L, stackPos);
		lua_pushinteger(L, ref);
		lua_rawset(L, -4);
		lua_pop(L, 3);
		return ref;
	}
}

LUALIB_API void toluaL_unref(lua_State* L, int reference)
{
	lua_getref(L, LUA_RIDX_FIXEDMAP);
	lua_getref(L, reference);
	lua_pushnil(L);
	lua_rawset(L, -3);
	luaL_unref(L, LUA_REGISTRYINDEX, reference);
	lua_pop(L, 1);
}

LUA_API lua_State* tolua_getmainstate(lua_State* L1)
{
	lua_rawgeti(L1, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
	lua_State* L = lua_tothread(L1, -1);
	lua_pop(L1, 1);
	return L;
}

LUA_API int tolua_getvaluetype(lua_State* L, int stackPos)
{
	stackPos = abs_index(L, stackPos);
	lua_getref(L, LUA_RIDX_CHECKVALUE);
	lua_pushvalue(L, stackPos);
	lua_call(L, 1, 1);
	int ret = (int)luaL_checknumber(L, -1);
	lua_pop(L, 1);
	return ret;
}

LUALIB_API bool tolua_createtable(lua_State* L, const char* path, int szhint)
{
	const char *e = NULL;
	lua_pushvalue(L, LUA_GLOBALSINDEX);						//stack _G

	do 
	{
	  e = strchr(path, '.');
	  if (e == NULL) e = path + strlen(path);
	  lua_pushlstring(L, path, e - path);					//stack _G key
	  lua_rawget(L, -2);									//stack _G value
	  int type = lua_type(L, -1);

	  if (type == LUA_TNIL) 
	  {  
	    lua_pop(L, 1); 										//stack _G
	    lua_createtable(L, 0, (*e == '.' ? 1 : szhint));	//stack _G table
	    lua_pushlstring(L, path, e - path);					//stack _G table name
	    lua_pushvalue(L, -2);								//stack _G table name table
	    lua_settable(L, -4);  								//stack _G table
	  }
	  else if (type != LUA_TTABLE) 
	  {  
	    lua_pop(L, 2);  
	    return false;  
	  }

	  lua_remove(L, -2);  									//stack table
	  path = e + 1;
	} while (*e == '.');

	return true;
}

LUALIB_API bool tolua_pushluatable(lua_State* L, const char* path)
{
	const char *e = NULL;
	lua_pushvalue(L, LUA_GLOBALSINDEX);	

	do 
	{
	  e = strchr(path, '.');
	  if (e == NULL) e = path + strlen(path);
	  lua_pushlstring(L, path, e - path);
	  lua_rawget(L, -2);

	  if (!lua_istable(L, -1))
	  {  
	    lua_pop(L, 2); 
	    return false;
	  }

	  lua_remove(L, -2);  
	  path = e + 1;
	} while (*e == '.');

	return true;
}

LUALIB_API const char* tolua_typename(lua_State* L, int lo)
{
    int tag = lua_type(L,lo);
    
    if (tag == LUA_TNONE)
    {
		lua_pushstring(L,"[no object]");
    }
    else if (tag != LUA_TUSERDATA && tag != LUA_TTABLE)
    {
        lua_pushstring(L, lua_typename(L,tag));
    }
    else if (tag == LUA_TUSERDATA)
    {
        if (!lua_getmetatable(L,lo))
        {
            lua_pushstring(L, lua_typename(L,tag));
        }
        else
        {
            lua_pushstring(L, ".name");
            lua_rawget(L, -2);

            if (!lua_isstring(L,-1))
            {
                lua_pop(L,1);
                lua_pushstring(L,"[undefined]");
            }
        }
    }
    else  //is table
    {
        lua_pushvalue(L,lo);        

        if (!lua_getmetatable(L,lo))
        {
            lua_pop(L,1);
            lua_pushstring(L,"table");
        }
        else
        {
            lua_pushstring(L, ".name");
            lua_rawget(L, -2);

            lua_pushstring(L,"class ");
            lua_insert(L,-2);
            lua_concat(L,2);
        }
    }

    return lua_tostring(L, -1);
}

LUALIB_API int tolua_getmetatableref(lua_State* L, int pos)
{
	int ref = 0;
	lua_getmetatable(L, pos);

    if (!lua_isnil(L, -1))
    {
        lua_pushstring(L, ".ref");
        lua_rawget(L, -2);	

        if (lua_isnumber(L, -1))
        {
            ref = (int)lua_tointeger(L, -1);
        }

        lua_pop(L, 2);
    }

    return ref;
}

static lua_State* getthread(lua_State *L, int *arg) 
{
    if (lua_isthread(L, 1)) 
    {
        *arg = 1;
        return lua_tothread(L, 1);
    }
    else 
    {
        *arg = 0;
        return L;  
    }
}

static int traceback(lua_State *L) 
{
    int arg;
    lua_State* L1 = getthread(L, &arg);
    const char* msg = lua_tostring(L, arg + 1);

    if (msg == NULL && !lua_isnoneornil(L, arg + 1))  
    {
        lua_pushvalue(L, arg + 1);  
    }
    else 
    {
        int level = (int)luaL_optinteger(L, arg + 2, (L == L1) ? 1 : 0);
#ifdef LUAJIT_VERSION            
        luaL_traceback(L, L1, msg, level);
#else        
        lua_getref(L, LUA_RIDX_TRACEBACK);
        lua_pushthread(L1);
        lua_pushvalue(L, arg + 1);
        lua_pushnumber(L, level + 1);
        lua_call(L, 3, 1);   
#endif            
    }     

    return 1;
}

LUALIB_API int tolua_beginpcall(lua_State* L, int reference)
{	
    lua_pushcfunction(L, traceback);    
	int top = lua_gettop(L);
	lua_getref(L, reference);
	return top;
}

LUALIB_API void tolua_pushtraceback(lua_State* L)
{
	lua_pushcfunction(L, traceback);
}

LUALIB_API int tolua_require(lua_State* L, const char* fileName)
{
    int top = lua_gettop(L);
    lua_pushcfunction(L, traceback);    
    lua_getref(L, LUA_RIDX_REQUIRE);
    lua_pushstring(L, fileName);    
    return lua_pcall(L, 1, -1, top + 1);
}

/*LUALIB_API bool tolua_checkluaslot(lua_State* L, int stackPos, int* func, int* table)
{
    lua_pushstring(L, "__call");      
    lua_rawget(L, stackPos);

    if (lua_isfunction(L, -1))
    {
        *func = toluaL_ref(L);        
    }
    else
    {
        lua_pop(L, 1);
        return false;
    }

    lua_pushvalue(L, stackPos);
    *table = toluaL_ref(L);    
    return true;
}*/

static int do_operator (lua_State* L, const char* op)
{
    if (lua_isuserdata(L,1))
    {        
        lua_pushvalue(L,1);  

        while (lua_getmetatable(L,-1))
        {   
            lua_remove(L,-2);                      
            lua_pushstring(L,op);               
            lua_rawget(L,-2);    

            if (lua_isfunction(L,-1))
            {
                lua_pushvalue(L,1);
                lua_pushvalue(L,2);
                lua_call(L,2,1);
                return 1;
            }

            lua_settop(L,3);
        }
    }

    luaL_error(L,"attempt to perform operation %s failed", op);
    return 0;
}

static int class_add_event (lua_State* L)
{
    return do_operator(L,"op_Addition");
}

static int class_sub_event (lua_State* L)
{
    return do_operator(L,"op_Subtraction");
}

static int class_mul_event (lua_State* L)
{
    return do_operator(L,"op_Multiply");
}

static int class_div_event (lua_State* L)
{
    return do_operator(L,"op_Division");
}

static int class_equals_event (lua_State* L)
{
    return do_operator(L,"op_Equality");
}


#ifdef _WIN32
double tolua_timegettime()
{
	FILETIME ft;
	double t;
	GetSystemTimeAsFileTime(&ft);
	/* Windows file time (time since January 1, 1601 (UTC)) */
	t = ft.dwLowDateTime / 1.0e7 + ft.dwHighDateTime*(4294967296.0 / 1.0e7);
	/* convert to Unix Epoch time (time since January 1, 1970 (UTC)) */
	return (t - 11644473600.0);
}
#else
double tolua_timegettime()
{
	struct timeval v;
	gettimeofday(&v, (struct timezone *) NULL);
	/* Unix Epoch time (time since January 1, 1970 (UTC)) */
	return v.tv_sec + v.tv_usec / 1.0e6;
}
#endif

static int tolua_gettime(lua_State* L)
{
	lua_pushnumber(L, tolua_timegettime());
	return 1;
}

static int tolua_bnd_setpeer(lua_State* L) 
{
    // stack: userdata, table
    if (!lua_isuserdata(L, -2)) 
    {
        return luaL_error(L, "Invalid argument #1 to setpeer: userdata expected.");        
    }

    if (lua_isnil(L, 2)) 
    {
        lua_pop(L, 1);
        lua_pushvalue(L, TOLUA_NOPEER);
        lua_setfenv(L, -2);
    }        
    else
    {
        lua_pushvalue(L, 2);                //stack: u p p
        lua_setfenv(L, -3);                 //stack: u p
        lua_newtable(L);                    //stack: u p vt        
        
        lua_pushlightuserdata(L, &vptr);
        lua_pushvalue(L, 1);
        lua_rawset(L, -3);

        lua_pushvalue(L, 1);
        lua_rawseti(L, -2, 1);

        lua_getref(L, LUA_RIDX_VPTR);       //stack: u p vt mt
        lua_setmetatable(L, -2);            //stack: u p vt

        lua_pushstring(L, "base");          //stack: u p vt "base"
        lua_pushvalue(L, -2);               //stack: u p vt "base" vt
        lua_rawset(L, 2);                   //stack: p t vt    
        lua_pop(L, 1);
    }

    return 0;
};

static int tolua_bnd_getpeer(lua_State* L) 
{    
    lua_getfenv(L, -1);

    if (lua_rawequal(L, -1, TOLUA_NOPEER)) 
    {
        lua_pop(L, 1);
        lua_pushnil(L);
    };

    return 1;
};

static int tolua_bnd_getfunction(lua_State* L)
{
    lua_pushvalue(L, 1);                            // stack: obj key obj

    while (lua_getmetatable(L, -1) != 0)            // stack: obj key mt
    {            
        lua_remove(L, -2);                          // stack: obj key mt

        lua_pushvalue(L, 2);                        // stack: obj key mt key
        lua_rawget(L, -2);                          // stack: obj key mt value        

        if (!lua_isfunction(L, -1))
        {
            return 1;
        }
        else
        {
            lua_pop(L, 1);
        }
        
        lua_pushlightuserdata(L, &gettag);          
        lua_rawget(L, -2);                          //stack: obj key mt tget

        if (lua_istable(L, -1))
        {
            lua_pushvalue(L, 2);                    //stack: obj key mt tget key
            lua_rawget(L, -2);                      //stack: obj key mt tget value 

            if (lua_isfunction(L, -1))
            {                                           
                return 1;
            }
        }
            
        lua_settop(L, 3);                           //stack: obj key mt
    }

    lua_settop(L, 2);
    lua_pushnil(L);
    return 1;
}

static int tolua_bnd_type (lua_State* L)
{
    tolua_typename(L,lua_gettop(L));
    return 1;
}

static int tolua_initgettable(lua_State* L)
{
    if (!lua_istable(L, 1))
    {        
        return luaL_typerror(L, 1, "table");         
    }

    lua_newtable(L);
    lua_pushlightuserdata(L, &gettag);
    lua_pushvalue(L, -2);    
    lua_rawset(L, 1);
    return 1;
}

static int tolua_initsettable(lua_State* L)
{
    if (!lua_istable(L, 1))
    {        
        return luaL_typerror(L, 1, "table");         
    }

    lua_newtable(L);
    lua_pushlightuserdata(L, &settag);    
    lua_pushvalue(L, -2);    
    lua_rawset(L, 1);
    return 1;
}


LUALIB_API bool tolua_isint64(lua_State* L, int pos)
{
    if (lua_type(L, pos) == LUA_TNUMBER)
    {
        return true;
    }

    int64_t* p = (int64_t*)lua_touserdata(L, pos);

    if (p != NULL)
    {
        if (lua_getmetatable(L, pos))
        {            
            lua_getref(L, LUA_RIDX_INT64); 
            int equal = lua_rawequal(L, -1, -2);
            lua_pop(L, 2);  

            if (equal)  
            {
                return true;
            }
        }
    }

    return false;
}

LUALIB_API void tolua_pushint64(lua_State* L, int64_t n)
{
    if (toluaflags & FLAG_INT64)    
    {
        lua_pushinteger(L, (lua_Integer)n);
    }
    else
    {
        int64_t* p = (int64_t*)lua_newuserdata(L, sizeof(int64_t));
        *p = n;
        lua_getref(L, LUA_RIDX_INT64);
        lua_setmetatable(L, -2);            
    }
}

LUALIB_API int64_t tolua_toint64(lua_State* L, int pos)
{
    int64_t n = 0;
    int type = lua_type(L, pos);

    switch(type) 
    {
        case LUA_TNUMBER:         
            n = (int64_t)lua_tointeger(L, pos);        
            break;    
        case LUA_TSTRING: 
            n = atoll(lua_tostring(L, pos));
            break;
        case LUA_TUSERDATA:     
            n = *(int64_t*)lua_touserdata(L, pos);            
            break;    
        default:
            break;
    }

    return n;
}

static int64_t tolua_checkint64(lua_State* L, int pos)
{
    int64_t n = 0;
    int type = lua_type(L, pos);
    
    switch(type)
    {
        case LUA_TNUMBER:
            n = (int64_t)lua_tointeger(L, pos);
            break;
        case LUA_TSTRING:
            n = atoll(lua_tostring(L, pos));
            break;
        case LUA_TUSERDATA:
            n = *(int64_t*)lua_touserdata(L, pos);
            break;
        default:
            return luaL_typerror(L, pos, "int64");
    }
    
    return n;
}

static int _int64add(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);
    tolua_pushint64(L, lhs + rhs);
    return 1;
}

static int _int64sub(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);
    tolua_pushint64(L, lhs - rhs);
    return 1;
}


static int _int64mul(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);
    tolua_pushint64(L, lhs * rhs);
    return 1;    
}

static int _int64div(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);
    tolua_pushint64(L, lhs / rhs);
    return 1;
}

static int _int64mod(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);

    if (rhs == 0) 
    {
        return luaL_error(L, "mod by zero");
    }

    tolua_pushint64(L, lhs % rhs);
    return 1;
}

static int _int64unm(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);        
    tolua_pushint64(L, -lhs);
    return 1;
}

static int _int64pow(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);    
    int64_t rhs = tolua_checkint64(L, 2);
    int64_t ret;
    
    if (rhs > 0)
    {
        ret = pow(lhs, rhs);
    }
    else if (rhs == 0)
    {
        ret = 1;
    }
    else
    {
        char temp[64];
        sprintf(temp, "%"PRId64, rhs);    
        return luaL_error(L, "pow by nagtive number %s", temp);
    }

    tolua_pushint64(L, ret);
    return 1;
}

static int _int64eq(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1);     
    int64_t rhs = *(int64_t*)lua_touserdata(L, 2);     
    lua_pushboolean(L, lhs == rhs);    
    return 1;
}

static int _int64equals(lua_State* L)
{
    int64_t lhs = tolua_checkint64(L, 1);
    int64_t rhs = tolua_checkint64(L, 2);
    lua_pushboolean(L, lhs == rhs);
    return 1;
}

static int _int64lt(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1); 
    int64_t rhs = tolua_checkint64(L, 2);
    lua_pushboolean(L, lhs < rhs);
    return 1;
}

static int _int64le(lua_State* L)
{
    int64_t lhs = *(int64_t*)lua_touserdata(L, 1); 
    int64_t rhs = tolua_checkint64(L, 2);
    lua_pushboolean(L, lhs <= rhs);
    return 1;
}

static int _int64tostring(lua_State* L)
{    
    if (!tolua_isint64(L, 1))
    {
        return luaL_typerror(L, 1, "int64");
    }

    int64_t n = tolua_toint64(L, 1);    
    char temp[64];
    sprintf(temp, "%"PRId64, n);    
    lua_pushstring(L, temp);
    return 1;
}

static int _int64tonum2(lua_State* L)
{
    if (!tolua_isint64(L, 1))
    {
        return luaL_typerror(L, 1, "int64");
    }

    int64_t n = tolua_toint64(L, 1);

    if (n != 0)
    {
        int64_t high = n >> 32;        
        int64_t low = n & 0xFFFFFFFF;
        lua_pushnumber(L, low);
        lua_pushnumber(L, high);
    }
    else
    {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
    }

    return 2;
}

static int _int64tofixed(lua_State* L)
{
    int64_t n = *(int64_t*)lua_touserdata(L, 1);    
    lua_pushlstring(L, (char*)&n, 8);
    return 1;
}

static int _int64fromfixed(lua_State* L)
{
    int64_t* n = (int64_t*)lua_touserdata(L, 1);    
    size_t len = 0;
    const char* str = lua_tolstring(L, 2, &len);

    if (len > 8) 
    {
        return luaL_error(L, "The string (length = %d) is not an fixed64 string", len);
    }

    int i = 0;
    uint64_t n64 = 0;

    for (i=0; i < len; i++) 
    {
        n64 |= (uint64_t)(str[i] << (i*8));
    }
        
    *n = (int64_t)n64;
    return 1;
}

static int _int64new(lua_State* L)
{
    int64_t n = 0;
    int type = lua_type(L, 1);

    if (type == LUA_TSTRING)
    {        
        const char* str = lua_tostring(L, 1);        
        n = atoll(str);                       
    }
    else if (type == LUA_TNUMBER)
    {
        int64_t n1 = (int64_t)luaL_checknumber(L, 1);
        int64_t n2 = (int64_t)lua_tonumber(L, 2);
        n = n1 + (n2 << 32);
    }

    tolua_pushint64(L, n);
    return 1;
}


void tolua_openint64(lua_State* L)
{        
    lua_newtable(L);      
    lua_pushvalue(L, -1);
    lua_setglobal(L, "int64");

    lua_pushstring(L, "__add"),
    lua_pushcfunction(L, _int64add);
    lua_rawset(L, -3);

    lua_pushstring(L, "__sub"),
    lua_pushcfunction(L, _int64sub);
    lua_rawset(L, -3);

    lua_pushstring(L, "__mul"),
    lua_pushcfunction(L, _int64mul);
    lua_rawset(L, -3);

    lua_pushstring(L, "__div"),
    lua_pushcfunction(L, _int64div);
    lua_rawset(L, -3);

    lua_pushstring(L, "__mod"),
    lua_pushcfunction(L, _int64mod);
    lua_rawset(L, -3);

    lua_pushstring(L, "__unm"),
    lua_pushcfunction(L, _int64unm);
    lua_rawset(L, -3);

    lua_pushstring(L, "__pow"),
    lua_pushcfunction(L, _int64pow);
    lua_rawset(L, -3);    

    lua_pushstring(L, "__tostring");
    lua_pushcfunction(L, _int64tostring);
    lua_rawset(L, -3);      

    lua_pushstring(L, "tostring");
    lua_pushcfunction(L, _int64tostring);
    lua_rawset(L, -3);     

    lua_pushstring(L, "__eq");
    lua_pushcfunction(L, _int64eq);
    lua_rawset(L, -3);  

    lua_pushstring(L, "__lt");
    lua_pushcfunction(L, _int64lt);
    lua_rawset(L, -3); 

    lua_pushstring(L, "__le");
    lua_pushcfunction(L, _int64le);
    lua_rawset(L, -3);     

    lua_pushstring(L, ".name");
    lua_pushstring(L, "int64");
    lua_rawset(L, -3);

    lua_pushstring(L, "new");
    lua_pushcfunction(L, _int64new);
    lua_rawset(L, -3);       

    lua_pushstring(L, "equals");
    lua_pushcfunction(L, _int64equals);
    lua_rawset(L, -3);     

    lua_pushstring(L, "tonum2");
    lua_pushcfunction(L, _int64tonum2);
    lua_rawset(L, -3);       

    /*lua_pushstring(L, "tofixed");
    lua_pushcfunction(L, _int64tofixed);
    lua_rawset(L, -3);  

    lua_pushstring(L, "fromfixed");
    lua_pushcfunction(L, _int64fromfixed);
    lua_rawset(L, -3);*/     

    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_rawset(L, -3);    

    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_INT64);     
}

void tolua_openvptr(lua_State* L)
{
    lua_newtable(L);        

    lua_pushstring(L, "__index");
    lua_pushcfunction(L, vptr_index_event);
    lua_rawset(L, -3);  

    lua_pushstring(L, "__newindex");
    lua_pushcfunction(L, vptr_newindex_event);
    lua_rawset(L, -3);  

    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_VPTR); 
}

static const struct luaL_reg tolua_funcs[] = 
{
	{ "gettime", tolua_gettime },
	{ "typename", tolua_bnd_type },
	{ "setpeer", tolua_bnd_setpeer},
	{ "getpeer", tolua_bnd_getpeer},
    { "getfunction", tolua_bnd_getfunction},
    { "initset", tolua_initsettable},
    { "initget", tolua_initgettable},
    { "int64", _int64new},        
	{ NULL, NULL }
};

void tolua_setluabaseridx(lua_State* L)
{
	for (int i = 1; i <= 64; i++)
	{
		lua_pushinteger(L, i);
		lua_rawseti(L, LUA_REGISTRYINDEX, i);
	}

	lua_pushthread(L);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    lua_getglobal(L, "require");
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_REQUIRE);    
}

void tolua_opentraceback(lua_State* L)
{
    lua_getglobal(L, "debug");
    lua_pushstring(L, "traceback");
    lua_rawget(L, -2);
    lua_pushvalue(L, -1);
    lua_setfield(L, LUA_GLOBALSINDEX, "traceback");            
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_TRACEBACK);
    lua_pop(L, 1);    
}

void tolua_openubox(lua_State* L)
{
	lua_newtable(L);
	lua_newtable(L);            
	lua_pushstring(L, "__mode");
	lua_pushstring(L, "v");
	lua_rawset(L, -3);
	lua_setmetatable(L, -2);            
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UBOX);	
}

void tolua_openfixedmap(lua_State* L)
{
	lua_newtable(L);
	//lua_newtable(L);
	//lua_pushstring(L, "__mode");
	//lua_pushstring(L, "k");
	//lua_rawset(L, -3);
	//lua_setmetatable(L, -2);    	
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_FIXEDMAP);		
}

//对于下列读取lua 特定文件需要判空报错
void tolua_openvaluetype(lua_State* L)
{
	lua_getglobal(L, "GetLuaValueType");
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_CHECKVALUE);	
}

void tolua_openluavec3(lua_State* L)
{
	lua_getglobal(L, "Vector3");

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "Vector3 not exists or loaded");
        return;
    }

	lua_pushstring(L, "New");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKVEC3);	
	lua_pushstring(L, "Get");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKVEC3);	
	lua_pop(L, 1);
}

void tolua_openluavec2(lua_State* L)
{
	lua_getglobal(L, "Vector2");

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "Vector2 not exists or loaded");
        return;
    }

	lua_pushstring(L, "New");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKVEC2);	
	lua_pushstring(L, "Get");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKVEC2);	
	lua_pop(L, 1);
}

void tolua_openluavec4(lua_State* L)
{
	lua_getglobal(L, "Vector4");

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "Vector4 not exists or loaded");
        return;
    }

	lua_pushstring(L, "New");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKVEC4);	
	lua_pushstring(L, "Get");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKVEC4);	
	lua_pop(L, 1);
}

void tolua_openluaclr(lua_State* L)
{
	lua_getglobal(L, "Color");

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "Color not exists or loaded");
        return;
    }

	lua_pushstring(L, "New");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKCLR);	
	lua_pushstring(L, "Get");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKCLR);	
	lua_pop(L, 1);
}

void tolua_openluaquat(lua_State* L)
{
	lua_getglobal(L, "Quaternion");

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "Quaternion not exists or loaded");
        return;
    }

	lua_pushstring(L, "New");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKQUAT);	
	lua_pushstring(L, "Get");
	lua_rawget(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKQUAT);	
	lua_pop(L, 1);
}

void tolua_openlualayermask(lua_State* L)
{
    lua_getglobal(L, "LayerMask");   

    if (!lua_istable(L, 1))
    {        
        luaL_error(L, "LayerMask not exists or loaded");
        return;
    }

    lua_pushstring(L, "New");
    lua_rawget(L, -2);
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_PACKLAYERMASK);   
    lua_pushstring(L, "Get");
    lua_rawget(L, -2);
    lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_UNPACKLAYERMASK); 
    lua_pop(L, 1);
}

LUALIB_API int tolua_openlualibs(lua_State* L)
{
	tolua_openvaluetype(L);
	tolua_openluavec3(L);
	tolua_openluavec2(L);
	tolua_openluavec4(L);
	tolua_openluaclr(L);
	tolua_openluaquat(L);   
    tolua_openlualayermask(L);
    return 0;
}

static int mathf_ispoweroftwo(lua_State* L)
{
    lua_Integer mask = luaL_checkinteger(L, 1);
    bool flag = (mask & (mask-1)) == 0 ? true : false;
    lua_pushboolean(L, flag);
    return 1;
}

inline lua_Integer NextPowerOfTwo(lua_Integer v)
{
    v -= 1;
    v |= v >> 16;
    v |= v >> 8;
    v |= v >> 4;
    v |= v >> 2;
    v |= v >> 1;
    return v + 1;
}

static int mathf_nextpoweroftwo(lua_State* L)
{
    lua_Integer v = luaL_checkinteger(L, 1);
    v = NextPowerOfTwo(v);
    lua_pushinteger(L, v);
    return 1;
}

static int mathf_closestpoweroftwo(lua_State* L)
{
    lua_Integer v = luaL_checkinteger(L, 1);
    lua_Integer nextPower = NextPowerOfTwo (v);
    lua_Integer prevPower = nextPower >> 1;    

    if (v - prevPower < nextPower - v)
    {
        lua_pushinteger(L, prevPower);        
    }
    else
    {    
        lua_pushinteger(L, nextPower);
    }

    return 1;
}

static int mathf_gammatolinearspace(lua_State* L)
{
    lua_Number value = luaL_checknumber(L, 1);

    if (value <= 0.04045f)
    {
        value /= 12.92f;
    }
    else if (value < 1.0f)
    {    
        value = pow((value + 0.055f)/1.055f, 2.4f);
    }
    else
    {
        value = pow(value, 2.4f);
    }

    lua_pushnumber(L, value);
    return 1;    
}

static int mathf_lineartogammaspace (lua_State* L)
{
    lua_Number value = luaL_checknumber(L, 1);

    if (value <= 0.0F)
    {    
        value = 0;
    }
    else if (value <= 0.0031308F)
    {
        value *= 12.92f;
    }
    else if (value <= 1.0f)
    {
        value = 1.055f * powf(value, 0.41666f) - 0.055f;
    }
    else
    {
        value = powf(value, 0.41666f);
    }

    lua_pushnumber(L, value);
    return 1;
}

static int mathf_normalize(lua_State* L)
{
    float x = (float)lua_tonumber(L, 1);
    float y = (float)lua_tonumber(L, 2);
    float z = (float)lua_tonumber(L, 3);

    float len = sqrt(x * x + y * y + z * z);
    
    if (len == 1)
    {
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        lua_pushnumber(L, z);
    }
    else if (len > 1e-5)
    {
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        lua_pushnumber(L, z);
    }
    else    
    {
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
        lua_pushnumber(L, 0);
    }

    return 3;
}

static const struct luaL_reg tolua_mathf[] = 
{
    { "NextPowerOfTwo", mathf_nextpoweroftwo },
    { "ClosestPowerOfTwo", mathf_closestpoweroftwo },
    { "IsPowerOfTwo", mathf_ispoweroftwo},
    { "GammaToLinearSpace", mathf_gammatolinearspace},
    { "LinearToGammaSpace", mathf_lineartogammaspace},
    { "Normalize", mathf_normalize},
    { NULL, NULL }
};


LUALIB_API void tolua_openlibs(lua_State* L)
{   
    luaL_openlibs(L);   
    int top = lua_gettop(L);    

    tolua_setluabaseridx(L);
    tolua_opentraceback(L);
    tolua_openubox(L);
    tolua_openfixedmap(L);    
    tolua_openint64(L);
    tolua_openvptr(L);

    luaL_register(L, "Mathf", tolua_mathf);     
    luaL_register(L, "tolua", tolua_funcs);    

    lua_getglobal(L, "tolua");

    lua_pushstring(L, "gettag");
    lua_pushlightuserdata(L, &gettag);
    lua_rawset(L, -3);

    lua_pushstring(L, "settag");
    lua_pushlightuserdata(L, &settag);
    lua_rawset(L, -3);

    lua_settop(L,top);
}

LUALIB_API void tolua_setflag(int bit, bool flag)
{
    if (flag)
    {
        toluaflags |= bit;    
    }
    else
    {
        toluaflags &= ~bit;    
    }
}

LUALIB_API bool tolua_getflag(int bit)
{
    return toluaflags & bit ? true : false;
}



