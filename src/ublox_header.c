#include "ublox.h"
#include "ublox_enums.h"

#include <ft_string.h>

#define append(S, ...) string_append_format(&S, __VA_ARGS__)

t_string ublox_header_tostring(const struct ublox_header* message)
{
	t_string result = NEW_STRING;

	append(result, "class: %s, ", ublox_class_strings[message->class]);
	append(result, "type: %#.2hhx, ", message->type);
	append(result, "length: %hu", message->length);
	return result;
}
