#pragma once

/**
 * @file serial.h
 * Manipulate a serial port connection
 */

#include <termios.h>

#include <stdbool.h>
#include <stdint.h> // uint*_t

/**
 * Verbose alternative to termios.c_iflag.
 * This section describes the terminal attribute flags that control fairly
 * low-level aspects of input processing: handling of parity errors, break
 * signals, flow control, and `RET` and `LFD` characters.
 */
struct serial_input_modes
{
	uint32_t ignore_break          :1; /**< Ignore break condition */
	uint32_t signal_break          :1; /**< Signal interrupt on break */
	uint32_t discard_invalid_chars :1; /**< Ignore characters with parity errors */
	uint32_t mark_errors           :1; /**< Mark parity and framing errors */
	uint32_t parity_check          :1; /**< Enable input parity check */
	uint32_t strip_8th_bit         :1; /**< Strip 8th bit off characters */
	uint32_t map_nl_to_cr          :1; /**< Map `\n` to `\r` on input */
	uint32_t ignore_cr             :1; /**< Ignore Carriage Returns (`\r`) */
	uint32_t map_cr_to_nl          :1; /**< Map `\r` to `\n` on input */
	/** Map uppercase characters to lowercase on input. */
	uint32_t map_upper_to_lower    :1;
	uint32_t enable_start_stop_out :1; /**< Enable start/stop output control */
	uint32_t any_can_restart_output:1; /**< Enable any character to restart output */
	uint32_t enable_start_stop_in  :1; /**< Enable start/stop input control */
	uint32_t ring_bell_when_full   :1; /**< Ring bell when input queue is full */
	uint32_t is_utf8               :1; /**< Input is UTF8 */
};

/**
 * Verbose alternative to termios.c_cflag
 *
 * This structures stores the terminal flags and fields that control parameters
 * usually associated with asynchronous serial data transmission.
 * These flags may not make sense for other kinds of terminal ports
 * (such as a network connection pseudo-terminal).
 */
struct serial_control_modes
{
	uint32_t _unused        :4;
	uint32_t character_size :2; /**< Number of bits used by a single character */
	uint32_t two_stop_bits  :1; /**< Set two stop bits, rather than one */
	uint32_t read           :1; /**< Enable receiver */
	/** Enable parity generation on output and parity checking for input. */
	uint32_t enable_parity  :1;
	/** If set, then parity for input and output is odd; otherwise even parity is used. */
	uint32_t odd_parity     :1;
	/** Lower modem control lines after last process closes the device (hang up). */
	uint32_t hang_up        :1;
	uint32_t is_local       :1; /**< Ignore modem control lines */
};

/**
 * Possible values of @ref serial_control_modes::character_size.
 *
 * Used to specify the number of data bits per byte.
 *
 * 8 data bits are almost universally used in newer applications. 5 or 7 bits
 * generally only make sense with older equipment such as teleprinters.
 */
enum character_size
{
	character_size_5 = 0, /**< for Baudot code */
	character_size_6 = 1, /**< rarely used */
	character_size_7 = 2, /**< for true ASCII */
	character_size_8 = 3  /**< for most kinds of data */
};

/** Verbose alternative ot struct termios */
typedef union serial_options serial_options_t;

/**
 * Holds a struct termios that can be modified using the verbose alternatives.
 */
union serial_options
{
	struct termios termios;
	struct
	{
		struct serial_input_modes   input;
		struct serial_control_modes output;
		struct serial_control_modes control;
	};
};

int  ublox_open_serial_port(const char* port_name);
bool serial_make_raw(int port_fd);

void serial_print_input_modes(const struct serial_input_modes* modes);
void serial_print_control_modes(const struct serial_control_modes* modes);
bool serial_print_config(int port_fd);

