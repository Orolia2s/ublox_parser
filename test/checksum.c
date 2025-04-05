#include "ublox.h"

#include <libunit.h> // TEST_SECTION

#define CHECKSUM_name(MESSAGE, EXPECTED) \
	MERGE(length, ARG_COUNT MESSAGE, checksum, EXPECTED)

#define CHECKSUM_condition(MESSAGE, EXPECTED)        \
    ({                                               \
        uint8_t message[] = { ID MESSAGE };          \
        ublox_checksum_t computed =                  \
            ublox_compute_checksum((void*)message,   \
                                   sizeof(message)); \
        *(uint16_t*)&computed == EXPECTED;           \
    })

TEST_SECTION(checksum, CHECKSUM_name, CHECKSUM_condition,
	((0x02, 0x13, 0x2c, 00, 0x02, 0x0d, 0x01, 00, 0x09, 0x0b, 0x02, 00, 0xf4,
	  0xa6, 0x78, 0x3f, 0xec, 0x72, 0xa3, 0xc7, 0x84, 0x4f, 0x3b, 0x4d, 00, 00,
	  0xbb, 0xdc, 0xe1, 0xd3, 0x4e, 0x93, 0xd3, 0xc4, 0x2e, 0x37, 0xc1, 0x37,
	  0xf8, 0xb4, 00, 0x40, 0xd5, 0x49, 0x01, 00, 00, 00),
	 0x5406),
	((0x02, 0x13, 0x18, 00, 0x06, 0xff, 00, 0x02, 0x04, 0x1f, 0x02, 0x38, 0x49,
	  0x0c, 0x64, 0x45, 0xe4, 0x7d, 0x10, 0xc5, 00, 0x60, 0xec, 0x19, 0x03,
	  00, 0x27, 0x0d),
	 0xaa61),
	((0x0a, 0x09, 0x3c, 00, 00, 00, 00, 00, 0xff, 0xff, 0x01, 00, 00, 00, 00,
	  00, 00, 00, 00, 00, 00, 00,00, 00, 00, 00, 00, 0x85, 0xff, 0xff, 0x01,
	  00, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3,
	  0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 00, 0xee, 0x5a, 00, 00, 00, 00, 00,
	  00, 00, 00, 00, 00, 00, 00),
	 0xb0cb),
	((0x02, 0x13, 0x18, 00, 0x06, 0x0b, 00, 0x07, 0x04, 0x0d, 0x02, 0x38, 0x49,
	  0x0c, 0x64, 0x45, 0xe4, 0x7d, 0x10, 0xc5, 00, 0x60, 0xec, 0x19, 0x03,
	  00, 0x27, 0x0d),
	 0xd160),
	((0x02, 0x13, 0x18, 00, 0x06, 0x16, 00, 0x04, 0x04, 0x0f, 0x02, 00, 0x49,
	  0x0c, 0x64, 0x45, 0xe4, 0x7d, 0x10, 0xc5, 00, 0x60, 0xec, 0x19, 0x03,
	  00, 0x27, 0x0d),
	 0xfd32)
);
