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

/*
 * Flagset contains a number of flags,
 * and is populated wth argc / argv with the
 * remaining arguments.
 *
 * In the event of an error the error union
 * is populated with either the flag or the
 * associated argument.
 */
struct stlink2_flag_set {
	const char *usage;
	int nflags;
	struct stlink2_flag flags[STLINK2_FLAGS_MAX];
	int argc;
	const char *argv[STLINK2_FLAGS_MAX_ARGS];
	union {
		struct stlink2_flag *flag;
		const char *arg;
	} error;
};

void stlink2_flag_int(int *value, const char *name, const char *help);
void stlink2_flag_bool(bool *value, const char *name, const char *help);
void stlink2_flag_string(const char **value, const char *name, const char *help);

/**
 * Parse commandline arguments from OS
 */
void stlink2_flag_parse(int argc, const char *argv[], const char *version);

#endif /* STLINK2_UTIL_FLAG_H_ */
