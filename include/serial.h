#pragma once

/**
 * @file serial.h
 * Manipulate a serial port connection
 */

#include "serial_options.h"

#include <ft_deque.h>

#include <sys/types.h> // ssize_t

#include <stdbool.h>
#include <stdint.h> // uint*_t

/** Might or might not be a struct. */
typedef struct serial_port serial_port_t;

/**
 * Serial port handler, similar to FILE.
 */
struct serial_port
{
	serial_options_t options;         /**< Terminal options */
	t_deque          buffer;          /**< Circular buffer */
	int              file_descriptor; /**< Underlying file */
	uint8_t          opened      :1;  /**< Is this file still open ? */
	uint8_t          got_options :1;  /**< Is @ref options filled ? */
};

serial_port_t serial_open(const char* port_name);
void          serial_close(serial_port_t* port);
bool          serial_get_options(serial_port_t* port);

bool          serial_make_raw(serial_port_t* port);
int64_t       serial_decode_baudrate(speed_t flag);
speed_t       serial_encode_baudrate(int64_t speed_in_bps);

bool          serial_print_config(serial_port_t* port);

ssize_t       serial_single_read(serial_port_t* port);
bool          serial_accumulate(serial_port_t* port, size_t n);

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

/**
@var serial_port::buffer
Good complexity for push_bash and pop_front.
No need to memmove remaining data to the front.
*/
