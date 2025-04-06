#include "ublox.h"
#include "ublox_enums.h"

#include <libunit.h>                 // TEST_SECTION TEST_GROUP
#include <o2s/array.h>               // Array
#include <o2s/log.h>                 // log_*
#include <o2s/string_input_stream.h> // StringInputStream
#include <o2s/to_string.h>           // *_to_string

#include <stddef.h> // uint8_t

#define MU "\xb5"

#define expect_error(NAME, INPUT_STRING, LENGTH, EXPECTED)                                                \
	({                                                                                                    \
		StringInputStream stream  = cstring_input_stream(INPUT_STRING, LENGTH);                           \
		Array             storage = ArrayNew(uint8_t);                                                    \
		enum parser_error status  = ublox_parse_single_message(&stream, &storage);                        \
		if (status != EXPECTED)                                                                           \
		{                                                                                                 \
			String str = deque_to_string(&stream.buffer, (string_conversion_t)char_to_string);            \
			log_error("Parsing %s returned %u instead of %u", string_to_cstring(&str), status, EXPECTED); \
		}                                                                                                 \
		status == EXPECTED;                                                                               \
	})

#define expect_success(NAME, INPUT_STRING, LENGTH, CLASS, TYPE, SIZE)                                                  \
	({                                                                                                                 \
		StringInputStream stream  = cstring_input_stream(INPUT_STRING, LENGTH);                                        \
		Array             storage = ArrayNew(uint8_t);                                                                 \
		enum parser_error status  = ublox_parse_single_message(&stream, &storage);                                     \
		ublox_message_t*  message = (ublox_message_t*)array_first(&storage);                                           \
		status == PARSER_SUCCESS && message->ublox_class == CLASS && message->type == TYPE && message->length == SIZE; \
	})

#define expect_error_autolen(NAME, INPUT_STRING, EXPECTED) \
	expect_error(NAME, INPUT_STRING, strlen(INPUT_STRING), EXPECTED)

TEST_SECTION(parse_error_prefix, extract_name, expect_error_autolen,
             (none, "hello world !", PARSER_GARBAGE),
             (first, MU "_RXMSFRBXCKAB", PARSER_GARBAGE),
             (second, "ub\x01\x07\x01\x01what\xaa\xbb", PARSER_GARBAGE),
             (off, MU MU "b\x01\x07\x01\x01Noooo\xaa\xbb", PARSER_GARBAGE)
	);

TEST_SECTION(parse_error_class, extract_name, expect_error,
             (zero, MU "b\x00\x42\x02\x00KO\xaa\xbb", 10, PARSER_ERROR_INVALID_CLASS),
             (three, MU "b\x03\x42\x02\x00KO\xaa\xbb", 10, PARSER_ERROR_INVALID_CLASS),
             (seven, MU "b\x07\x42\x02\x00KO\xaa\xbb", 10, PARSER_ERROR_INVALID_CLASS),
             (eight, MU "b\x08\x42\x02\x00KO\xaa\xbb", 10, PARSER_ERROR_INVALID_CLASS)
	);

TEST_SECTION(parse_error_not_enough, extract_name, expect_error,
             (empty, "", 0, PARSER_ERROR_READ),
             (one, MU, 1, PARSER_ERROR_READ),
             (prefix, MU "b", 2, PARSER_ERROR_READ),
             (header, MU "b\x01\x07\x00\x00", 6, PARSER_ERROR_READ),
             (missing_payload, MU "b\x01\x07\x02\x00" "\xaa\xbb", 8, PARSER_ERROR_READ),
             (truncated_payload, MU "b\x01\x07\x04\x00" "KO" "\xaa\xbb", 10, PARSER_ERROR_READ),
             (missing_checksum, MU "b\x01\x07\x04\x00" "KOCK", 10, PARSER_ERROR_READ),
             (missing_one_byte, MU "b\x01\x07\x04\x00" "KOCKA", 11, PARSER_ERROR_READ)
	);

TEST_SECTION(parse_error_checksum, extract_name, expect_error,
             (zero, MU "b\x01\x07\x02\x00OK\xaa\xbb", 10, PARSER_ERROR_WRONG_CHECKSUM),
             (first, MU "b\x01\x07\x04\x00" "Cafe\x7b\xbb", 12, PARSER_ERROR_WRONG_CHECKSUM),
             (second, MU "b\x01\x07\x06\x00Hello!\xaa\x15", 14, PARSER_ERROR_WRONG_CHECKSUM)
	);

TEST_SECTION(parse_success, extract_name, expect_success,
             (NAV_PVT, MU "b\x01\x07\x02\x00" "OK" "\xa4\x1a", 10, NAV, PVT, 2),
             (RXM_SFRBX, MU "b\x02\x13\x05\x00" "Hello" "\x0e\x54", 13, RXM, SFRBX, 5),
             (MON_RF, MU "b\x0A\x38\x0B\x00" "Hello World" "\x69\x35", 19, MON, RF, 11)
	);

int test_parse()
{
	TEST_GROUP("parser");

	return test_parse_error_prefix()
		|| test_parse_error_class()
		|| test_parse_error_not_enough()
		|| test_parse_error_checksum()
		|| test_parse_success()
		;
}
