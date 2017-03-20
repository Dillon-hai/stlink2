#include <stlink2.h>
#include <string.h>
#ifdef STLINK2_HAS_LUA
#include <stlink2/lua.h>
#include <lauxlib.h>
#include <lualib.h>
#endif

void stlink2_dev_set_env(stlink2_t dev)
{
	stlink2_log_set_filename(dev, getenv("STLINK2_LOGFILE"));
	stlink2_log_set_level_str(dev, getenv("STLINK2_LOGLEVEL"));
}

void stlink2_cmd_probe(void)
{
	stlink2_context_t ctx = stlink2_init();
	stlink2_devs_t devs;

	stlink2_probe(ctx, &devs);

	if (devs.len) {
		printf("Found %zu stlink2 programmer%c\n", devs.len, (devs.len == 1) ? '\0' : 's');

		for (size_t n = 0; n < devs.len; n++) {
			stlink2_t dev = stlink2_open(ctx, devs.serial[n]);
			stlink2_dev_set_env(dev);
			if (!dev)
				return;

			printf("\n  serial: %s\n", stlink2_get_serial(dev));
			printf("      name: %s\n", stlink2_get_name(dev));
			printf("   version: %s\n", stlink2_get_version(dev));

			stlink2_close(&dev);
		}
	} else {
		printf("No stlink2 programmers found\n");
	}

	stlink2_exit(&ctx);
}

void stlink2_cmd_version(void)
{
	printf("%s\n", STLINK2_VERSION);
}

#ifdef STLINK2_HAS_LUA
void stlink2_cmd_script(const char *filename)
{
	lua_State *lua = luaL_newstate();
	luaL_openlibs(lua);

	luaopen_stlink2(lua);
	(void)luaL_dofile(lua, filename);

	lua_close(lua);
}
#endif

int main(const int argc, const char *argv[])
{
	bool cmd_version = false;
	bool cmd_help = false;
	bool cmd_probe = false;
#ifdef STLINK2_HAS_LUA
	bool cmd_script = false;
#endif

	stlink2_flag_set_cmd_name("stlink2");
	stlink2_flag_set_usage("command [arguments]");

	stlink2_flag_bool(&cmd_version, "version", "print stlink2 version");
	stlink2_flag_bool(&cmd_help, "help", "print command help");
	stlink2_flag_bool(&cmd_probe, "probe", "probe all stlink2 programmers");
#ifdef STLINK2_HAS_LUA
	stlink2_flag_bool(&cmd_script, "script", "run stlink2 lua script");
#endif

	stlink2_flag_parse(argc, argv);

	if (cmd_version)
		stlink2_cmd_version();
	else if (cmd_probe)
		stlink2_cmd_probe();
#ifdef STLINK2_HAS_LUA
	else if (cmd_script && argc == 3)
		stlink2_cmd_script(argv[2]);
#endif

	return 0;
}
