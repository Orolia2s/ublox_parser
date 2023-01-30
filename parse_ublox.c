#include "serial.h"
#include "ublox.h"

#include <argp.h>
#include <fcntl.h>
#include <ft_prepro/tools.h>
#include <ft_printf.h>
#include <termios.h>

#include <stdbool.h>
#include <stdio.h>

const char*  argp_program_version     = "ublox_parser " PP_STR(VERSION);
const char*  argp_program_bug_address = "<antoine.gagniere@orolia2s.com>";
static char  doc[] = "Prints serial port configuration and ublox messages.";
static char  args_doc[] = "[PATH]";

static struct argp_option options[] = {
	{"passive", 'p', 0, OPTION_ARG_OPTIONAL, "Do not change the port configuration, only display it.", 1},
	{ 0 }
};

struct arguments
{
	bool is_passive;
};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
	struct arguments* arguments = state->input;
	switch (key)
	{
	case 'p': arguments->is_passive = true; break;
	case ARGP_KEY_ARG:
		if (arg)
		{
			Serial port = serial_open(arg);
			if (arguments->is_passive)
				serial_print_config(&port);
			else
				ublox_port_config(&port);
		}
		return 0;
	default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int ac, char** av)
{
	struct arguments arguments = { .is_passive = false };

	argp_parse(&argp, ac, av, 0, 0, &arguments);
	return 0;
}
