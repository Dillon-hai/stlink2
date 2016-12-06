#include <stlink2.h>

int main(const int argc, const char *argv[])
{
	bool show_version = false;
	bool show_help = false;
	bool cmd_probe = true;

	stlink2_flag_set_cmd_name("stlink2");
	stlink2_flag_set_usage("command [arguments]");

	stlink2_flag_bool(&show_version, "version", "print stlink2 version");
	stlink2_flag_bool(&show_help, "help", "print command help");
	stlink2_flag_bool(&cmd_probe, "probe", "probe all stlink2 programmers");

	stlink2_flag_parse(argc, argv);

	stlink2_context_t ctx = stlink2_init();
	stlink2_t dev;

	dev = stlink2_open(ctx, NULL);
	if (!dev)
		return -1;

	printf("  serial: %s\n", stlink2_get_serial(dev));
	printf("    name: %s\n", stlink2_get_name(dev));
	printf(" version: %s\n", stlink2_get_version(dev));

	stlink2_close(&dev);
	stlink2_exit(&ctx);

	return 0;
}
