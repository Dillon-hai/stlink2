/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_UTILS_IHEX_H_
#define STLINK2_UTILS_IHEX_H_

#include <stdlib.h>
#include <stdint.h>

enum stlink2_ihex_offset {
	STLINK2_IHEX_OFFSET_SIZE  = 1,
	STLINK2_IHEX_OFFSET_ADDR  = 3,
	STLINK2_IHEX_OFFSET_RTYPE = 7,
	STLINK2_IHEX_OFFSET_DATA  = 9
};

enum stlink2_ihex_rtype {
	STLINK2_IHEX_RTYPE_DATA              = 0,
	STLINK2_IHEX_RTYPE_EOF               = 1,
	STLINK2_IHEX_RTYPE_EXTENDED_SEG_ADDR = 2,
	STLINK2_IHEX_RTYPE_EXTENDED_LIN_ADDR = 4
};

struct stlink2_ihex_rec {
	size_t size;
	uint32_t addr;
	enum stlink2_ihex_rtype rtype;
	uint8_t *data;
	uint8_t checksum;
	struct stlink2_ihex_rec *next;
};

void stlink2_ihex_readline(struct stlink2_ihex_rec *rec, const char *line);
void stlink2_ihex_rec_dump(struct stlink2_ihex_rec *rec);

#endif /* STLINK2_UTILS_IHEX_H_ */
