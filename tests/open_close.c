#include <stdio.h>
#include <stlink2.h>

void dump_hex(const void * data, size_t size)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

static void test_open_close(stlink2_context_t ctx, const char *serial)
{
	stlink2_t dev;

	dev = stlink2_open(ctx, serial);
	if (!dev)
		return;

	uint32_t cpuid;
	uint16_t devid;

	stlink2_log_set_level(dev, STLINK2_LOGLEVEL_INFO);

	printf("  serial: %s\n",    stlink2_get_serial(dev));
	printf("    name: %s\n",    stlink2_get_name(dev));
	printf(" version: %s\n",    stlink2_get_version(dev));

	stlink2_get_mode(dev);
	stlink2_set_mode_swd(dev);
	stlink2_mcu_halt(dev);

	uint32_t pc;

	stlink2_read_reg(dev, STLINK2_CORTEXM_REG_PC, &pc);
	printf(" PC: 0x%08x\n", pc);
	stlink2_read_all_regs(dev);

	stlink2_mcu_init(dev);
	stlink2_read_reg(dev, STLINK2_CORTEXM_REG_PC, &pc);
	printf(" PC: 0x%08x\n", pc);

	uint32_t xpsr;
	stlink2_read_reg(dev, STLINK2_CORTEXM_REG_XPSR, &xpsr);
	printf(" xPSR: 0x%08x\n", xpsr);

	uint32_t control;
	stlink2_read_reg(dev, STLINK2_CORTEXM_REG_CONTROL, &control);
	printf(" CONTROL: 0x%08x\n", control);



	cpuid = stlink2_get_cpuid(dev);
	devid = stlink2_get_devid(dev);

	printf(" voltage: %f\n",    stlink2_get_target_voltage(dev));
	printf("   cpuid: %08x\n",  cpuid);
	printf("     partno: %03x (%s)\n", stlink2_cortexm_cpuid_get_partno(cpuid), stlink2_cortexm_cpuid_partno_str(cpuid));
	printf("  coreid: %08x\n",  stlink2_get_coreid(dev));
	printf("  chipid: %08x\n",  stlink2_get_chipid(dev));
	printf("   devid: %s (0x%03x)\n", stlink2_stm32_devid_str(devid), devid);
	printf("  flash size: %dKiB\n", stlink2_get_flash_size(dev));
	printf("  unique id: %s\n", stlink2_get_unique_id(dev, 0x1ff800d0)); /** @todo STM32L152 now... */

#ifdef BLA
	uint8_t *flash = malloc(1024);
	stlink2_read_mem(dev, 0x08000000, flash, 1024);
	dump_hex(flash, 1024);
#endif
#ifdef SEMIHOSTING
	while (true) {
		if (stlink2_get_status(dev) == STLINK2_STATUS_CORE_HALTED)
			stlink2_mcu_run(dev);
		stlink2_semihosting(dev);
		stlink2_msleep(1);
	}
#endif

	stlink2_mcu_run(dev);
	stlink2_close(&dev);
}

int main(void)
{
	stlink2_context_t ctx = stlink2_init();

	test_open_close(ctx, NULL);
	stlink2_exit(&ctx);
}
