#include "ublox.h"

/**
 * This checksum algorithm is the 8-bit Fletcher Algorithm,
 * which is used in the TCP standard.
 *
 * The checksum is calculated over the message, starting and including the class
 * ﬁeld up until, but excluding, the checksum ﬁelds.
 */
ublox_checksum_t ublox_compute_checksum(ublox_message_t* message, size_t size)
{
	ublox_checksum_t result = {0, 0};
	uint8_t*         byte   = (uint8_t*)message;

	while (size --> 0)
	{
		result.a += *byte++;
		result.b += result.a;
	}
	return result;
}
