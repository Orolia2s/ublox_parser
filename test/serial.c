#include "serial.h"

#include <libunit.h>
#include <termios.h>

#include <stdbool.h>

#define TYPE_condition(TYPE, EXPECTED_VALUE) \
	sizeof(struct TYPE) == EXPECTED_VALUE

TEST_SECTION(serial_types, extract_name, TYPE_condition,
             (serial_control_modes, sizeof(tcflag_t)),
             (serial_input_modes, sizeof(tcflag_t))
	);


#define CONTROL_condition(NAME, INITIALIZER, EXPECTED_VALUE) \
	({                                                       \
		struct serial_control_modes v = {ID INITIALIZER};    \
		*(tcflag_t*)&v == EXPECTED_VALUE;                    \
	})

TEST_SECTION(serial_control_modes, extract_name, CONTROL_condition,
             (char_size_5,   (.character_size = character_size_5), CS5),
             (char_size_6,   (.character_size = character_size_6), CS6),
             (char_size_7,   (.character_size = character_size_7), CS7),
             (char_size_8,   (.character_size = character_size_8), CS8),
             (stop_bit,      (.two_stop_bits = true),           CSTOPB),
             (read,          (.read = true),                     CREAD),
             (enable_parity, (.enable_parity = true),           PARENB),
             (odd_parity,    (.odd_parity = true),              PARODD),
             (hupcl,         (.hang_up = true),                  HUPCL),
             (is_local,      (.is_local = true),                CLOCAL)
	);

#define INPUT_condition(NAME, INITIALIZER, EXPECTED_VALUE) \
	({                                                     \
		struct serial_input_modes v = {ID INITIALIZER};    \
		*(tcflag_t*)&v == EXPECTED_VALUE;                  \
	})

TEST_SECTION(serial_input_modes, extract_name, INPUT_condition,
	(ignore_break,          (.ignore_break = true),          IGNBRK),
	(signal_break,          (.signal_break = true),          BRKINT),
	(discard_invalid_chars, (.discard_invalid_chars = true), IGNPAR),
	(mark_errors,           (.mark_errors = true),           PARMRK),
	(parity_check,          (.parity_check = true),           INPCK)
);


int test_serial()
{
	int success = 0;
	int total   = 0;

	total++; success += !test_serial_types();
	total++; success += !test_serial_control_modes();
	total++; success += !test_serial_input_modes();
	return success != total;
}
