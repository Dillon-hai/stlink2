#include <stlink2/utils/ihex.h>

int main(void) {
	struct stlink2_ihex_rec rec;
	const char *ihex = ":10001000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0\r\n";

	stlink2_ihex_readline(&rec, ihex);
	stlink2_ihex_rec_dump(&rec);
}
