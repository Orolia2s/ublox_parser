#pragma once

#include <ft_prepro/enum.h>

/**
 * @file ublox_enums.h
 */

// clang-format off

/**
 * Message Class IDs.
 * A class is a grouping of messages which are related to each other.
 *
 * Those are the possible values of @ref ublox_header::class
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

/**
 * Possible messages in the MON class
 */
DECLARE_ENUM_WITH_VALUES(ublox_monitoring_message,
	(HW    /**< Hardware status                */, 0x09),
	(HW2   /**< Extended Hardware status       */, 0x0b),
	(HW3   /**< I/O pin status                 */, 0x37),
	(RF    /**< Radio Frequency information    */, 0x38),
	(COMMS /**< Communication port information */, 0x36)
);

/**
 * Possible messages in the RXM class
 */
DECLARE_ENUM_WITH_VALUES(ublox_receiver_message,
	(SFRBX /**< Broadcast navigation data subframe */, 0x13)
);

/**
 * Possible values of @ref ublox_navigation_data::constellation
 */
DECLARE_ENUM_WITH_VALUES(ublox_constellation,
	(GPS     /**< Global Positioning System, from the US          */, 0),
	(SBAS    /**< Satellite-based augmentation systems            */, 1),
	(Galileo /**< From Europe                                     */, 2),
	(BeiDou  /**< BeiDou Navigation Satellite System, from China  */, 3),
	(IMES    /**< Indoor MEssaging System, from Japan             */, 4),
	(QZSS    /**< Quasi-Zenith Satellite System, from Japan       */, 5),
	(GLONASS /**< Global Navigation Satellite System, from Russia */, 6),
	(NavIC   /**< Navigation Indian Constellation                 */, 7)
);

// clang-format on

enum ublox_jamming_state
{
	UBX_JAMMING_UNKNOWN, /**< unknown or feature disabled */
	UBX_JAMMING_OK,      /**< no significant jamming */
	UBX_JAMMING_WARNING, /**< interference visible but fix OK */
	UBX_JAMMING_CRITICAL, /**< interference visible and no fix */
	ublox_jamming_state_count
};

[[ maybe_unused ]]
static const char* ublox_jamming_state_strings[] = {"unknown", "OK", "warning", "critical"};

enum ublox_antenna_status
{
	UBLOX_ANTENNA_INIT,
	UBLOX_ANTENNA_DONTKNOW,
	UBLOX_ANTENNA_OK,
	UBLOX_ANTENNA_SHORT,
	UBLOX_ANTENNA_OPEN,
	ublox_antenna_status_count
};

[[ maybe_unused ]]
static const char* ublox_antenna_status_strings[]= {"initializing", "unknown", "OK", "short", "open"};

enum ublox_antenna_power
{
	UBLOX_ANTENNA_OFF,
	UBLOX_ANTENNA_ON,
	UBLOX_ANTENNA_POWER_UNKNOWN,
	ublox_antenna_power_count
};

[[ maybe_unused ]]
static const char* ublox_antenna_power_strings[]= {"OFF", "ON", "unknown"};
