#pragma once

/**
 * @file ublox.h
 * Define Ublox messages as C structures.
 *
 * This code assumes a Little Endian host.
 */

#include <ft_prepro/enum.h>

#include <sys/types.h> // size_t

#include <stdint.h> // uint*_t

extern const uint8_t sync_chars[2];

/**
 * All ublox messages inherit from that type.
 * i.e. their address can be cast to:
 * `ublox_message_t*`
 */
typedef struct ublox_header ublox_message_t;
struct ublox_header
{
	uint8_t class;   /**< Message Class */
	uint8_t  type;   /**< Message ID */
	uint16_t length; /**< Number of bytes in the payload */
};

typedef struct ublox_footer ublox_checksum_t;
struct ublox_footer
{
	uint8_t a; /**< CK_A */
	uint8_t b; /**< CK_B */
};

// clang-format off

/**
 * Message Class IDs.
 * A class is a grouping of messages which are related to each other.
 */
DECLARE_ENUM_WITH_VALUES(ublox_class,
	(NAV /**< Navigation Results Messages              */, 0x01),
	(RXM /**< Receiver Manager Messages                */, 0x02),
	(INF /**< Information Messages                     */, 0x04),
	(ACK /**< Acknowledge and Not-Acknowledge Messages */, 0x05),
	(CFG /**< Configuration Input Messages             */, 0x06),
	(UPD /**< Firmware Update Messages                 */, 0x09),
	(MON /**< Monitoring Messages                      */, 0x0A),
	(TIM /**< Timing Messages                          */, 0x0D),
	(MGA /**< Multiple GNSS Assistance Messages        */, 0x13),
	(LOG /**< Logging Messages                         */, 0x21),
	(SEC /**< Security Feature Messages                */, 0x27)
);

DECLARE_ENUM_WITH_VALUES(ublox_constellation,
	(GPS     /**< Global Positioning System, from the US          */, 0),
	(Galileo /**< From Europe                                     */, 2),
	(BeiDou  /**< BeiDou Navigation Satellite System, from China  */, 3),
	(QZSS    /**< Quasi-Zenith Satellite System, from Japan       */, 5),
	(GLONASS /**< Global Navigation Satellite System, from Russia */, 6)
);

// clang-format on

typedef struct ublox_navigation_data ubx_rxm_sfrbx;

/**
 * Broadcast Navigation Data Subframe.
 *
 * This message reports a complete subframe of broadcast navigation data
 * decoded from a single signal. The number of data words reported in each
 * message depends on the nature of the signal.
 */
struct ublox_navigation_data
{
	struct ublox_header header;
	uint8_t             constellation; /**< GNSS identifier */
	uint8_t             satellite;     /**< Satellite identifier */
	uint8_t             _reserved1;
	/**
	 * Only used for GLONASS.
	 * This is the frequency slot + 7 (range from 0 to 13)
	 */
	uint8_t             glonass_frequency;
	/**
	 * The number of data words contained in this message.
	 * (up to 10, for currently supported signals)
	 */
	uint8_t             word_count;
	/** The tracking channel number the message was received on. */
	uint8_t             channel;
	uint8_t             version; /**< Message version (=2 for this version) */
	uint8_t             _reserved2;
};

ublox_checksum_t ublox_compute_checksum(ublox_message_t* message, size_t size);

/**
@struct ublox_navigation_data

@section integration More details from the Integration manual

UBX-RXM-SFRBX reports the broadcast navigation data message the receiver has
collected from each tracked signal. When enabled, a separate message is
generated each time the receiver decodes a complete subframe of data from a
tracked signal. The data bits are reported as received, including preambles and
error checking bits as appropriate. However, because there is considerable
variation in the data structure of the different GNSS signals, the form of the
reported data also varies. This document uses the term "subframe", but other
GNSS data structures might use different terms, for example, GLONASS uses
"strings" and Galileo uses "pages".

@subsection sub1 Parsing navigation data subframes

Each UBX-RXM-SFRBX message contains a subframe of data bits appropriate for the
relevant GNSS, delivered in a number of 32-bit words, as indicated by numWords
field. Due to the variation in data structure between different GNSS, the most
important step in parsing a UBX-RXM-SFRBX message is to identify the form of the
data. This should be done by reading the gnssId field, which indicates which
GNSS the data was decoded from. In almost all cases, this is sufficient to
indicate the structure. Because of this, the following sections are organized by
GNSS. However, in some cases the identity of the GNSS is not sufficient, and
this is described, where appropriate, in the following sections. In most cases,
the data does not map perfectly into a number of 32-bit words and, consequently,
some of the words reported in UBX-RXM-SFRBX messages contain fields marked as
"Pad". These fields should be ignored and no assumption should be made about
their contents.
*/