/** @struct serial_input_modes
Documentation source:
https://www.gnu.org/software/libc/manual/html_node/Input-Modes.html

@var serial_input_modes::parity_check
If this bit is set, input parity checking is enabled. If it is not set,
no checking at all is done for parity errors on input;
the characters are simply passed through to the application.

Parity checking on input processing is independent of whether parity
detection and generation on the underlying terminal hardware is enabled; see
@ref serial_control_modes. For example, you could clear the @ref parity_check
input mode flag and set the serial_control_modes::enable_parity control mode
flag to ignore parity errors on input, but still generate parity on output.

If this bit is set, what happens when a parity error is detected depends on
whether the @ref discard_invalid_chars or @ref mark_errors bits are set.
If neither of these bits are set, a byte with a parity error is passed to
the application as a `\0` character.

@var serial_input_modes::discard_invalid_chars
If this bit is set, any byte with a framing or parity error is ignored.
This is only useful if @ref parity_check is also set.

@var serial_input_modes::mark_errors
If this bit is set, input bytes with parity or framing errors are marked
when passed to the program. This bit is meaningful only when
@ref parity_check is set and @ref discard_invalid_chars is not set.

The way erroneous bytes are marked is with two preceding bytes, `0377` and `\0`.
Thus, the program actually reads three bytes for one erroneous byte received
from the terminal.

If a valid byte has the value `0377`, and @ref strip_8th_bit (see below) is not
set, the program might confuse it with the prefix that marks a parity error.
So a valid byte `0377` is passed to the program as two bytes, `0377 0377`,
in this case.

@var serial_input_modes::strip_8th_bit
If this bit is set, valid input bytes are stripped to seven bits;
otherwise, all eight bits are available for programs to read.

@var serial_input_modes::ignore_break
If this bit is set, break conditions are ignored.

A break condition is defined in the context of asynchronous serial data
transmission as a series of zero-value bits longer than a single byte.

@var serial_input_modes::signal_break
If this bit is set and @ref ignore_break is not set, a break condition clears
the terminal input and output queues and raises a `SIGINT` signal for the
foreground process group associated with the terminal.

If neither @ref signal_break nor @ref ignore_break are set, a break condition
is passed to the application as a single `\0` character if @ref mark_errors is
not set, or otherwise as a three-character sequence `\0377 \0 \0`.

@var serial_input_modes::ignore_cr
If this bit is set, carriage return characters (`\r`) are discarded on input.
Discarding carriage return may be useful on terminals that send both carriage
return and linefeed when you type the `RET` key.

@var serial_input_modes::map_cr_to_nl
If this bit is set and @ref ignore_cr is not set, carriage return characters
(`\r`) received as input are passed to the application as newline characters
(`\n`).

@var serial_input_modes::map_nl_to_cr
If this bit is set, newline characters (`\n`) received as input are passed to
the application as carriage return characters (`\r`).

@var serial_input_modes::enable_start_stop_in
If this bit is set, start/stop control on input is enabled.
In other words, the computer sends STOP and START characters as necessary to
prevent input from coming in faster than programs are reading it.
The idea is that the actual terminal hardware that is generating the input
data responds to a STOP character by suspending transmission, and to a START
character by resuming transmission. See Special Characters for Flow Control.

@var serial_input_modes::enable_start_stop_out
If this bit is set, start/stop control on output is enabled.
In other words, if the computer receives a STOP character, it suspends output
until a START character is received. In this case, the STOP and START
characters are never passed to the application program. If this bit is not
set, then START and STOP can be read as ordinary characters.
See Special Characters for Flow Control.

@var serial_input_modes::any_can_restart_output
If this bit is set, any input character restarts output when output has been
suspended with the STOP character.

Otherwise, only the START character restarts output.

This is a BSD extension; it exists only on BSD systems and GNU/Linux
and GNU/Hurd systems.

@var serial_input_modes::ring_bell_when_full
If this bit is set, then filling up the terminal input buffer sends a `BEL`
character (code `007`) to the terminal to ring the bell.

This is a BSD extension.
*/

/** @struct serial_control_modes
Documentation source:
https://www.gnu.org/software/libc/manual/html_node/Control-Modes.html

@var serial_control_modes::character_size
See the character_size enum for possible values

@var serial_control_modes::is_local
If this bit is set, it indicates that the terminal is connected “locally”
and that the modem status lines (such as carrier detect) should be ignored.

On many systems if this bit is not set and you call open without the
`O_NONBLOCK` flag set, open blocks until a modem connection is established.

If this bit is not set and a modem disconnect is detected, a `SIGHUP` signal
is sent to the controlling process group for the terminal (if it has one).
Normally, this causes the process to exit; see Signal Handling.
Reading from the terminal after a disconnect causes an end-of-file condition,
and writing causes an `EIO` error to be returned.
The terminal device must be closed and reopened to clear the condition.

@var serial_control_modes::hang_up
If this bit is set, a modem disconnect is generated when all processes
that have the terminal device open have either closed the file or exited.

@var serial_control_modes::read
If this bit is set, input can be read from the terminal.
Otherwise, input is discarded when it arrives.

@var serial_control_modes::two_stop_bits
If this bit is set, two stop bits are used. Otherwise, only one stop bit is used.

@var serial_control_modes::enable_parity
If this bit is set, generation and detection of a parity bit are enabled.
See @ref serial_input_modes, for information on how input parity errors are handled.

If this bit is not set, no parity bit is added to output characters,
and input characters are not checked for correct parity.

@var serial_control_modes::odd_parity
This bit is only useful if @ref enable_parity is set.
If @ref odd_parity is set, odd parity is used,
otherwise even parity is used.
*/
