#include <stlink2/flash.h>

void stlink2_flash_erase(stlink2_t dev)
{
	(void)dev;
}

void stlink2_flash_unlock(stlink2_t dev)
{
	(void)dev;
}

void stlink2_flash_lock(stlink2_t dev)
{
	(void)dev;
}

bool stlink2_flash_is_locked(stlink2_t dev)
{
	(void)dev;
	return true;
}

uint32_t stlink2_flash_read_cr(stlink2_t dev)
{
	(void)dev;
	return 0;
}

void stlink2_flash_write_cr(stlink2_t dev, uint32_t val)
{
	(void)dev;
	(void)val;
}

void stlink2_flash_set_programming(stlink2_t dev, bool enable)
{
	(void)dev;
	(void)enable;
}

void stlink2_flash_set_mass_erase(stlink2_t dev, bool enable)
{
	(void)dev;
	(void)enable;
}

void stlink2_flash_set_program_size(stlink2_t dev, enum stlink2_flash_program_size size)
{
	(void)dev;
	(void)size;
}
