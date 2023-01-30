#include "serial.h"

#include <ft_prepro/tools.h>
#include <ft_prepro/color.h>
#include <log.h>
#include <termios.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>  // printf
#include <string.h> // strerror

#define WIDTH 30

#ifndef LOG_USE_COLOR
#define PRINT_BOOL(MODES, FIELD) printf("\t\t%-*s: %s\n", WIDTH, PP_STR(FIELD), bool2str(MODES->FIELD));
#else
#define PRINT_BOOL(MODES, FIELD) printf("\t\t%s%-*s: %s%s\n", MODES->FIELD ? COLOR(BOLD, WHITE) : COLOR(DIM, LIGHT_GREY), WIDTH, PP_STR(FIELD), bool2str(MODES->FIELD), COLOR(NORMAL));
#endif

#define INPUT_FIELDS ignore_break, signal_break, discard_invalid_chars, mark_errors, parity_check, strip_8th_bit, map_nl_to_cr, ignore_cr, map_cr_to_nl, map_upper_to_lower, enable_start_stop_out, any_can_restart_output, enable_start_stop_in, ring_bell_when_full, is_utf8
#define CONTROL_BOOLS two_stop_bits, read, enable_parity, odd_parity, hang_up, is_local

static inline const char* bool2str(bool b)
{
	return b ? "true" : "false";
}

void serial_print_input_modes(const struct serial_input_modes* modes)
{
	printf("\tinput_modes:\n");
	FOR(EACH(INPUT_FIELDS), PRINT_BOOL, modes);
}

void serial_print_control_modes(const struct serial_control_modes* modes)
{
	printf("\tcontrol_modes:\n");
	printf("\t\t%-*s: %i\n", WIDTH, "character_size", modes->character_size);
	FOR(EACH(CONTROL_BOOLS), PRINT_BOOL, modes);
}

bool serial_print_config(int port_fd)
{
	serial_options_t options;

	if (tcgetattr(port_fd, &options.termios) != 0)
	{
		log_error("Unable to get the attributes of the terminal: %s", strerror(errno));
		return false;
	}

	printf("serial_port_options:\n");
	serial_print_input_modes(&options.input);
	serial_print_control_modes(&options.control);
	return true;
}
