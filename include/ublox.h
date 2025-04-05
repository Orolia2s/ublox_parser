#pragma once

/**
 * @file ublox.h
 * Define Ublox messages as C structures.
 *
 * This code assumes a Little Endian host.
 */

#include <o2s/input_stream.h> // istream_t
#include <o2s/serial.h>       // serial_port_t
#include <o2s/string.h>       // string_t

#include <stddef.h> // size_t
#include <stdint.h> // uint*_t

extern const uint8_t        ublox_sync_chars[2];
extern const size_t         ublox_smallest_message_size;

/**
 * All ublox messages inherit from that type.
 * i.e. their address can be cast to: `ublox_message_t*`
 */
typedef struct ublox_header ublox_message_t;

/**
 * Common fields of all ublox messages.
 * This represents what's in common to all ublox messages, excluding the
 * constant preambule and the footer (because its offset varies)
 */
struct ublox_header
{
	uint8_t  ublox_class; /**< Message Class */
	uint8_t  type;        /**< Message ID */
	uint16_t length;      /**< Number of bytes in the payload */
};

/** 16-bit checksum */
typedef struct ublox_footer ublox_checksum_t;

/**
 * Concludes a frame.
 * Present at the end of every ublox message
 */
struct ublox_footer
{
	uint8_t a; /**< CK_A */
	uint8_t b; /**< CK_B */
};

bool             ublox_port_config(serial_port_t* port, int64_t baudrate);

ublox_message_t* ublox_next_message(istream_t* port);

ublox_checksum_t ublox_compute_checksum(const ublox_message_t* message, size_t size);

string_t         ublox_header_tostring(const struct ublox_header* message);

/**
@var ublox_header::class
A class is a group of messages that are related to each other

@var ublox_header::type
Deﬁnes the message that is to follow

@var ublox_header::length
The length is deﬁned as being that of the payload only. It does not include
the preamble, message class, message ID, length, or UBX checksum ﬁelds
*/
