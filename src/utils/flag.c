/*
 * Copyright 2016 Jerry Jacobs. All rights reserved.
 * Copyright 2015 TJ Holowaychuk (github.com/clibs/flag). All rights reserved.
 * Use of this source code is governed by the MIT
 * license that can be found in the LICENSE file.
 */
#include <stlink2/utils/flag.h>
#include <string.h>

static struct stlink2_flag_set g_flag_set;

static bool stlink2_flag_is_flag(const char *s)
{
	return strlen(s) > 2 && s[0] == '-' && s[1] == '-';
}

/*
 * Largest flag name in the set.
 */

static size_t stlink2_flag_set_largest_flag_size(struct stlink2_flag_set *self)
{
	size_t max = 0;

	for (int i = 0; i < self->nflags; ++i) {
		struct stlink2_flag *flag = &self->flags[i];
		size_t len = strlen(flag->name);
		if (len)
			max = len > max ? len : max;
	}

	return max;
}

void stlink2_flag_set_write_usage(struct stlink2_flag_set *self, FILE *fp, const char *name)
{
	fprintf(fp, "\nUsage: %s %s\n", name, self->usage);
	fprintf(fp, "\nOptions:\n");

	const size_t max = stlink2_flag_set_largest_flag_size(self);

	for (int i = 0; i < self->nflags; ++i) {
		const struct stlink2_flag *flag = &self->flags[i];

		fprintf(fp, "		--%-*s %s", (int)max+1, flag->name, flag->help);
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

void stlink2_flag_set_parse(struct stlink2_flag_set *self, int argc, const char **args)
{
	(void)self;

	for (int i = 0; i < argc; i++) {
		const char *arg = args[i];

		if (!stlink2_flag_is_flag(arg))
			continue;

	}
}

void stlink2_flag_set_bool(struct stlink2_flag_set *self, bool *value, const char *name, const char *help)
{
	(void)self;
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_int(int *value, const char *name, const char *help)
{
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_bool(bool *value, const char *name, const char *help)
{
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_string(const char **value, const char *name, const char *help)
{
	(void)value;
	(void)name;
	(void)help;
}

void stlink2_flag_parse(int argc, const char *argv[], const char *version)
{
	(void)argc;
	(void)version;

	bool show_version = false;
	bool show_help = false;

	stlink2_flag_set_bool(&g_flag_set, &show_version, "version", "Output version");
	stlink2_flag_set_bool(&g_flag_set, &show_help, "help", "Output help");

	stlink2_flag_set_write_usage(&g_flag_set, stdout, argv[0]);
}
