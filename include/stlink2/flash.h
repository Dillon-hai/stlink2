/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_FLASH_H_
#define STLINK2_FLASH_H_

#include <stdint.h>
#include <stdbool.h>

enum stlink2_flash_program_size {
	STLINK2_FLASH_PROGRAM_SIZE_1BYTE,
	STLINK2_FLASH_PROGRAM_SIZE_2BYTE,
	STLINK2_FLASH_PROGRAM_SIZE_4BYTE,
	STLINK2_FLASH_PROGRAM_SIZE_8BYTE
};

typedef struct stlink2 *stlink2_t;

void stlink2_flash_erase(stlink2_t dev);
void stlink2_flash_unlock(stlink2_t dev);
void stlink2_flash_lock(stlink2_t dev);

/**
 * Read MCU flash lock status (CR_LOCK bit from CR)
 */
bool stlink2_flash_is_locked(stlink2_t dev);

/**
 * Read MCU flash busy status (SR_BSY bit from SR)
 */
bool stlink2_flash_is_busy(stlink2_t dev);

/**
 * Read MCU flash control register
 */
uint32_t stlink2_flash_read_cr(stlink2_t dev);

/**
 * Write MCU flash control register
 */
void stlink2_flash_write_cr(stlink2_t dev, uint32_t val);

/**
 * Read MCU flash Status Register
 */
uint32_t stlink2_flash_read_sr(stlink2_t dev);

void stlink2_flash_set_programming(stlink2_t dev, bool enable);
void stlink2_flash_set_mass_erase(stlink2_t dev, bool enable);
void stlink2_flash_set_program_size(stlink2_t dev, enum stlink2_flash_program_size size);

#endif /* STLINK2_FLASH_H_ */
