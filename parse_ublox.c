#include "serial.h"
#include "ublox.h"

#include <argp.h>
#include <fcntl.h>           // open
#include <ft_prepro/tools.h> // PP_STR
#include <log.h>
#include <termios.h>

#include <stdbool.h>
#include <stdio.h>  // printf
#include <stdlib.h> // free

/**
 * Receive, parse and display all ublox messages found on the serial port.
 *
 * @param passive: When passive, do not change the port config, only display it
 * and try to read.
 */
void parse_ublox(const char* port_name, bool passive)
{
	ublox_message_t* message;
	Serial           port = serial_open(port_name);

	if (port.file.descriptor < 0
	    || (!passive && ublox_port_config(&port, 115200) == false))
		return ;
	serial_print_config(&port);

	while ((message = ublox_next_message(&port)) != NULL)
	{
		if (message->class == RXM && message->type == 0x13)
		{
			RAII(t_string)
			str = ublox_navigation_data_tostring((struct ublox_navigation_data*)message);
			log_info("{%s}", cstring(&str));
		}
		else if (message->class == MON && message->type == HW)
		{
			RAII(t_string)
			str = ublox_monitoring_hardware_tostring((struct ublox_monitoring_hardware*)message);
			log_info("{%s}", cstring(&str));
		}
		else if (message->class == MON && message->type == RF)
		{
			RAII(t_string)
			str = ublox_monitoring_rf_tostring((struct ublox_monitoring_rf*)message);
			log_info("{%s}", cstring(&str));
		}
		else
		{
			// RAII(t_string) str = ublox_header_tostring(message);
			// log_info("{%s}", cstring(&str));
		}
		free(message);
	}

} /* <- port will be closed at this point */

const char* argp_program_version     = "ublox_parser " PP_STR(VERSION);
const char* argp_program_bug_address = "<antoine.gagniere@orolia2s.com>";
static char doc[]      = "Prints serial port configuration and ublox messages.";
static char args_doc[] = "[PATH]";

static struct argp_option options[] = {
    {"passive", 'p', 0, OPTION_ARG_OPTIONAL, "Do not change the port configuration, only display it.", 1},
    {0}
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
			parse_ublox(arg, arguments->is_passive);
		return 0;
	default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int                main(int arg_count, char** arg_values)
{
	struct arguments arguments = {.is_passive = false};

	argp_parse(&argp, arg_count, arg_values, 0, 0, &arguments);
	return 0;
}
