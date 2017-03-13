/*
 * Copyright 2017 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stlink2.h>

static stlink2_context_t g_stlink2;

static int l_version(lua_State *L)
{
	lua_pushstring(L, STLINK2_VERSION);
	return 1;
}

static int l_probe(lua_State *L)
{
	stlink2_devs_t devs;
	stlink2_probe(g_stlink2, &devs);
	/* TODO: probably we need to free the devs list after probe */

	if (!devs.len)
		return 0;

	lua_createtable(L, 0, (int)devs.len);

	for (size_t n = 0; n < devs.len; n++) {
		lua_pushinteger(L, n);
		lua_pushstring(L, devs.serial[n]);
		lua_settable(L, -3);
	}

	return 1;
}

/* stlink2_dev */

static stlink2_t l_dev_to_stlink2_t(lua_State *L)
{
	stlink2_t dev = NULL;

	if (luaL_getmetafield(L, -1, "__dev") == 0)
		return 0;
	if (lua_islightuserdata(L, 2))
		dev = lua_touserdata(L, 2);
	lua_pop(L, 2);

	return dev;
}

static int l_dev_halt(lua_State *L)
{
	stlink2_t dev = l_dev_to_stlink2_t(L);
	if (!dev)
		return 0;

	stlink2_mcu_halt(dev);	

	return 0;
}

static int l_dev_run(lua_State *L)
{
	stlink2_t dev = l_dev_to_stlink2_t(L);
	if (!dev)
		return 0;

	stlink2_mcu_halt(dev);	

	return 0;
}

#define STLINK2_LUA_FUNC_DECL(name, luatype, cfunction) \
	static int l_dev_##name(lua_State *L) \
	{ \
		stlink2_t dev = l_dev_to_stlink2_t(L); \
		if (!dev) { \
			return 0; \
		} \
		lua_push##luatype(L, cfunction(dev)); \
		return 1; \
	}

STLINK2_LUA_FUNC_DECL(serial,     string,  stlink2_get_serial)
STLINK2_LUA_FUNC_DECL(name,       string,  stlink2_get_name)
STLINK2_LUA_FUNC_DECL(version,    string,  stlink2_get_version)
STLINK2_LUA_FUNC_DECL(cpuid,      integer, stlink2_get_cpuid)
STLINK2_LUA_FUNC_DECL(devid,      integer, stlink2_get_devid)
STLINK2_LUA_FUNC_DECL(coreid,     integer, stlink2_get_coreid)
STLINK2_LUA_FUNC_DECL(chipid,     integer, stlink2_get_chipid)
STLINK2_LUA_FUNC_DECL(flash_size, integer, stlink2_get_flash_size)
STLINK2_LUA_FUNC_DECL(voltage,    number,  stlink2_get_target_voltage)

static int l_dev_partno(lua_State *L)
{
	stlink2_t dev = l_dev_to_stlink2_t(L);
	if (!dev) {
		return 0;
	}

	uint32_t cpuid = stlink2_get_cpuid(dev);

	lua_pushinteger(L, stlink2_cortexm_cpuid_get_partno(cpuid));
	return 1;
}

static int l_dev_read_reg(lua_State *L)
{
	stlink2_t dev = l_dev_to_stlink2_t(L);

	if (!dev)
		return 0;

	uint32_t val;
	stlink2_read_reg(dev, 0,&val);
	lua_pushinteger(L, val);
	return 1;
}

static int l_dev_gc(lua_State *L)
{
	stlink2_t dev = l_dev_to_stlink2_t(L);

	if (!dev)
		return 0;
	
	stlink2_close(&dev);

	return 0;
}

static const luaL_Reg l_dev_m[] = {
	{"serial",    l_dev_serial},
	{"name",      l_dev_name},
	{"version",   l_dev_version},
	{"voltage",   l_dev_voltage},
	{"cpuid",     l_dev_cpuid},
	{"coreid",    l_dev_coreid},
	{"chipid",    l_dev_chipid},
	{"devid",     l_dev_devid},
	{"partno",    l_dev_partno},
	{"halt",      l_dev_halt},
	{"run",       l_dev_run},
	{"flashSize", l_dev_flash_size},
	{"readReg",   l_dev_read_reg},
	{"__gc", l_dev_gc},
	{NULL, NULL}
};

static int l_open(lua_State *L)
{
	const char *serial = luaL_checkstring(L, 1);

	if (!serial)
		return 0;

	stlink2_t dev = stlink2_open(g_stlink2, serial);
	if (!dev)
		return 0;

	stlink2_set_mode_swd(dev);
	stlink2_mcu_init(dev);

	lua_newtable(L);
	luaL_getmetatable(L, "stlink2_dev");
	lua_pushliteral(L, "__dev");
	lua_pushlightuserdata(L, dev);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);

	return 1;
}

static const struct luaL_Reg stlink2[] = {
	{ "version", l_version },
	{ "probe",   l_probe },
	{ "open",    l_open },
	{ NULL, NULL }
};

int l_dev_register(lua_State *L)
{
	luaL_newmetatable(L, "stlink2_dev");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, l_dev_m, 0);
	lua_setglobal(L, "stlink2_dev");

	return 1;
}

void luaopen_stlink2(lua_State *L)
{
	luaL_newlib(L, stlink2);
	lua_setglobal(L, "stlink2");

	l_dev_register(L);

	g_stlink2 = stlink2_init();
}
