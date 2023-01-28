#include "ublox.h"
#include "serial.h"

#include <ft_printf.h>
#include <termios.h>

int main()
{
	ft_printf("u-blox classes :\n");
	enum ublox_class class = -1;
	while (has_next_ublox_class(&class))
		ft_printf(" - %s\n", ublox_class_strings[class]);

	ft_printf("\nGNSS Constellations:\n");
	enum ublox_constellation gnss = -1;
	while (has_next_ublox_constellation(&gnss))
		ft_printf(" - %s\n", ublox_constellation_strings[gnss]);

	int fd = ublox_open_serial_port("/dev/ttyS5");
	ft_printf("%i\n", fd);
}
