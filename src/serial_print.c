#include "serial.h"

#include <ft_prepro/tools.h>
#include <ft_prepro/color.h>

#include <stdio.h>  // printf

#define WIDTH 30

#ifndef LOG_USE_COLOR
# define PRINT_BOOL(MODES, FIELD) \
	printf("    %-*s: %s\n", WIDTH, PP_STR(FIELD), bool2str(MODES->FIELD));
#else
# define PRINT_BOOL(MODES, FIELD) \
	printf("    %s%-*s: %s%s\n", \
	       MODES->FIELD ? COLOR(BOLD) : COLOR(DIM), \
	       WIDTH, PP_STR(FIELD), bool2str(MODES->FIELD), COLOR(NORMAL));
#endif

#define INPUT_FIELDS \
	ignore_break, signal_break, discard_invalid_chars, mark_errors, \
	parity_check, strip_8th_bit, map_nl_to_cr, ignore_cr, map_cr_to_nl, \
	map_upper_to_lower, enable_start_stop_out, any_can_restart_output, \
	enable_start_stop_in, ring_bell_when_full, is_utf8

#define OUTPUT_BOOLS enable_processing, map_lower_to_upper

#define CONTROL_BOOLS \
	two_stop_bits, read, enable_parity, odd_parity, hang_up, is_local

#define LOCAL_FIELDS \
	enable_signals, canonical, echo, echo_erasure, echo_kill, echo_nl, \
	disable_flush, tostop, enable_processing

static inline const char* bool2str(bool b)
{
	return b ? "true" : "false";
}

void serial_print_input_modes(const struct serial_input_modes* modes)
{
	printf("  input_modes:\n");
	FOR(EACH(INPUT_FIELDS), PRINT_BOOL, modes);
}

void serial_print_output_modes(const struct serial_output_modes* modes)
{
	printf("  output_modes:\n");
	FOR(EACH(OUTPUT_BOOLS), PRINT_BOOL, modes);
}

void serial_print_control_modes(const struct serial_control_modes* modes)
{
	printf("  control_modes:\n");
	printf("    %-*s: %i\n", WIDTH, "character_size", 5 + modes->character_size);
	FOR(EACH(CONTROL_BOOLS), PRINT_BOOL, modes);
}

void serial_print_local_modes(const struct serial_local_modes* modes)
{
	printf("  local_modes:\n");
	FOR(EACH(LOCAL_FIELDS), PRINT_BOOL, modes);
}

bool serial_print_config(serial_port_t* port)
{
	serial_ensure_options(port);
	printf("serial_port_options:\n");
	serial_print_input_modes(&port->options.input);
	serial_print_output_modes(&port->options.output);
	serial_print_control_modes(&port->options.control);
	serial_print_local_modes(&port->options.local);
	printf("  baudrate:\n");
	printf("    %-*s: %li\n", WIDTH, "input",  serial_extract_baudrate(port->options.input_speed));
	printf("    %-*s: %li\n", WIDTH, "output", serial_extract_baudrate(port->options.output_speed));
	printf("---\n");
	return true;
}
