#include "serial.h"

#include <ft_prepro/tools.h>
#include <ft_prepro/color.h>

#include <ft_printf.h>

#define WIDTH 30

#ifndef LOG_USE_COLOR
#   define PRINT_BOOL(MODES, FIELD) \
        ft_printf("    %-*s: %B\n", WIDTH, PP_STR(FIELD), MODES->FIELD);
#else
#   define PRINT_BOOL(MODES, FIELD)                        \
        ft_printf("    %s%-*s: %B%s\n",                    \
                  MODES->FIELD ? COLOR(BOLD) : COLOR(DIM), \
                  WIDTH, PP_STR(FIELD),                    \
                  MODES->FIELD, COLOR(NORMAL));
#endif

#define INPUT_FIELDS \
	ignore_break, signal_break, discard_invalid_chars, mark_errors, \
	parity_check, strip_8th_bit, map_nl_to_cr, ignore_cr, map_cr_to_nl, \
	map_upper_to_lower, enable_start_stop_out, any_can_restart_output, \
	enable_start_stop_in, ring_bell_when_full, is_utf8

#define OUTPUT_BOOLS enable_processing, map_lower_to_upper, map_nl_to_crnl, \
	map_cr_to_nl, no_cr, nl_returns, use_fill, fill_is_del, vertical_tab_delay

#define CONTROL_BOOLS \
	two_stop_bits, read, enable_parity, odd_parity, hang_up, is_local

#define LOCAL_FIELDS \
	enable_signals, canonical, echo, echo_erasure, echo_kill, echo_nl, \
	disable_flush, tostop, enable_processing

#define CONTROL_CHARACTERS \
	interrupt, quit, erase, kill, end_of_file, timeout, minimum

static void serial_print_input_modes(const struct serial_input_modes* modes)
{
	ft_printf("  input_modes:\n");
	FOR(EACH(INPUT_FIELDS), PRINT_BOOL, modes);
}

static void serial_print_output_modes(const struct serial_output_modes* modes)
{
	ft_printf("  output_modes:\n");
	FOR(EACH(OUTPUT_BOOLS), PRINT_BOOL, modes);
}

static void serial_print_control_modes(const struct serial_control_modes* modes)
{
	ft_printf("  control_modes:\n");
	ft_printf("    %-*s: %i\n", WIDTH, "character_size", 5 + modes->character_size);
	FOR(EACH(CONTROL_BOOLS), PRINT_BOOL, modes);
}

static void serial_print_local_modes(const struct serial_local_modes* modes)
{
	ft_printf("  local_modes:\n");
	FOR(EACH(LOCAL_FIELDS), PRINT_BOOL, modes);
}

#define PRINT_UCHAR(CHARS, NAME) \
	ft_printf("    %-*s: %#.2hhx\n", WIDTH, PP_STR(NAME), CHARS->NAME);

static void serial_print_control_characters(union serial_control_characters* chars)
{
	ft_printf("  control_characters:\n");
	FOR(EACH(CONTROL_CHARACTERS), PRINT_UCHAR, chars);
}

/**
 * Output a YAML to stderr listing the status of the terminal options.
 */
bool serial_print_config(serial_port_t* port)
{
	serial_ensure_options(port);
	ft_printf("serial_port_options:\n");
	serial_print_input_modes(&port->options.input);
	serial_print_output_modes(&port->options.output);
	serial_print_control_modes(&port->options.control);
	serial_print_local_modes(&port->options.local);
	serial_print_control_characters(&port->options.control_characters);
	ft_printf("  baudrate:\n");
	ft_printf("    %-*s: %hhu\n", WIDTH, "line_discipline", port->options.line_discipline);
	ft_printf("    %-*s: %li\n", WIDTH, "input",  serial_decode_baudrate(port->options.input_speed));
	ft_printf("    %-*s: %li\n", WIDTH, "output", serial_decode_baudrate(port->options.output_speed));
	ft_printf("---\n");
	return true;
}
