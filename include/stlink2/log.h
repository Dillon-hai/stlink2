/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */

/**
 * @file stlink2/log.h
 */
#ifndef STLINK2_LOG_H_
#define STLINK2_LOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stlink2.h>

/**
 * Loglevels
 */
enum stlink2_loglevel {
	STLINK2_LOGLEVEL_QUIET,
	STLINK2_LOGLEVEL_ERROR,
	STLINK2_LOGLEVEL_WARN,
	STLINK2_LOGLEVEL_INFO,
	STLINK2_LOGLEVEL_DEBUG,
	STLINK2_LOGLEVEL_TRACE
};

/**
 * Set default loglevel
 */
void stlink2_log_set_default_level(stlink2_t ctx, enum stlink2_loglevel level);

/**
 * Set logfile for device
 * @param dev  Device
 * @param file File
 */
void stlink2_log_set_file(stlink2_t dev, FILE *file);

/**
 * Set effective loglevel
 * @param dev  Device
 * @param file File
 */
void stlink2_log_set_level(stlink2_t dev, enum stlink2_loglevel level);

#endif /* STLINK2_LOG_H_ */
