#include "ublox.h"
#include "ublox_reader.h"

bool ublox_subscribe(ublox_reader_t* reader, ublox_callback_t callback)
{
	return array_push_back(&reader->callbacks, &callback);
}
