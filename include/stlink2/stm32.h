/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_STM32_H_
#define STLINK2_STM32_H_

#include <stlink2.h>

#define STLINK2_STM32_DEV(name, string, devid, flash_size_reg)
#define STLINK2_STM32_DEV_LIST \
	STLINK2_STM32_DEV(FXXX,      "STM32FXXX",                                 0x411, 0x1fff7a22) \
	STLINK2_STM32_DEV(F10XXLOWD, "STM32F10xx Low-density",                    0x412, 0x1ffff7e0) \
	STLINK2_STM32_DEV(F405XXF40, "STM32F405xx/F407xx/F415xx/F417xx",          0x413, 0x1fff7a22) \
	STLINK2_STM32_DEV(F10XXHIGH, "STM32F10xx High-density",                   0x414, 0x1ffff7e0) \
	STLINK2_STM32_DEV(L47XL48X,  "STM32L47x/L48x",                            0x415, 0x1fff75e0) \
	STLINK2_STM32_DEV(L0XX64K,   "STM32L0xx 64k",                             0x417, 0x1ff8007c) \
	STLINK2_STM32_DEV(F10XXCONN, "STM32F10xx Connectivity Line",              0x418, 0x1ffff7e0) \
	STLINK2_STM32_DEV(F42XXXF43, "STM32F42xxx/F43xxx",                        0x419, 0x1fff7a22) \
	STLINK2_STM32_DEV(F100XXLOW, "STM32F100xx Low/Medium density Value Line", 0x420, 0x1ffff7e0) \
	STLINK2_STM32_DEV(F446XX,    "STM32F446xx",                               0x421, 0x1fff7a22) \
	STLINK2_STM32_DEV(F302XBXCF, "STM32F302xB-xC/F303xB-xC/F358xx",           0x422, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F401XBC,   "STM32F401xB/C",                             0x423, 0x1fff7a22) \
	STLINK2_STM32_DEV(L03X32K,   "STM32L03x 32k",                             0x425, 0x1ff8007c) \
	STLINK2_STM32_DEV(L100XCL15, "STM32L100xC/L15xxC/L162xC",                 0x427, 0x1ff800cc) \
	STLINK2_STM32_DEV(F100XXHIG, "STM32F100xx High-density Value Line",       0x428, 0x1ffff7e0) \
	STLINK2_STM32_DEV(F10XXXLDE, "STM32F10xx XL-density",                     0x430, 0x1ffff7e0) \
	STLINK2_STM32_DEV(F411XCE,   "STM32F411xC/E",                             0x431, 0x1fff7a22) \
	STLINK2_STM32_DEV(F37XX,     "STM32F37xx",                                0x432, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F401XDE,   "STM32F401xD/E",                             0x433, 0x1fff7a22) \
	STLINK2_STM32_DEV(F469XF479, "STM32F469x/F479x",                          0x434, 0x1fff7a22) \
	STLINK2_STM32_DEV(L43X,      "STM32L43x",                                 0x435, 0x1fff75e0) \
	STLINK2_STM32_DEV(L15XXCXXD, "STM32L15xxC-xxD/L162xC-xD",                 0x436, 0x1ff800cc) \
	STLINK2_STM32_DEV(L15XXEL16, "STM32L15xxE/L162xE",                        0x437, 0x1ff800cc) \
	STLINK2_STM32_DEV(F303X4X6X, "STM32F303x4-x6-x8/F328xx/F334xx",           0x438, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F301X4X6X, "STM32F301x4-x6-x8/F302x4-x6-x8/F318xx",     0x439, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F051X4F05, "STM32F051x4/F051x6/F051x8/F030x8",          0x440, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F412XX,    "STM32F412xx",                               0x441, 0x1fff7a22) \
	STLINK2_STM32_DEV(F091XBXCF, "STM32F091xB-xC/F098xC/F030xC",              0x442, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F030X4F03, "STM32F030x4/F030x6",                        0x444, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F04XX,     "STM32F04xx",                                0x445, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F302XEF30, "STM32F302xE/F303xE/F398xx",                 0x446, 0x1ffff7cc) \
	STLINK2_STM32_DEV(L07X,      "STM32L07x/STM32L08x",                       0x447, 0x1ff8007c) \
	STLINK2_STM32_DEV(F072XXF07, "STM32F072xx/F078xx",                        0x448, 0x1ffff7cc) \
	STLINK2_STM32_DEV(F74XF75X,  "STM32F74x/F75x",                            0x449, 0x1ff0f442) \
	STLINK2_STM32_DEV(H7XX,      "STM32H7xx",                                 0x450, 0x00000000) \
	STLINK2_STM32_DEV(F76X,      "STM32F76x",                                 0x451, 0x1ff0f442) \
	STLINK2_STM32_DEV(F72XF73X,  "STM32F72x/F73x",                            0x452, 0x1ff07a22) \
	STLINK2_STM32_DEV(L01XL02X,  "STM32L01x/L02x",                            0x457, 0x1ff8007c) \
	STLINK2_STM32_DEV(F410XX,    "STM32F410xx",                               0x458, 0x1fff7a22) \
	STLINK2_STM32_DEV(L4X6,      "STM32L4x6",                                 0x461, 0x1fff75e0) \
	STLINK2_STM32_DEV(F4X3,      "STM32F4x3",                                 0x463, 0x1fff7a22)
#undef  STLINK2_STM32_DEV

/** STM32 family device ids */
enum stlink2_stm32_devid {
#define STLINK2_STM32_DEV(name, string, devid, flash_size_reg) \
	STLINK2_STM32_DEVID_##name = devid,
STLINK2_STM32_DEV_LIST
#undef STLINK2_STM32_DEV
};

const char *stlink2_stm32_devid_str(uint32_t devid);
uint32_t stlink2_stm32_flash_size_reg(uint32_t devid);

#endif /* STLINK2_STM32_H_ */
