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
 *
 * Code: UBX-RXM-SFRBX
 * Type: Output
 */
struct ublox_navigation_data
{
	ublox_message_t header;
	uint8_t constellation; /**< GNSS identifier: gnssId */
	uint8_t satellite; /**< Satellite identifier: svId */
	uint8_t _reserved1;
	/** freqId. Only used for GLONASS: This is the frequency slot + 7 (range from 0 to 13) */
	uint8_t glonass_frequency;
	/**
	 * numWords.
	 * The number of data words contained in this message
	 * (up to 10, for currently supported signals)
	 */
	uint8_t word_count;
	uint8_t channel; /**< chn. The tracking channel number the message was received on */
	uint8_t version; /**< Message version. 0x02 for this version */
	uint8_t _reserved2;
};

ublox_checksum_t ublox_compute_checksum(ublox_message_t* message, size_t size);
