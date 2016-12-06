/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#ifndef STLINK2_UTIL_FLAG_H_
#define STLINK2_UTIL_FLAG_H_

#include <stdio.h>
#include <stdbool.h>

/*
 * Max flags that may be defined.
 */

#define STLINK2_FLAGS_MAX 8

/*
 * Max arguments supported for set->argv.
 */

#define STLINK2_FLAGS_MAX_ARGS 16

enum stlink2_flag_type {
	STLINK2_FLAG_TYPE_INT,
	STLINK2_FLAG_TYPE_BOOL,
	STLINK2_FLAG_TYPE_STRING
};

/*
 * Flag represents as single user-defined
 * flag with a name, help description,
 * type, and pointer to a value which
 * is replaced upon precense of the flag.
 */
struct stlink2_flag {
	const char *name;
	const char *help;
	enum stlink2_flag_type type;
	void *value;
};

void stlink2_flag_set_cmd_name(const char *name);
void stlink2_flag_set_usage(const char *usage);

void stlink2_flag_int(int *value, const char *name, const char *help);
void stlink2_flag_bool(bool *value, const char *name, const char *help);
void stlink2_flag_string(const char **value, const char *name, const char *help);

/**
 * Parse commandline arguments from OS
 */
void stlink2_flag_parse(int argc, const char *argv[]);

#endif /* STLINK2_UTIL_FLAG_H_ */
