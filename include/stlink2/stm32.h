/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_STM32_H_
#define STLINK2_STM32_H_

#include <stlink2.h>

enum stlink2_stm32_devid {
	STLINK2_STM32_DEVID_STM32F2XX          = 0x411,
	STLINK2_STM32_DEVID_STM32L1XX_CAT3_MED = 0x427,
	STLINK2_STM32_DEVID_STM32L0XX_CAT1     = 0x457
};

/* */
enum stlink2_stm32_revid {
	STLINK2_STM32_REVID_STM32F2XX_REV_A = 0x1000,
	STLINK2_STM32_REVID_STM32F2XX_REV_B = 0x2000,
	STLINK2_STM32_REVID_STM32F2XX_REV_Z = 0x1001,
	STLINK2_STM32_REVID_STM32F2XX_REV_Y = 0x2001,
	STLINK2_STM32_REVID_STM32F2XX_REV_X = 0x2003,

	STLINK2_STM32_REVID_STM32F4XX_REV_A = 0x1000,
	STLINK2_STM32_REVID_STM32F4XX_REV_Z = 0x1001,
	STLINK2_STM32_REVID_STM32F4XX_REV_Y = 0x1003,
	STLINK2_STM32_REVID_STM32F4XX_REV_1 = 0x1007,
	STLINK2_STM32_REVID_STM32F4XX_REV_3 = 0x2001
};

void stlink2_stm32_info(struct stlink2 *dev, char *buf, int buf_size);

#endif /* STLINK2_STM32_H_ */
