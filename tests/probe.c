#include <stdio.h>
#include <stlink2.h>

int main(void)
{
	stlink2_context_t ctx;
	stlink2_devs_t devs;

	ctx = stlink2_init();
	stlink2_probe(ctx, &devs);

	if (devs.len == 0) {
		printf("no stlink2 devices found\n");
		stlink2_exit(&ctx);
		return 0;
	}

	printf("found %zu stlink2 devices\n", devs.len);
	for (size_t n = 0; n < devs.len; n++) {
		printf("\tserial: %s\n", devs.serial[n]);
	}

	stlink2_exit(&ctx);
}
