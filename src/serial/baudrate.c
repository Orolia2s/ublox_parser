#include "serial.h"

#include <ft_prepro/tools.h>
#include <termios.h>

#include <stdint.h>

#define possible_baudrates \
	0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, \
	19200, 38400, 57600, 115200, 230400, 460800, 500000, 576000, \
	921600, 1000000, 1152000, 1500000, 2000000, 2500000, 3000000, 3500000, \
	4000000

#define TO_BPS(_, V) case PP_CAT(B, V): return V;

/**
 * Convert the baudrate from termios to bits per second
 *
 * @return
 *  - A line speed in bits per second
 *  - `-1` if the value isn't part of the possible values
 */
int64_t serial_decode_baudrate(speed_t flag)
{
	switch (flag)
	{
		FOR(EACH(possible_baudrates), TO_BPS)
	}
	return -1;
}

#define FROM_BPS(_, V) case V: return PP_CAT(B, V);

/**
 * Convert the baudrate to a system specific enum.
 *
 * @return
 *  - A system specific value, as define in `termios.h`
 *  - 0 if the value wasn't an accepted value.
 *
 */
speed_t serial_encode_baudrate(int64_t speed_in_bps)
{
	switch (speed_in_bps)
	{
		FOR(EACH(possible_baudrates), FROM_BPS)
	}
	return 0;
}
