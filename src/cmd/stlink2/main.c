#include <stlink2.h>

int main(int argc, const char *argv[], const char *env[])
{
	(void)env;

	stlink2_flag_parse(argc, argv, STLINK2_VERSION);

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
