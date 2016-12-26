#include <stlink2.h>

void stlink2_cmd_probe(void)
{
	stlink2_context_t ctx = stlink2_init();
	stlink2_devs_t devs;

	stlink2_probe(ctx, &devs);

	if (devs.len) {
		printf("Found %zu stlink2 programmer%c\n", devs.len, (devs.len == 1) ? '\0' : 's');

		for (size_t n = 0; n < devs.len; n++) {
			stlink2_t dev = stlink2_open(ctx, devs.serial[n]);
			if (!dev)
				return;

			printf("\n  serial: %s\n", stlink2_get_serial(dev));
			printf("    name: %s\n", stlink2_get_name(dev));
			printf(" version: %s\n", stlink2_get_version(dev));

			stlink2_close(&dev);
		}
	} else {
		printf("No stlink2 programmer found");
	}

	stlink2_exit(&ctx);
}

void stlink2_cmd_version(void)
{
	printf("%s\n", STLINK2_VERSION);
}

int main(const int argc, const char *argv[])
{
	bool cmd_version = false;
	bool cmd_help = false;
	bool cmd_probe = false;

	stlink2_flag_set_cmd_name("stlink2");
	stlink2_flag_set_usage("command [arguments]");

	stlink2_flag_bool(&cmd_version, "version", "print stlink2 version");
	stlink2_flag_bool(&cmd_help, "help", "print command help");
	stlink2_flag_bool(&cmd_probe, "probe", "probe all stlink2 programmers");

	stlink2_flag_parse(argc, argv);

	if (cmd_help)
		stlink2_flag_usage();
	else if (cmd_version)
		stlink2_cmd_version();
	else if (cmd_probe)
		stlink2_cmd_probe();

	return 0;
}
