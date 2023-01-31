#pragma once

/**
 * @file serial_options.h
 * Verbose alternative to the termios structure
 */

#include <termios.h>

#include <stdbool.h>
#include <stdint.h> // uint*_t

/**
 * Verbose alternative to termios.c_iflag
 *
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
 * Verbose alternative to termios.c_oflag
 */
struct serial_output_modes
{
	uint32_t enable_processing   :1; /**< Post-process output */
	/** Map lowercase characters to lowercase on output */
	uint32_t map_lower_to_upper  :1;
	uint32_t map_nl_to_crnl      :1; /**< Map `\n` to `\r\n` on output */
	uint32_t map_cr_to_nl        :1; /**< Map `\r` to `\n` on output */
	uint32_t no_cr               :1; /**< No `\r` output at colum 0*/
	uint32_t nl_returns          :1; /**< `\n` performs `\r` function */
	uint32_t use_fill            :1; /**< Use fill characters for delay */
	uint32_t fill_is_del         :1; /**< Fill is `DEL` */
	uint32_t _nl_delay           :1;
	uint32_t _cr_delay           :2;
	uint32_t _tab_delay          :2;
	uint32_t _backspace_delay    :1;
	uint32_t vertical_tab_delay  :1;
	uint32_t _formfeed_delay     :1;
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
 * Verbose alternative to termios.c_lflag
 *
 * Flags that require canonical mode or that are not in POSIX
 * are not documented in doxygen.
 */
struct serial_local_modes
{
	/** Generate a signal when INTR, QUIT, SUSP, or DSUSP are received */
	uint32_t enable_signals    :1;
	uint32_t canonical         :1; /**< Enable canonical mode */
	uint32_t _uppercase_only   :1; /* Not supported under Linux */
	uint32_t echo              :1; /* Echo input characters. */
	uint32_t echo_erasure      :1; /* requires canonical mode */
	uint32_t echo_kill         :1; /* requires canonical mode */
	uint32_t echo_nl           :1; /* requires canonical mode */
	uint32_t disable_flush     :1;
	/** Send SIGTOU to background jobs attempting to write to the terminal. */
	uint32_t tostop            :1;
	uint32_t _echo_control     :1; /* Escape control characters */
	uint32_t _misc             :5;
	uint32_t enable_processing :1; /* implementation-defined input processing */
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

/** Verbose alternative to struct termios */
typedef union serial_options serial_options_t;

/**
 * Holds a struct termios that can be modified using the verbose alternatives.
 */
union serial_options
{
	struct termios termios; /**< Standard structure */

	struct
	{
		struct serial_input_modes   input;   /**< Input mode flags */
		struct serial_output_modes  output;  /**< Output mode flags */
		struct serial_control_modes control; /**< Control mode flags */
		struct serial_local_modes   local;   /**< Local mode flags */
		uint8_t                     line_discipline;

		union
		{
			uint8_t array[NCCS]; /**< Standard array */

			struct
			{
				uint8_t interrupt; /**< Send a `SIGINT` signal */
				uint8_t quit;      /**< Send a `SIGQUIT` signal */
				uint8_t erase;
				uint8_t kill;
				uint8_t end_of_file;
				/** Timeout in deciseconds for noncanonical read. */
				uint8_t timeout;
				/** Minimum number of characters for noncanonical read. */
				uint8_t minimum;
			}; /**< Verbose alternative */
		} control_characters; /**< Values of special characters */

		uint32_t input_speed;
		uint32_t output_speed;
	}; /**< Verbose alternative */
};

/**
@var serial_options::interrupt
Recognized when @ref enable_signals is set, and then not passed as input.

The `INTR` (interrupt) character raises a `SIGINT` signal for all processes in
the foreground job associated with the terminal. The `INTR` character itself is
then discarded.

Typically, the INTR character is `C-c` (i.e. `0x03`, `ETX`, end of text)

@var serial_options::quit
The `QUIT` character raises a `SIGQUIT` signal for all processes in the
foreground job associated with the terminal. The QUIT character itself is
then discarded.

Typically, the QUIT character is `C-\` (i.e. `0x1c`, `FS`, file separator)
*/

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

/**
@var serial_output_modes::enable_processing
If this bit is set, output data is processed in some unspecified way so that it
is displayed appropriately on the terminal device. This typically includes
mapping newline characters (`\n`) onto carriage return and linefeed pairs.

If this bit isn’t set, the characters are transmitted as-is.

@var serial_output_modes::map_nl_to_crnl
If this bit is set, convert the newline character on output into a pair of
characters, carriage return followed by linefeed.
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

/**
@var serial_local_modes::enable_signals
This bit controls whether the INTR, QUIT, and SUSP characters are recognized.
The functions associated with these characters are performed if and only if this
bit is set. Being in canonical or noncanonical input mode has no effect on the
interpretation of these characters.

You should use caution when disabling recognition of these characters. Programs
that cannot be interrupted interactively are very user-unfriendly. If you clear
this bit, your program should provide some alternate interface that allows the
user to interactively send the signals associated with these characters, or to
escape from the program.

*/
