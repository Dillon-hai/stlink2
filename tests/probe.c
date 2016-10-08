#include <stdlib.h>
#include <stlink2.h>

int main(void)
{
	stlink2_devs_t devs;

	stlink2_init();
	stlink2_probe(&devs);

	printf("found %zu stlink2 devices\n", devs.len);
	for (size_t n = 0; n < devs.len; n++) {
		printf("\tserial: %s\n", devs.serial[n]);
	}

	stlink2_exit();
}
