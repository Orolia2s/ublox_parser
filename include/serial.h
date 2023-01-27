#pragma once

/**
 * @file serial.h
 * Manipulate a serial port connection
 */

#include <termios.h> // for the union :/

#include <stdint.h> // uint*_t

/**
* Verbose alternative to termios.c_cflag
*/
struct serial_control_modes
{
	uint32_t _unused        :4;
	uint32_t character_size :2; /**< Number of bits used by a single character. */
	uint32_t stop_bit       :1; /**< Set two stop bits, rather than one. */
	uint32_t read           :1; /**< Enable receiver. */
	/** Enable parity generation on output and parity checking for input. */
	uint32_t enable_parity  :1;
	/** If set, then parity for input and output is odd; otherwise even parity is used. */
	uint32_t odd_parity     :1;
	/** Lower modem control lines after last process closes the device (hang up). */
	uint32_t hupcl          :1;
	uint32_t is_local       :1; /**< Ignore modem control lines. */
};

/**
 * Possible values of @ref serial_control_modes::character_size
 */
enum character_size
{
	character_size_5 = 0,
	character_size_6 = 1,
	character_size_7 = 2,
	character_size_8 = 3
};

typedef union serial_options serial_options_t;
union serial_options
{
	struct termios termios;
	struct
	{
		struct serial_control_modes input;
		struct serial_control_modes output;
		struct serial_control_modes control;
	};
};

int ublox_open_serial_port(const char* port_name);
