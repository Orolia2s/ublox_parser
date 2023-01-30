#pragma once

/**
 * @file serial.h
 * Manipulate a serial port connection
 */

#include "serial_options.h"

#include <stdint.h> // uint*_t

typedef struct serial_port serial_port_t;

/**
 * Serial port handler, similar to FILE.
 */
struct serial_port
{
	int              file_descriptor;
	serial_options_t options;
	uint8_t          opened      :1;
	uint8_t          got_options :1;
};

serial_port_t serial_open(const char* port_name);
void          serial_close(serial_port_t* port);
bool          serial_get_options(serial_port_t* port);

bool          serial_make_raw(serial_port_t* port);

void serial_print_input_modes(const struct serial_input_modes* modes);
void serial_print_control_modes(const struct serial_control_modes* modes);
bool serial_print_config(serial_port_t* port);

/**
 * Use the RAII with serial ports.
 *
 * In a situation where one wants to declare a serial port variable
 * on the stack in the local scope, this "typedef" can be used for
 * the port to be closed automatically when the variable goes out
 * of scope.
 *
 * It means this "typedef" can only be used like this :
 * @code{.c}
 * {
 *     Serial port = serial_open(...);
 *     ...
 * } // <- The port will be closed at that point
 * @endcode
 */
#define Serial __attribute__((cleanup(serial_close))) serial_port_t

/**
 * Fill the options structure if needed
 */
#define serial_ensure_options(PORT) \
	do { if (!(PORT)->got_options) serial_get_options(PORT); } while (0)
