#include "ublox.h"
#include "ublox_enums.h"
#include "ublox_messages.h"

#include <ft_prepro/tools.h>
#include <ft_string.h>

#include <inttypes.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)
#define _append_field2(STRING, MESSAGE, TYPE, FIELD) append(STRING, ", " PP_STR(FIELD) ": %" TYPE, MESSAGE->FIELD);
#define _append_field1(STRING, MESSAGE, PAIR) _append_field2(STRING, MESSAGE, PAIR)
#define append_field(STRING, MESSAGE, PAIR) _append_field1(STRING, MESSAGE, ID PAIR)

t_string ublox_position_time_tostring(const struct ublox_position_time* message)
{
	t_string result = ublox_header_tostring(&message->header);

	FOR(EACH((PRIu32, time_of_week),
	         (PRIu16, year),
	         (PRIu8,  month),
	         (PRIu8,  day),
	         (PRIu8,  minute),
	         (PRIu8,  second),
	         ("B",    is_date_valid),
	         ("B",    is_time_valid),
	         ("B",    is_time_resolved),
	         ("B",    is_magnetic_valid),
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
