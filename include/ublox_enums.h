#pragma once

#include <blackmagic/enum.h> // DECLARE_ENUM_WITH_*

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
	(SPAN  /**< Signal characteristics         */, 0x31),
	(HW3   /**< I/O pin status                 */, 0x37),
	(RF    /**< Radio Frequency information    */, 0x38),
	(COMMS /**< Communication port information */, 0x36)
);

/**
 * Possible messages in the RXM class
 */
DECLARE_ENUM_WITH_VALUES(ublox_receiver_message,
	(SFRBX /**< Broadcast navigation data subframe */, 0x13),
	(RAWX  /**< Multi-GNSS raw measurements        */, 0x15),
	(TM    /**< Time mark data for UBX-RXM-RAWX    */, 0x74)
);

/**
 * Possible messages in the NAV class
 */
DECLARE_ENUM_WITH_VALUES(ublox_navigation_message,
	(STATUS  /**< Receiver navigation status                 */, 0x03),
	(DOP     /**< Dilution of precision                      */, 0x04),
	(TIMEGPS /**<  */, 0x20),
	(TIMEGLO /**<  */, 0x23),
	(TIMEBDS /**<  */, 0x24),
	(TIMEGAL /**<  */, 0x25),
	(TIMELS  /**< Leap second event information              */, 0x26),
	(PVT     /**< Navigation position velocity time solution */, 0x07),
	(CLOCK   /**< Clock solution                             */, 0x22),
	(EOE     /**< End of epoch                               */, 0x61)
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

DECLARE_ENUM_WITH_STRINGS(ublox_jamming_state,
	(UBX_JAMMING_UNKNOWN  /**< unknown or feature disabled     */, "unknown" ),
	(UBX_JAMMING_OK       /**< no significant jamming          */, "OK"      ),
	(UBX_JAMMING_WARNING  /**< interference visible but fix OK */, "warning" ),
	(UBX_JAMMING_CRITICAL /**< interference visible and no fix */, "critical")
);

DECLARE_ENUM_WITH_STRINGS(ublox_antenna_status,
	(UBLOX_ANTENNA_INIT ,    "initializing"),
	(UBLOX_ANTENNA_DONTKNOW, "unknown"     ),
	(UBLOX_ANTENNA_OK,       "OK"          ),
	(UBLOX_ANTENNA_SHORT,    "short"       ),
	(UBLOX_ANTENNA_OPEN,     "open"        )
);

DECLARE_ENUM_WITH_STRINGS(ublox_antenna_power,
	(UBLOX_ANTENNA_OFF,           "OFF"    ),
	(UBLOX_ANTENNA_ON,            "ON"     ),
	(UBLOX_ANTENNA_POWER_UNKNOWN, "unknown")
);
