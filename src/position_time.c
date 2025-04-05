#include "private.h" // append_field boolalpha
#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <o2s/string.h> // string_t

#include <inttypes.h> // PRI*

string_t ublox_position_time_tostring(const struct ublox_position_time* message)
{
	string_t result = ublox_header_tostring(&message->header);

	FOR(EACH((PRIu32, time_of_week),
	         (PRIu16, year),
	         (PRIu8,  month),
	         (PRIu8,  day),
	         (PRIu8,  minute),
	         (PRIu8,  second),
	         ("u",    is_date_valid),
	         ("u",    is_time_valid),
	         ("u",    is_time_resolved),
	         ("u",    is_magnetic_valid),
	         (PRIu32, time_accuracy),
	         (PRIi32, nanosecond),
	         (PRIu8,  gnss_fix),
	         (PRIu8,  satellite_count),
	         (PRIi32, longitude),
	         (PRIi32, latitude),
	         (PRIi32, height),
	         (PRIi32, height_above_sea_level),
	         (PRIu32, horizontal_accuracy),
	         (PRIu32, vertical_accuracy),
	         (PRIi32, velocity_north),
	         (PRIi32, velocity_east),
	         (PRIi32, velocity_down),
	         (PRIi32, ground_speed),
	         (PRIi32, heading)
		    ),
	    append_field, result, message);

	return result;
}
