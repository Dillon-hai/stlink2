/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_INTERNAL_H_
#define STLINK2_INTERNAL_H_

#include <stdint.h>
#include <stlink2-internal/log.h>
#include <stlink2-internal/usb.h>

struct stlink2_context {
	libusb_context *usb;
};

struct stlink2 {
	char *serial;
	const char *name;
	struct {
		enum stlink2_loglevel level;
		FILE *fp;
	} log;
	struct {
		uint32_t coreid;
		uint32_t chipid;
		uint32_t cpuid;
		uint16_t flash_size;
		char *unique_id;
	} mcu;
	struct {
		uint8_t stlink;
		uint8_t jtag;
		uint8_t swim;
		char *version;
	} fw;
	struct {
		uint16_t pid;
		libusb_device_handle *dev;
		uint8_t rx_ep;
		uint8_t tx_ep;
		unsigned int timeout;
	} usb;
};

char *stlink2_strdup(const char *s);
void stlink2_read_debug16(struct stlink2 *dev, uint32_t addr, uint16_t *val);
void stlink2_read_debug32(struct stlink2 *dev, uint32_t addr, uint32_t *val);

#endif /* STLINK2_INTERNAL_H_ */
