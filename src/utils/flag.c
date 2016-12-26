/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Copyright 2015 TJ Holowaychuk (github.com/clibs/flag). All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/utils/flag.h>
#include <string.h>

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
	const char *cmd_name;
	const char *usage;
	size_t nflags;
	struct stlink2_flag flags[STLINK2_FLAGS_MAX];
	int argc;
	const char *argv[STLINK2_FLAGS_MAX_ARGS];
	union {
		struct stlink2_flag *flag;
		const char *arg;
	} error;
};

static struct stlink2_flag_set g_flag_set;

bool stlink2_flag_is_flag(const char *s)
{
	return strlen(s) > 1 && s[0] == '-';
}

/*
 * Largest flag name in the set.
 */

static size_t stlink2_flag_set_largest_flag_size(struct stlink2_flag_set *self)
{
	size_t max = 0;

	for (size_t i = 0; i < self->nflags; ++i) {
		struct stlink2_flag *flag = &self->flags[i];
		size_t len = strlen(flag->name);
		if (len)
			max = len > max ? len : max;
	}

	return max;
}

void stlink2_flag_set_write_usage(FILE *fp, struct stlink2_flag_set *self)
{
	fprintf(fp, "\nUsage:\n\n\t%s %s\n", self->cmd_name, self->usage);
	fprintf(fp, "\nThe commands are:\n\n");

	const size_t max = stlink2_flag_set_largest_flag_size(self);

	for (size_t i = 0; i < self->nflags; i++) {
		const struct stlink2_flag *flag = &self->flags[i];

		fprintf(fp, "	%-*s %s", (int)max+1, flag->name, flag->help);
		switch (flag->type) {
			case STLINK2_FLAG_TYPE_STRING:
				fprintf(fp, " (%s)", *(char **) flag->value);
				break;
			case STLINK2_FLAG_TYPE_INT:
				fprintf(fp, " (%d)", *(int *) flag->value);
				break;
			case STLINK2_FLAG_TYPE_BOOL:
				break;
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "\n");
}

struct stlink2_flag *stlink2_flag_set_get_by_name(struct stlink2_flag_set *self, const char *name)
{
	for (size_t n = 0; n < sizeof(self->flags)/sizeof(self->flags[0]); n++) {
		struct stlink2_flag *f = &self->flags[n];
		if (!f->name)
			continue;
		if (strcmp(name, f->name) == 0)
			return f;
	}

	return NULL;
}

void stlink2_flag_set_bool(struct stlink2_flag_set *self, bool *value, const char *name, const char *help)
{
	(void)value;

	for (size_t n = 0; n < sizeof(self->flags)/sizeof(self->flags[0]); n++) {
		struct stlink2_flag *f = &self->flags[n];
		if (f->name)
			continue;

		f->name  = name;
		f->help  = help;
		f->value = value;
		f->type  = STLINK2_FLAG_TYPE_BOOL;

		self->nflags++;
		break;
	}
}

void stlink2_flag_int(int *value, const char *name, const char *help)
{
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_bool(bool *value, const char *name, const char *help)
{
	stlink2_flag_set_bool(&g_flag_set, value, name, help);
}

void stlink2_flag_string(const char **value, const char *name, const char *help)
{
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_set_cmd_name(const char *name)
{
	g_flag_set.cmd_name = name;
}

void stlink2_flag_set_usage(const char *usage)
{
	g_flag_set.usage = usage;
}

void stlink2_flag_usage(void)
{
	stlink2_flag_set_write_usage(stdout, &g_flag_set);
}

void stlink2_flag_parse(const int argc, const char *argv[])
{
	if (argc < 2) {
		stlink2_flag_usage();
		return;
	}

	for (size_t n = 1; n < (size_t)argc; n++) {
		struct stlink2_flag *f = stlink2_flag_set_get_by_name(&g_flag_set, argv[n]);
		if (!f)
			continue;

		switch (f->type) {
			case STLINK2_FLAG_TYPE_STRING:
				//fprintf(fp, " (%s)", *(char **) flag->value);
				break;
			case STLINK2_FLAG_TYPE_INT:
				//fprintf(fp, " (%d)", *(int *) flag->value);
				break;
			case STLINK2_FLAG_TYPE_BOOL:
				*(bool *)f->value = true;
				break;
		}
	}
}
