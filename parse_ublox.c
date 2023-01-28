#include "ublox.h"
#include "serial.h"

#include <ft_printf.h>
#include <termios.h>

#define EVAL(x) #x, x

int main()
{
	int i;

	i = ublox_class_upper_bound;
	while (i --> 0)
		if (string_from_ublox_class[i])
			ft_printf("%s\n", string_from_ublox_class[i]);
	i = ublox_constellation_upper_bound;
	while (i --> 0)
		if (string_from_ublox_class[i])
			ft_printf("%s\n", string_from_ublox_class[i]);
	ft_printf("%-40s: %zu\n", EVAL(sizeof(tcflag_t)));
	ft_printf("%-40s: %zu\n", EVAL(sizeof(struct termios)));
	ft_printf("%-40s: %zu\n", EVAL(sizeof(ublox_message_t)));
	ft_printf("%-40s: %zu\n", EVAL(sizeof(struct ublox_navigation_data)));

	int fd = ublox_open_serial_port("/dev/ttyS5");
	printf("%i\n", fd);
}
