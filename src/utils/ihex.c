/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */

/**
 * @file src/utils/ihex.c
 */

#include <stlink2/utils/ihex.h>
#include <stlink2/utils/hexstr.h>

#include <stdio.h>
#include <stdlib.h>

void stlink2_ihex_rec_dump(struct stlink2_ihex_rec *rec)
{
	printf(" size: %zu\n", rec->size);
	printf(" addr: 0x%04x\n", rec->addr);
	printf("rtype: %d\n", rec->rtype);
	printf(" data: ");
	for (size_t n = 0; n < rec->size; n++)
		printf("%02x ", rec->data[n]);
	printf("\n");
	printf("checksum: %02x\n", rec->checksum);
}

static uint8_t stlink2_ihex_rec_checksum(const struct stlink2_ihex_rec *rec)
{
	uint8_t checksum = 0;

	checksum += rec->size;
	checksum += rec->addr;
	checksum += rec->rtype;

	for (size_t n = 0; n < rec->size; n++)
		checksum += rec->data[n];

	return 1 + (~checksum);
}

/**
 * hex2int
 * take a hex string and convert it to a 32bit number (max 8 hex digits)
 */
static uint32_t stlink2_ihex_hex2bin(const char *hex, const size_t size)
{
	uint32_t val = 0;
	const size_t _size = size * 2;

	for (size_t n = 0; n < _size; n++) {
		char c = hex[n];

		if (c == '\0')
			break;

		if (c >= '0' && c <= '9')
			c = c - '0';
		else if (c >= 'a' && c <= 'f')
			c = c - 'a' + 10;
		else if (c >= 'A' && c <= 'F')
			c = c - 'A' + 10;

		val = (val << 4) | (c & 0xf);
	}

	return val;
}

void stlink2_ihex_readline(struct stlink2_ihex_rec *rec, const char *line)
{
	rec->size = 0;

	if (line[0] != ':')
		return;

	rec->size  = stlink2_ihex_hex2bin(line + STLINK2_IHEX_OFFSET_SIZE,  1);
	rec->addr  = stlink2_ihex_hex2bin(line + STLINK2_IHEX_OFFSET_ADDR,  2);
	rec->rtype = stlink2_ihex_hex2bin(line + STLINK2_IHEX_OFFSET_RTYPE, 1);

	rec->data = malloc(rec->size);
	if (!rec->data)
		return;

	stlink2_hexstr_to_bin(rec->data, rec->size, line + STLINK2_IHEX_OFFSET_DATA, rec->size * 2);
	stlink2_hexstr_to_bin(&rec->checksum, sizeof(rec->checksum), line + STLINK2_IHEX_OFFSET_DATA + (rec->size * 2), 1);
	if (rec->checksum != stlink2_ihex_rec_checksum(rec)) {
		free(rec->data);
		rec->data = NULL;
		rec->size = 0;
		rec->addr = 0;
		rec->rtype = 0;
	}
}
