#include "serial.h"
#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"
#include "ublox_reader.h"

#include <argp.h>
#include <fcntl.h>           // open
#include <ft_prepro/tools.h> // PP_STR
#include <log.h>
#include <termios.h>

#include <stdbool.h>
#include <stdio.h>  // printf
#include <stdlib.h> // free

void ublox_printer(ublox_message_t* message)
{
	if (message->group == RXM && message->type == 0x13)
	{
		RAII(t_string)
			str = ublox_navigation_data_tostring((struct ublox_navigation_data*)message);
		printf("{%s}\n---\n", cstring(&str));
	}
	else if (message->group == MON && message->type == HW)
	{
		RAII(t_string)
			str = ublox_monitoring_hardware_tostring((struct ublox_monitoring_hardware*)message);
		printf("{%s}\n---\n", cstring(&str));
	}
	else if (message->group == MON && message->type == RF)
	{
		RAII(t_string)
			str = ublox_monitoring_rf_tostring((struct ublox_monitoring_rf*)message);
		printf("{%s}\n---\n", cstring(&str));
	}
	else
	{
		RAII(t_string) str = ublox_header_tostring(message);
		printf("{%s}\n---\n", cstring(&str));
	}
}

/**
 * Receive, parse and display all ublox messages found on the serial port.
 *
 * @param passive: When passive, do not change the port config, only display it
 * and try to read.
 * @param is_file: Do not try to access the terminal options
 */
void parse_ublox(const char* file_name, bool passive, bool is_file)
{
	serial_port_t port;

	if (is_file)
		port.file = file_open(file_name, O_RDONLY);
	else
	{
		port = serial_open(file_name);

		if (!passive && ublox_port_config(&port, 115200) == false)
			return ;
		serial_print_config(&port);

	}

	if (port.file.descriptor < 0)
		return ;

	Reader        reader = ublox_reader_init(&port.file);

	ublox_subscribe(&reader, ublox_printer);
	ublox_reader_loop(&reader);
}

const char* argp_program_version     = "ublox_parser " PP_STR(VERSION);
const char* argp_program_bug_address = "<antoine.gagniere@orolia2s.com>";
static char doc[]      = "Prints serial port configuration and ublox messages.";
static char args_doc[] = "[PATH]";

static struct argp_option options[] = {
    {"passive", 'p', 0, OPTION_ARG_OPTIONAL, "Do not change the port configuration, only display it.", 1},
    {"file", 'f', 0, OPTION_ARG_OPTIONAL, "Input is not a serial port but a file", 1},
    {0}
};

struct arguments
{
	bool is_passive;
	bool is_file;
};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
	struct arguments* arguments = state->input;
	switch (key)
	{
	case 'p': arguments->is_passive = true; break;
	case 'f': arguments->is_file = true; break;
	case ARGP_KEY_ARG:
		if (arg)
			parse_ublox(arg, arguments->is_passive, arguments->is_file);
		return 0;
	default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int                main(int arg_count, char** arg_values)
{
	struct arguments arguments = {.is_passive = false, .is_file = false};

	printf("---\n");
	argp_parse(&argp, arg_count, arg_values, 0, 0, &arguments);
	return 0;
}
