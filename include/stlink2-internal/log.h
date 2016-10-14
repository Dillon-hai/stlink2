/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */

/**
 * @file stlink2-internal/log.h
 */
#ifndef STLINK2_INTERNAL_LOG_H_
#define STLINK2_INTERNAL_LOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stlink2.h>

#define STLINK2_LOG_DEFAULT_LEVEL STLINK2_LOGLEVEL_QUIET
#define STLINK2_LOG_DEFAULT_FILE  NULL

#define STLINK2_LOG(level, dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_##level, __FILE__, __LINE__, __func__, dev, format, ##__VA_ARGS__)
#define STLINK2_LOG_WRITE(level, dev, format, ...) \
	stlink2_log(STLINK2_LOGLEVEL_##level, NULL, 0, NULL, dev, format, ##__VA_ARGS__)

void stlink2_log(enum stlink2_loglevel, const char *file, unsigned int line,
		 const char *function, stlink2_t dev, const char *format, ...);

#endif /* STLINK2_INTERNAL_LOG_H_ */
