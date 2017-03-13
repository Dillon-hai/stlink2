/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_H_
#define STLINK2_H_

#include <stdlib.h>
#include <stdint.h>

#include <stlink2/cortexm.h>

#define STLINK2_ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

enum stlink2_status {
	STLINK2_STATUS_UNKNOWN      = -1,
	STLINK2_STATUS_CORE_RUNNING = 0x80,
	STLINK2_STATUS_CORE_HALTED  = 0x81
};

enum stlink2_mode {
	STLINK2_MODE_UNKNOWN = -1,
	STLINK2_MODE_DFU     = 0x00,
	STLINK2_MODE_MASS    = 0x01,
	STLINK2_MODE_DEBUG   = 0x02
};

enum stlink2_swdclk {
	STLINK2_SWDCLK_4000KHZ = 0U,
	STLINK2_SWDCLK_1800KHZ = 1U,
	STLINK2_SWDCLK_1200KHZ = 2U,
	STLINK2_SWDCLK_950KHZ  = 3U,
	STLINK2_SWDCLK_480KHZ  = 7U,
	STLINK2_SWDCLK_240KHZ  = 15U,
	STLINK2_SWDCLK_125KHZ  = 31U,
	STLINK2_SWDCLK_100KHZ  = 40U,
	STLINK2_SWDCLK_50KHZ   = 79U,
	STLINK2_SWDCLK_25KHZ   = 158U,
	STLINK2_SWDCLK_15KHZ   = 265U,
	STLINK2_SWDCLK_5KHZ    = 798U
};

typedef struct stlink2_context *stlink2_context_t;
typedef struct stlink2 *stlink2_t;
typedef struct stlink2_devs {
	size_t len;
	size_t cap;
	const char **serial;
} stlink2_devs_t;

stlink2_context_t stlink2_init(void);
void stlink2_exit(stlink2_context_t *ctx);

void stlink2_probe(stlink2_context_t ctx, stlink2_devs_t *devlist);
stlink2_t stlink2_open(stlink2_context_t ctx, const char *serial);
void stlink2_reset(stlink2_t *dev);
void stlink2_close(stlink2_t *dev);

const char *stlink2_get_name(stlink2_t dev);
const char *stlink2_get_serial(stlink2_t dev);
const char *stlink2_get_version(stlink2_t dev);
float stlink2_get_target_voltage(stlink2_t dev);
enum stlink2_mode stlink2_get_mode(struct stlink2 *dev);
void stlink2_set_mode_swd(struct stlink2 *dev);
enum stlink2_status stlink2_get_status(struct stlink2 *dev);
void stlink2_mcu_halt(stlink2_t dev);
void stlink2_mcu_run(stlink2_t dev);
void stlink2_mcu_init(stlink2_t dev);
uint32_t stlink2_get_coreid(stlink2_t dev);
uint32_t stlink2_get_chipid(stlink2_t dev);
uint32_t stlink2_get_cpuid(stlink2_t dev);
uint16_t stlink2_get_devid(stlink2_t dev);
uint32_t stlink2_get_flash_size(stlink2_t dev);

void stlink2_read_mem(stlink2_t dev, uint32_t addr, void *data, size_t len);

void stlink2_read_all_regs(stlink2_t dev);
void stlink2_read_reg(stlink2_t dev, enum stlink2_cortexm_reg reg, uint32_t *val);
void stlink2_write_reg(stlink2_t dev, enum stlink2_cortexm_reg reg, uint32_t val);

#include <stlink2/log.h>
#include <stlink2/build.h>
#include <stlink2/stm32.h>
#include <stlink2/semihosting.h>

#include <stlink2/utils/flag.h>

#endif /* STLINK2_H_ */
