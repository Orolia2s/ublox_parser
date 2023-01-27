#include <libunit.h>
#include "serial.h"

#include <termios.h>
#include <stdbool.h>

#define CONTROL_condition(NAME, INITIALIZER, EXPECTED_VALUE)\
	({ \
		struct serial_control_modes v = {ID INITIALIZER}; \
		*(tcflag_t*)&v == EXPECTED_VALUE; \
	})

TEST_SECTION(serial_control_modes, extract_name, CONTROL_condition,
             (char_size_5,   (.character_size = character_size_5), CS5),
             (char_size_6,   (.character_size = character_size_6), CS6),
             (char_size_7,   (.character_size = character_size_7), CS7),
             (char_size_8,   (.character_size = character_size_8), CS8),
             (stop_bit,      (.stop_bit = true),                   CSTOPB),
             (read,          (.read = true),                       CREAD),
             (enable_parity, (.enable_parity = true),              PARENB),
             (odd_parity,    (.odd_parity = true),                 PARODD),
             (hupcl,         (.hupcl = true),                      HUPCL),
             (is_local,      (.is_local = true),                   CLOCAL)
	);

#define TYPE_condition(TYPE, EXPECTED_VALUE) sizeof(struct TYPE) == EXPECTED_VALUE

TEST_SECTION(serial_types, extract_name, TYPE_condition,
             (serial_control_modes, sizeof(tcflag_t))
	);

int test_serial()
{
	int success = 0;
	int total   = 0;

	total++; success += !test_serial_control_modes();
	total++; success += !test_serial_types();
	return success != total;
}
