#include "serial.h"

#include <ft_prepro/color.h>
#include <ft_printf.h>
#include <libunit.h>
#include <termios.h>

#include <stdbool.h>
#include <string.h> // memcmp

// clang-format off

#define TYPE_condition(NAME, TYPE, EXPECTED) \
	sizeof(TYPE) == sizeof(EXPECTED)

TEST_SECTION(serial_types, extract_name, TYPE_condition,
	(serial_control_modes, struct serial_control_modes, tcflag_t),
	(serial_output_modes,  struct serial_output_modes,  tcflag_t),
	(serial_input_modes,   struct serial_input_modes,   tcflag_t),
	(serial_local_modes,   struct serial_local_modes,   tcflag_t),
	(serial_options,       union serial_options,  struct termios),
	(control_character,    uint8_t,                         cc_t),
	(speed,                uint32_t,                     speed_t)
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
	(stop_bit,      (.two_stop_bits  = true),          CSTOPB),
	(read,          (.read           = true),           CREAD),
	(enable_parity, (.enable_parity  = true),          PARENB),
	(odd_parity,    (.odd_parity     = true),          PARODD),
	(hupcl,         (.hang_up        = true),           HUPCL),
	(is_local,      (.is_local       = true),          CLOCAL)
);


#define INPUT_condition(NAME, EXPECTED_VALUE)          \
    ({                                                 \
        struct serial_input_modes v = {. NAME = true}; \
        *(tcflag_t*)&v == EXPECTED_VALUE;              \
    })

TEST_SECTION(serial_input_modes, extract_name, INPUT_condition,
	(ignore_break,          IGNBRK),
	(signal_break,          BRKINT),
	(discard_invalid_chars, IGNPAR),
	(mark_errors,           PARMRK),
	(parity_check,           INPCK),
	(strip_8th_bit,         ISTRIP),
	(map_nl_to_cr,           INLCR),
	(ignore_cr,              IGNCR),
	(map_cr_to_nl,           ICRNL),
	(enable_start_stop_out,   IXON),
	(any_can_restart_output, IXANY),
	(enable_start_stop_in,   IXOFF),
	(ring_bell_when_full,  IMAXBEL),
	(is_utf8,                IUTF8)
);


#define LOCAL_condition(NAME, EXPECTED_VALUE)          \
    ({                                                 \
        struct serial_local_modes v = {. NAME = true}; \
        *(tcflag_t*)&v == EXPECTED_VALUE;              \
    })

TEST_SECTION(serial_local_modes, extract_name, LOCAL_condition,
	(enable_signals,      ISIG),
	(canonical,         ICANON),
	(echo,                ECHO),
	(echo_erasure,       ECHOE),
	(echo_kill,          ECHOK),
	(enable_processing, IEXTEN)
);


#define OUTPUT_condition(NAME, EXPECTED_VALUE)          \
    ({                                                  \
        struct serial_output_modes v = {. NAME = true}; \
        *(tcflag_t*)&v == EXPECTED_VALUE;               \
    })

TEST_SECTION(serial_output_modes, extract_name, OUTPUT_condition,
	(enable_processing,  OPOST),
	(map_nl_to_crnl,     ONLCR),
	(map_cr_to_nl,       OCRNL),
	(no_cr,              ONOCR),
	(nl_returns,         ONLRET),
	(use_fill,           OFILL),
	(fill_is_del,        OFDEL),
	(vertical_tab_delay, VTDLY)
);


#define CHARS_condition(NAME, STD)          \
    ({                                      \
        serial_options_t opt = {};          \
        opt.control_characters.NAME = 0xf;  \
        opt.termios.c_cc[STD] == 0xf;       \
    })

TEST_SECTION(serial_control_characters, extract_name, CHARS_condition,
	(interrupt,  VINTR),
	(quit,       VQUIT),
	(erase,     VERASE),
	(kill,       VKILL),
	(end_of_file, VEOF),
	(timeout,    VTIME),
	(minimum,     VMIN)
);


#define WHOLE_condition(NAME, MY_INIT, STD_INIT) \
    ({                                           \
        serial_options_t mine = { ID MY_INIT };  \
        struct termios std = { ID STD_INIT };    \
        memcmp(&mine, &std, sizeof(std)) == 0;   \
    })

TEST_SECTION(serial_whole, extract_name, WHOLE_condition,
	(character_size, (.control.character_size = character_size_8), (.c_cflag = CS8 & CSIZE)),
	(canonical,    (.local.canonical = true), (.c_lflag = ICANON)),
	(input_speed,  (.input_speed  = serial_encode_baudrate(115200)), (.c_ispeed = B115200)),
	(output_speed, (.output_speed = serial_encode_baudrate(115200)), (.c_ospeed = B115200))
);

// clang-format on

int test_serial_options()
{
	TEST_GROUP("serial options");

	return test_serial_types()
		|| test_serial_control_modes()
		|| test_serial_input_modes()
		|| test_serial_local_modes()
		|| test_serial_output_modes()
		|| test_serial_control_characters()
		|| test_serial_whole();
}
